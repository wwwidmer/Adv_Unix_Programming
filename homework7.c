/* 
  signalsuspend 

  write a program showing sigsuspend()
  init three sets of signals, old new wait
  to empty set using sigemptyset()
  add a few isgnals to this set using sigaddset()
  It should contain a signal handler, design to be shared
  across the few signals you have added to your set.
  specify a set of blocked signals, using sigprocmask()
  the signals trigged and handled you should set and
  establish handlers for using either signal or sigaction()
  then you want signals in the blocked mask to suspend until the 
  signals from the wait mask are triggered and return from their 
  handle using sigsuspend().
  after the signals in the wait mask have been delivered and hand
  led unblock the suspended signals using teh sigprocmask() again
  Insert printouts of every point in your program where you can
  show signal sets, where the signals are raised and where the
  suspended signals get unblocked.

  */

// initializes oldset, newset, waitset
// signal( , handler)
// sigprocmask(sigblock, newset,oldset)
// sigsuspend(waitmask)


#include <signal.h>
#include <stdlib.h>
#include <string.h>


int main(int argc,char *argv[]){

  return 0;
}


// signal needs handler..
void handler(int signo){
  prinf("Handler....");

}

