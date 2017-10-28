 #include "scheduler.h"
#include "device.h"

void ShortestRemainingTime::addProcess(int value)
{
    int pos;
    pos = nextEmpty;
    while ( (pos > 1) && (heap[pos/2].timeLeft() > procs[value]->timeLeft()) )
    {
        heap[pos] = heap[pos/2];	// move larger parent value down
        pos = pos/2;            // and continue upwards
    }
    heap[pos] = *procs[value];          // insert new data
    nextEmpty ++;
}

void ShortestRemainingTime::chooseProcess(int &value)
{
    value = heap[1].procID();
    
    int pos = 1;
    int child = 2;
    bool done = false;		// not done fixing the heap
    nextEmpty --;           // we will make rightmost leaf empty
    heap[pos] = heap[ nextEmpty ];            
    while (child < nextEmpty && !done )		// stop at heap bottom or when nothing moved
    {
        if ((child+1 < nextEmpty) && (heap[child].timeLeft() > heap[child+1].timeLeft())) // if right child has lesser value
            child = child+1;		// choose that one
        if (heap[child].timeLeft() > heap[pos].timeLeft())	// if parent has smaller value
            done = true;			// we are done
        else
        {
            //swap heap[pos] and heap[child]
            swap(heap[pos],heap[child]);
            pos = child;			// down the tree
            child = 2*pos;			// with new left child
        }
    }
}

void Scheduler::runScheduler(Process *tasks[], int arrival[], int size)
{
    int pid;			   // process wanting action
    int newpid;			   // newly arriving process
    Device *nextDevice;    // the device that wants to run
    char discard;		   // discard unwanted return from popFront
    
    for (int i=0; i < size; i++)
    {
        future.insert( i, arrival[i], 'X');	// all want to run
        tasks[i]->restart();			    // and start at beginning
        tasks[i]->addLog( arrival[i], '-');	// might have to wait
    }
    
    clock = 0;		    // initialize the clock
    nextDevice = &cpu;  // initialize nextDevice
    
    while ( !future.empty() || !noneReady() )
    {
        cpu.restart();
        disk.restart();
        net.restart();
        console.restart();
        user.restart();
        
        if ( noneReady() )		//if readyset is empty, add a future process to readyset
        {
            clock = future.leadTime();		   //clock starts at time of first process
            future.popFront( pid, discard );   //remove process from future
            addProcess( pid );			       //add process to readyset
        }
        else 				    //run first processes in readyset
        {
            chooseProcess(pid);		     //remove process from readyset
            tasks[pid]->run( clock, allowance(), nextDevice );  //run process
            
            //some tasks may have arrived in the meantime, so get those
            while ( !future.empty() && clock >= future.leadTime() )
            {
                future.popFront( newpid, discard ); //remove from future
                addProcess( newpid );               //add to readyset
            }
            
            //If nextDevice is Null, nextRequest[] is complete.
            //  Else, if nextDevice is cpu ('X') add it back to readyset
            //  otherwise nextDevice will be either user ('U'), disk ('D'),
            //  network ('N'), or console ('I') and scheduler should
            //  request use of them.
            if(nextDevice == NULL){}
            else if (nextDevice == &cpu)
                addProcess( pid );
            else
                nextDevice->request(pid, clock, tasks, future);
        }
    }
}