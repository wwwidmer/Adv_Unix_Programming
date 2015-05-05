/*
Write a user daemon that ouputs the current time every 10 secondsinto a log file at teh same directory. 
Then change the configuration file to output elsewhere. 
Have the configuration file redirect the log file to this place.
Send the daemon a signal telling it that the cconfiguration file was changed. 
Forcing it to reread and start ending log messages to the new location.
 */

/*
 * output time
 * check timed.conf at startup
 * recieve signal that timed.conf has changed. 
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <syslog.h>
#include <pthread.h>

sigset_t mask;
char* logfile;
char line[128];
char timebuffer[26];
const char* conf = "timed.conf";

int formatTime();

void reread(void){

  FILE *rr;
  rr = fopen(conf,"r");
  char tc;
  if (rr == NULL){
    exit(1);
  }
  while(fgets(line,sizeof(line),rr)!= NULL){
    memmove(line,line+10, strlen(line));
  }
  printf("%s",line);
  logfile = (char *)line;
  logfile[strlen(logfile)-1] = '\0';
  fclose(rr);
}

void * thr_fun(void *arg){
  int err, signo;

  for(;;){
    err = sigwait(&mask, &signo);
    // erro trarp
    if( err !=0){
      exit(1);
    }
    switch(signo){
    case SIGHUP:
      syslog(LOG_INFO, "Rereading conf");
      reread();
      break;
    case SIGTERM:
      syslog(LOG_INFO, "exiting");
      exit(0);
    default:
      syslog(LOG_INFO, "signal unexpected");
    }
    return (0);
  }


}

int main(int argc, char* argv[]){
  int err;
  pid_t process_id = 0;
  pid_t sid=0;
  pthread_t tid;
  struct sigaction sa;
  //logfile = "time.txt";
  reread();
  // Daemonize
  process_id = fork();
  // fork fail
  if(process_id<0){
    exit(1);
  }
  // parent process needs to be killed
  if(process_id>0){
    printf("%d \n", process_id);
    exit(0);
  }
  //umask file mode and start new session
  
  umask(0);
  sid=setsid();
  // sid fail
  if(sid<0){
    exit(1);
  }
  // close stdin, stdout, stderr
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  // Now we're a daemon...
  // Handle .conf file changes 
  sa.sa_handler = SIG_DFL;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigfillset(&mask);
  //thread to handle signals
  err = pthread_create(&tid, NULL, thr_fun, 0);
   
  // Do the assignment
  clocker();
  return 0;
}
 

int clocker(){
  FILE *fp;
  while(1){
    reread();
    formatTime();
    fp = fopen(logfile,"w+");
    fprintf(fp, "%s \n", timebuffer);
    fflush(fp);
    sleep(10);
  }
  fclose(fp);
  return 0;
}

int formatTime(){
  time_t timer;
  struct tm* tm_i;
  
  time(&timer);
  tm_i = localtime(&timer);
  strftime(timebuffer, 26, "%Y:%m:%d %H:%M:%S",tm_i);
  
  return 0;
}
