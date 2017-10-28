// Device Driver Simulation
// A simplified version of the behavior of a device driver.
// It is notified of when a request arrives, and then it
// determines when that action begins and ends.

#include "proclist.h"

class Process;
class Device
{
private:
    char action;		// letter used for display
    int duration;		// how long the operation might take
    int readyTime;		// when next operation may start
public:
    Device( char a, int d ) : action(a), duration(d), readyTime(0) { }
    void restart() {readyTime = 0;}
    virtual void request(int, int, Process* [], ProcList &);
    int getDuration() {return duration;}
   
};

//Simulates a user input to the operating system. This class is treated
// as a device which generates processess created by the user/console.
// Not to be confused with the interactive console.
class User : public Device
{
private:
    int processCount = 1;   // number of processes currently active
    char action;            // letter used for display
    int duration;           // how long the operation might take
    int readyTime;          // when next operation may start
public:
    User( char a, int d) : Device(a,d), action(a), duration(d), readyTime(0) { }
    void request(int, int, Process* [], ProcList &);
};

extern Device disk, net, console, cpu;
extern User user;