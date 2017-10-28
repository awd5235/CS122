// A quick driver to test the History Display routine
// It just fabricates a single job history to see what it does
//
#include <iostream>
using namespace std;

#include "scheduler.h"
#include "device.h"

int main()
{
    Scheduler *scheds[] = 	    // make some process schedulers
    { new FCFS(), new RoundRobin(), new Priority(), new Preempt(), new ShortestRemainingTime() };
    Process *tasks[10] = {new Shell(0)};
    int arrival[] = {0};   // arrive at these times
    
    for (int i=0; i<5; i++)
    {
        scheds[i]->runScheduler(tasks, arrival, 1);
        displayHistory(tasks, 10, 0, 5000);
    }
}
