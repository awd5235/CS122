#include "proclist.h"
#include <stdlib.h>

// A description of a process, executed in the process scheduling simulation
class Device;
class Process
{
protected:
    int myId;		// an identifier for the larger picture
    
    // A description of the process's total CPU needs
    int bursts;		// total number of CPU bursts (<= 10)
    int usages[50];		// lengths of each burst
    Device *nextRequest[50];	// what to do after each burst
    
    // A desription of what this process is doing now
    int currentCycle;	// which burst is next ro run or continue
    int remainingTime;	// how much time left on current burst
    
    // A desription of what this process has done so far
    ProcList log;
    
public:
    int procID()
    {
        return myId;
    }
    
    int timeLeft()
    {
        return remainingTime;
    }
    
    void restart()	// start at the very beginning
    {
        currentCycle = 0;
        remainingTime = usages[0];
        log.clear();	// empty the log
    }
    
    void addLog( int time, char state )
    {
        log.pushBack( myId, time, state );
    }
    
    ProcList &getLog()
    {
        return log;		// get summarized results at end
    }
    
    virtual bool isInteractive()
    {
        return false;	// assume a background job
    }
    
    void run( int &, int, Device *& );
};

class Computation : public Process
{
public:
    Computation( int id );
};

class Download : public Process
{
public:
    Download( int id );
};

class Interact: public Process
{
public:
    Interact( int id );
    bool isInteractive()
    {
        return true;
    }
};

class Game: public Process
{
public:
    Game(int id);
    /*bool isInteractive()
    {
        return true;
    }*/
};

class Shell: public Process
{
public:
    Shell(int id);
};
