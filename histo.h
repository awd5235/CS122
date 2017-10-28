#include "process.h"

// History Displayer
// Produces an Ascii-graphic representation of the history of a process
// within a simulation of job scheduling.  The history is recorded as a
// series of time stamps, represented here as integer time values and
// characters representing state.
// Paramaters:
//     history  input ProcList array	linked lists of events and times
//     size	input int		number of elements in array
//     start	input int		beginning of time frame of interest
//     stop	input int		end of time frame of interest
// Pre-Conditions:
//     'history' concludes with an entry with state 'Q' for completion
//     'start' and 'stop' are both positive integers, with start < stop
//     NOTE:  either 'start' or 'stop' may exceed the largest value in 'when'
// Results:
//     A display of between 20 and 50 printable characters representing
//     the job history within the time range of interest, using the
//     characters stored within 'what'.  Events outside the actual range
//     of the job history will be displayed with blanks.
void displayHistory( Process *history[], int size, int start, int stop );

