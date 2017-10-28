#include <iostream>
using namespace std;

// List of Process ProcList Information
// This is a simple linked list, all of whose elements record
// information about a process in a simulated process scheduler.
// This list data structure will be used for three different purposes:
// -- a record of the past history of a particular process
// -- a collection for the job scheduler of processes currently wishing to run
// -- a task list for known future events for the simulation

#include "proclist.h"

// First, some helper functions to display the data

ostream &operator<<( ostream &stream, ProcListElement &ele )
{
    stream << "(" << ele.procID << "," << ele.state << ","
    << ele.time << ")";
    return stream;
}

ostream &operator<<( ostream &stream, ProcList &hist )
{
    for (ProcListElement *curr = hist.head; curr != NULL; curr = curr->next )
        stream << *curr;
    stream << endl;
    return stream;
}

// And some functionality for the list itself
ProcIterator ProcList::begin()
{
    return ProcIterator( this, head );
}

ProcIterator ProcList::end()
{
    return ProcIterator( this, NULL );
}

void ProcList::pushBack( int procId, int time, char state )
{
    ProcListElement *newEle = new ProcListElement( procId, time, state );
    if (head == NULL)
        head = tail = newEle;
    else
    {
        tail = tail->next = newEle;
    }
}

void ProcList::popFront( int &procId, char &state )
{
    ProcListElement *removal = head;
    head = head->next;
    if (head == NULL)
        tail = NULL;
    
    procId = removal->procID;
    state = removal->state;
    delete removal;
}

void ProcList::insert( int procId, int time, char state )
{
    ProcListElement *newEle = new ProcListElement( procId, time, state );
    if (head == NULL)
        head = tail = newEle;
    else if (time < head->time )
    {
        newEle->next = head;
        head = newEle;
    }
    else if (time >= tail->time )
    {
        tail = tail->next = newEle;
    }
    else
    {
        ProcListElement *scan;
        for (scan = head; scan->next->time < time; scan = scan->next) { }
        newEle->next = scan->next;
        scan->next = newEle;
    }
}




// EXTRA CREDIT
// Condenses the information in the linked list after it is collected.
// 1) If two adjacent cells have the same time information,
//    the first can be removed (did not actually occupy that state)
// 2) If two adjacent cells have the same state recorded,
//    the second can be removed (did not actually change state)
void ProcList::condense()
{
    ProcListElement *scan;		// to examine the data
    ProcListElement *removal;		// to remove an element
    
    // Although it is possible to accomplish both of the above in a single loop,
    // it is probably much simpler and clearer to handle them separately.
    
    // remove every element whose time equals the one following
    // the head must be handled, as a special case;   tail will never be removed here
    while (head->time == head->next->time)
    {
        removal = head;
        head = head->next;
        delete removal;
    }
    //  removing any other element requires updating the link that points at it
    for (scan = head; scan->next != tail; scan = scan->next )
    {
        if (scan->next->time == scan->next->next->time)
        {
            removal = scan->next;
            scan->next = scan->next->next;
            delete removal;
        }
    }
    
    //  remove every element whose state equals the one before it
    //  here, the head will never be removed, but the tail could be
    //  With Round Robin, there could be many back to back X's
    for (scan = head; scan != tail; scan = scan->next )
    {
        while (scan != tail && scan->state == scan->next->state )
        {
            removal = scan->next;
            scan->next = scan->next->next;
            if (removal == tail)
                tail = scan;
            delete removal;
        }
    }
}