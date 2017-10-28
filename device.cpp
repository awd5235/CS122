#include "device.h"
#include "process.h"

//  Simulate a request to an I/O device
//  Parameters:
//      pid	(input int)		which process is making the request
//      clock	(input int)		time at which request is made
//      tasks	(modified array)	Process information (to record events)
//      future	(modified ProcList)	record when operation is complete
void Device::request( int pid, int clock, Process *tasks[], ProcList &future )
{
    if (readyTime <= clock)
        readyTime = clock;                  // start acting now
    tasks[pid]->addLog(readyTime, action);  //note the start of a request
    readyTime = readyTime + duration;       //increment the clock
    tasks[pid]->addLog(readyTime, '-');     //log the completion
    future.insert(pid, readyTime, 'X');     //add to future
}

void User::request(int pid, int clock, Process *tasks[], ProcList &future)
{
//----------Request User device (same as Device::request)------------
    if (readyTime <= clock)
        readyTime = clock;                  // start acting now
    tasks[pid]->addLog(readyTime, action);  //note the start of a request
    readyTime = readyTime + duration;       //increment the clock
    tasks[pid]->addLog(readyTime, '-');     //log the completion
    future.insert(pid, readyTime, 'X');     //add to future
   
//-----------------------Create a process------------------
    //Once initial request is complete, User device now selects a new process
    // to add to tasks[] and future. Tasks[] is a list of all currently active
    // processes. For the sake of simulation, no more than 4 active processes
    if(processCount < 10)
    {
        int rando = rand() % 4;  //random number used to generate a random process
        
        switch(rando)
        {
            case 0:     //Generate Computation Process
                tasks[processCount] = new Computation(processCount);
                future.insert(processCount, readyTime, 'X');    // Insert into future
                tasks[processCount]->addLog(readyTime, '-');	// Process is waiting. Update log.
                tasks[processCount]->restart();                 // and start at beginning
                break;
                
            case 1:     //Generate Download Process
                tasks[processCount] = new Download(processCount);
                future.insert(processCount, readyTime, 'X');    // Insert into future
                tasks[processCount]->addLog(readyTime, '-');	// Process is waiting. Update log.
                tasks[processCount]->restart();                 // and start at beginning
                break;
                
            case 2:     //Generate Interactive Process
                tasks[processCount] = new Interact(processCount);
                future.insert(processCount, readyTime, 'X');    // Insert into future
                tasks[processCount]->addLog(readyTime, '-');	// Process is waiting. Update log.
                tasks[processCount]->restart();                 // and start at beginning
                break;
             
                
            case 3:     //Generate Interactive Process
                tasks[processCount] = new Game(processCount);
                future.insert(processCount, readyTime, 'X');    // Insert into future
                tasks[processCount]->addLog(readyTime, '-');	// Process is waiting. Update log.
                tasks[processCount]->restart();                 // and start at beginning
                break;
        }
        processCount++;
    }
}

//  Each device records a letter to be used in the summary display
//  and the time the operation is expected to take
Device disk( 'D', 200 ), 	// disk has 'slow' moving parts
net( 'N', 100 ), 	// networks are faster nowadays
console( 'I', 1000 ), 	// have to wait for user to respond
cpu('X',0);		// not external, but used for type compatibility
User user('U', 200);
//  When each process runs with the CPU, it will use this list to identify
//  what it wishes to do next (using the cpu object to continue running)
