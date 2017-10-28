#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

#include "process.h"


// Extra credit attempt
// Compute the standard deviation for an interactive/noninteractive process
// given the array of response/turnAround times, average time, and number of occurances
double stdDev(int times[], int avg, int count)
{
    // std deviation defined as sqrt[(1/n)sum((x-u)^2)]
    // where n=number of times, u=avg time, and x=current time
    double sum = 0;
    for(int x=0; x<count; x++)
        sum = sum + pow((times[x] - avg), 2);
    sum = sum/count;
    sum = sqrt(sum);
    return sum;
}

// History Displayer
// Produces an Ascii-graphic representation of the history of a process
// within a simulation of job scheduling.  The history is recorded as a
// series of time stamps, represented here as integer time values and
// characters representing state.
// Paramaters:
//     history	input procList array	linked lists of events and times
//     size	input int		number of elements in array
//     start	input int		beginning of time frame of interest
//     stop	input int		end of time frame of interest
// Pre-Conditions:
//     The elements in 'history' are in non-decreasing order
//     'history' ends with an entry with 'Q' as its state
//     There is at least one entry before the 'Q'
//     'start' and 'stop' are both positive integers, with start < stop
//     NOTE:  either 'start' or 'stop' may exceed the largest time in 'history'
// Results:
//     A display of between 20 and 50 printable characters representing
//     the job history within the time range of interest, using the
//     characters stored within 'history'.  Events outside the actual
//     range of the process history will be displayed with blanks.

void displayHistory(Process* history[], int size, int start, int stop )
{
    
    //--------------------- character display ------------------------------
    char display[50];			// to hold the output
    int outpos;                 // next output position to fill
    char currentState;			// current process state in history
    int time;                   // current time examined
    int range = stop - start;		// total time period
    int increment = 1 + range / 40;	// round fractions upwards
    
    cout << "Time:  " << setw(4) << start <<
    setw( range/increment ) << stop << endl;
    
    for (int j=0; j<size; j++)
    {
        for (int i=0; i<50; i++)		// clear display
            display[i] = ' ';
        
        ProcList &pl = history[j]->getLog();	// find out about this task
        pl.condense();			// trim data (if implemented)
        ProcIterator iter = pl.begin();	// starting at its first point
        
        //  Identify where to begin recording output data, and
        //  what time to begin analyzing (tracking may start early)
        iter = pl.begin();
        if (iter.time() > start)		// has not yet started yet
            outpos = (iter.time()-start) / increment;	// skip spaces
        else
            outpos = 0;				// start at beginning of display
        time = start + outpos*increment;	// identify smulation time
        
        //  Identify where first applicable at appears
        //  (tracking may start in the middle)
        currentState = iter.state();	// initialize if loop never repeats
        while (time <= stop && time >= iter.time())
        {
            currentState = iter.state();
            iter.advance();
        }
        //  currentState represents the process state at 'time'
        //  and changes from that state at iter.time()
        
        // collect relevant data into the output array
        while ( time <= stop && currentState != 'Q' && iter != pl.end() )
        {
            // currentState extends to iter.time(), so record it until
            // that time is reached.
            while ( time <= stop && time < iter.time() )
            {
                display[outpos] = currentState;
                outpos++;
                time += increment;
            }
            currentState = iter.state();	// change to this state
            iter.advance();			// and find when it ends
            
            // It is possible that the last increment jumped over a state
            // so we will record any missing X's in last output position
            // It is also very possible that many states got missed,
            // so we will attempt to examine all of them for X's
            while (currentState != 'Q' && time >= iter.time() )
            {
                if (currentState == 'X')	// missed this execution
                    display[outpos-1] = 'X';	// record very short CPU burst
                currentState = iter.state();	// what did we change to?
                iter.advance();			// find where that ends
            }
            
        }
        display[outpos] = '\0';
        cout << "History: " << display << endl;
    }
    
    //-------------------- Run Time Statistics --------------------------------
    
    int turnAroundTimes[10];    // Holds individual turn around times of each non interactive process
    int turnAroundSum = 0;      // Running sum of noninteractive process turn around times
    int noninteractiveCount = 0;// Number of noninteractive processes
    int avgTT = 0;              // Average turnaround time for non-interactive processes
    double stdDevNoninteractive = 0;//Stores std deviation of turn around times
    
    int responseTimes[10];      // Holds individual response times of each interactive process
    int responseTimeSum = 0;    // Running sum of interactive process response times
    int interactiveCount = 0;   // Number of breaks between an interactive process
    int startTime = 0;          // Time at which a break in 'I's begins
    int endTime = 0;            // Time at which a break in 'I's ends
    int maxRT = 0;              // Maximum response time for the interactive process
    int avgRT = 0;              // Average response time for the interactive process
    double stdDevInteractive = 0;  // Stores std deviation of response times
    
    for(int k=0; k<size; k++)
    {
        ProcList &pl = history[k]->getLog();	// find out about this task
        pl.condense();                          // trim data (if implemented)
        ProcIterator iter = pl.begin();         // starting at its first point
        
        if(history[k]->isInteractive())
        {
            // interctive ---> determine responseTimes[interaciveCount], responseTimeSum, and maxRT
            // increment interactiveCount.
            
            while(iter.state() != 'I')  // Find first instance of I
               iter.advance();
            while(iter != pl.end())     // Traverse the entire log
            {
                while(iter.state() == 'I')  // Pass through the current sequence of I's
                    iter.advance();
                
                startTime = iter.time();    // Record start time of intermission
                
                while(iter != pl.end() && iter.state() != 'I')  //Find next instance of I
                    iter.advance();
                
                if(iter != pl.end())
                    endTime = iter.time();  // Record end time of intermission
                else
                    endTime = pl.tailTime();
                
                responseTimes[interactiveCount] = endTime - startTime;    // Record the response time
                responseTimeSum = responseTimeSum + responseTimes[interactiveCount]; // update the running sum
                
                if(responseTimes[interactiveCount] > maxRT)
                    maxRT = responseTimes[interactiveCount]; // Update the maximum response time
                
                interactiveCount++;
            }
        }
        else
        {
            // non-interctive ---> determine turnAroundTimes[noninteractiveCount]
            //   and turnAroundSum. Increment noninteractiveCount.
            
            turnAroundTimes[noninteractiveCount] = pl.tailTime()-pl.leadTime();     //Record turn around time
            turnAroundSum = turnAroundSum + turnAroundTimes[noninteractiveCount];   //update running sum
            noninteractiveCount++;
        }
    
    }
    
    avgTT = turnAroundSum/noninteractiveCount;      // compute average turn around time
    
    //To avoid division by 0
    if(interactiveCount == 0)
        avgRT = 0;
    else
        avgRT = responseTimeSum/interactiveCount;       // compute average response time
    
    stdDevNoninteractive = stdDev(turnAroundTimes,avgTT,noninteractiveCount);
    stdDevInteractive = stdDev(responseTimes,avgRT,interactiveCount-1);
    
    cout << "----------------Runtime Analysis-----------------" << endl;
    cout << "Average Turnaround Tme = " << avgTT << endl;
    cout << "Average Response Time = " << avgRT << endl;
    cout << "Maximum Response Time = " << maxRT << endl;
    cout << "Std. dev. of interactive processes = " << stdDevInteractive << endl;
    cout << "Std. dev. of noninteractive processes = " << stdDevNoninteractive << endl;
    cout << "\n" << endl;
}
