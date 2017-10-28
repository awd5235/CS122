#include "process.h"
#include "device.h"

//  Run a Process for some time
//  The process would like to complete its current CPU burst,
//  but is currently only allowed what is granted to it.
//  Parameters:
//      clock           (modified int)  time on simulation clock
//      allowance       (input int)     time allowed to run with
//      next            (output Device) what device should be used next
//		next is from { disk, net, console, cpu )
//  Post-Condition:
//      the clock will have advanced until either the allowed time
//      has been used up, or the current CPU burst is complete
//      (whichever happens earlier).  There will be no idle CPU time.
//  Also:  The history log for this Process will record what is known
//      at this time
void Process::run( int &clock, int allowance, Device *&next )
{
    
    addLog(clock, 'X');		// running now
    if ( allowance >= remainingTime )
    {
        clock += remainingTime;		// use all the time needed
        next = nextRequest[currentCycle];
        if(next == NULL)
            addLog(clock, 'Q');
        else{
            currentCycle++;
            remainingTime = usages[currentCycle];
            addLog( clock, '-' );		// record completion
        }
    }
    else
    {
        clock += allowance;		// uses the time that was given
        remainingTime -= allowance;	// and save some work for later
        next = &cpu; // wish to keep running
        addLog( clock, '-' );		// wait to execute again
    }
}

Computation::Computation( int id )
{
//---------CPU and disk activity ('X' and 'D')------------
    myId = id;
    bursts = 6 + rand() % 10;    // Number of CPU bursts
    for (int i=0; i < bursts; i++)
    {
        usages[i] = 20 + rand() % 120; // Length of each burst
        nextRequest[i] = &disk;         // Which device to use after each burst
    }
    nextRequest[bursts-1] = NULL;	// Last array element is NULL
}

Download::Download( int id )
{
//-------CPU, Net, and Disk Activity ('X', 'N', 'D')--------
    myId = id;
    bursts = 17 + rand() % 16;		// Number of CPU Bursts
    for (int i=0; i < bursts; i++)
    {
        usages[i] = 10 + rand() % 20;   // Length of each burst
        if (i%2 == 0)                   // Which device to use after each burst?
            nextRequest[i] = &net;          // Even indices follow each burst with net activity
        else
            nextRequest[i] = &disk;         // Odd indices follow each burst with disk activity
    }
    nextRequest[bursts-1] = NULL;       // Last element NULL
}

Interact::Interact( int id )
{
//----------CPU and Console Activity ('X' and 'I')----------------
    myId = id;
    bursts = 25 + rand() % 5;            // Number of CPU bursts
    for (int i=0; i < bursts; i++)
    {
        usages[i] = 10 + rand() % 5;   // Length of each CPU burst
        nextRequest[i] = &console;      // which device to use after each burst
    }
    nextRequest[bursts-1] = NULL;	// Last element NULL
}

Game::Game(int id)
{
//-------Cpu, console, and Net Activity ('X', 'I', and 'N')--------
    myId = id;
    bursts = 20 + rand() % 10;		// Number of CPU Bursts
    for (int i=0; i < bursts; i++)
    {
        usages[i] = 40 + rand() %  25;  // Length of each burst
        if (i%2 == 0)                   // Which device to use after each burst?
            nextRequest[i] = &console;          // Even indices follow each burst with net activity
        else
            nextRequest[i] = &net;         // Odd indices follow each burst with disk activity
    }
    nextRequest[bursts-1] = NULL;       // Last element NULL
}

Shell::Shell( int id )
{
    myId = id;
    bursts = 10 + rand() % 3;
    for (int i=0; i < bursts; i++)
    {
        usages[i] = 5 + rand() % 20;
        nextRequest[i] = &user;	//User
    }
    nextRequest[bursts-1] = NULL;
}
