#include <iostream>
using namespace std;

#include "histo.h"
#include "proctree.h"

// Process Scheduler
// This represents the part of an operating system that manages processes,
// choosing which to assign the CPU (assuming a single-processor system).
// It will maintain a collection of processes that are currently ready
// to use the CPU, and then choose which to run next.
//
// The time on the system clock is not required for this object,
// so all of the time fields in the linked list objects are set to zero.

class Scheduler
{
protected:
    ProcList readySet;	// set of processes ready to run
    ProcList future;	// list of future events
    int clock;		// current clock time for simulation
    string name;		// name of the scheduling algorithm
public:
    virtual void addProcess( int procId )
    {
        readySet.pushBack( procId, 0, 'X');
    }
    virtual void chooseProcess( int &procId )
    {
        char hold;
        readySet.popFront( procId, hold );
    }
    virtual int allowance()
    {
        return 100000;	// a long time
    }
    virtual bool noneReady()
    {
        return readySet.empty();
    }
    virtual void runScheduler( Process* [], int[], int );
};

class FCFS : public Scheduler
{
public:
    FCFS() { name="First Come First Served"; }
};

class RoundRobin : public Scheduler
{
public:
    RoundRobin() { name="Round Robin"; }
    int allowance()
    {
        return 70;
    }
};

class Priority : public Scheduler
{
private:
    ProcTree tree;		// record order in a tree
public:
    Priority() { name="Priority"; }
    void addProcess( int procId )
    {
        tree.insert( procId );
    }
    void chooseProcess( int &procId )
    {
        procId = tree.removeMax();
    }
    bool noneReady()
    {
        return tree.empty();
    }
};

class Preempt : public Priority
{
private:
    ProcTree tree;
public:
    Preempt() { name="Preemptive Priority"; }
    
    int allowance()
    {
        if(!future.empty() && clock <= future.leadTime())
            return future.leadTime()-clock;
        else
            return 100000;
    }
    
};

class ShortestRemainingTime : public Preempt
{
private:
    Process **procs;        // this scheduler's way of getting to process info
    Process heap[20];           // heap implemented as an array
    int nextEmpty;          // index of the next empty spot in the array
    
public:
    ShortestRemainingTime() { name="Shortest Remaining Time"; nextEmpty = 1; }
    
    void addProcess(int value);
    
    void chooseProcess(int &value);
    
    bool noneReady()
    {
        return nextEmpty == 1;
    }
    
    //  grabs the process information, and then runs the simulation
    void runScheduler( Process* tasks[], int arrival[], int size)
    {
        procs = tasks;
        Scheduler::runScheduler( tasks, arrival, size );
    }
};