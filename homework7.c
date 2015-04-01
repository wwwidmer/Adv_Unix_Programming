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

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
static void handler(int);

int main(int argc,char *argv[]){
  sigset_t newset, oldset, waitset;

  //  printf("Starting...");
  printf("Starting\n");
  
  if (signal(SIGINT, handler) == SIG_ERR)
    printf("Signal Error..."); 

  sigemptyset(&waitset);
  sigaddset(&waitset, SIGUSR1);
  sigemptyset(&newset);
  sigaddset(&newset,SIGINT); 

  
  // Block sigint and save current signal mask
  if (sigprocmask(SIG_BLOCK, &newset, &oldset) < 0)
    printf("SignalProc Error...");

  printf("Critical section\n");

  if (sigsuspend(&waitset) != -1)
    printf("Sus error"); 

  printf("After suspending...\n");
  
  if(sigprocmask(SIG_SETMASK, &oldset, NULL) < 0 )
    printf("sigsetmask  error"); 

  
  printf("Exit\n");
  exit(0);
}


// signal needs handler..
static void handler(int signo){
  printf("\nHandler for signal number: %i....\n",signo);
}

