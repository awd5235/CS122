#include <iostream>
using namespace std;

#ifndef PROCLIST
#define PROCLIST

// List of Process History Information
// This is a simple linked list, all of whose elements record
// information about a process in a simulated process scheduler.
// This list data structure will be used for three different purposes:
// -- a record of the past history of a particular process
// -- a collection for the job scheduler of processes currently wishing to run
// -- a task list for known future events for the simulation
//
class ProcIterator;		// forward type declarations
class ProcList;			// since they all interact

class ProcListElement
{
    friend class ProcList;	// these elements only used by ProcList
    friend class ProcIterator;	//     its iterator, and an output function
    friend ostream& operator<<( ostream &, ProcListElement & );
    friend ostream& operator<<( ostream &, ProcList & );
    
private:
    int  procID;		// process this applies to
    int  time;		// time stamp for event
    char state;		// process state at that time
    ProcListElement *next;	// linked list link
    ProcListElement( int i, int t, char s ) :
    procID(i), time(t), state(s), next(NULL) { }
};

class ProcList
{
    friend class ProcIterator;
    friend ostream& operator<<( ostream &, ProcList & );
private:
    ProcListElement *head, *tail;	// endpoints of list
    
public:
    ProcList() : head(NULL), tail(NULL) { }
    ~ProcList()		// destructor
    {
        clear();
    }
    
    bool empty()		// identify whether list is empty
    {
        return head == NULL;
    }
    int leadTime()		// identify time of first event in list
    {
        return head->time;
    }
    int tailTime()		// identify time of last event in list
    {
        return tail->time;
    }
    void clear()		// erase the list
    {
        int i; char c;	// just placeholders
        while (head != NULL)
        {
            popFront( i, c );
        }
    }
    ProcIterator begin();
    ProcIterator end();
    
    void pushBack( int, int, char );	// add new element to end
    void insert( int, int, char );		// add element in time order
    void popFront( int&, char& );		// remove element from front
    
    void condense();		// EXTRA CREDIT function //
};

// and an iterator to help visit all the data
class ProcIterator
{
    friend class ProcList;	// List can create Iterators
private:
    ProcList *list;			// which list are we visiting?
    ProcListElement *current;	// which element now?
    ProcIterator( ProcList *l, ProcListElement *e ) :
    list(l), current(e) { }
    
public:
    bool operator!=( ProcIterator other )	// to compare to end()
    {
        return list != other.list || current != other.current;
    }
    int process() const	{ return current->procID; };
    int time() const	{ return current->time;	};
    char state() const	{ return current->state; };
    void advance()		{ current = current->next; }
};

#endif