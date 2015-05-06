/* 2015
 * DAEMON.C
 * WILLIAM WIDMER
 * ASSIGNMENT
 * CREATE AN EXAMPLE DAEMON PROCESS WHOSE PURPOSE IS TO OUTPUT
 * THE CURRENT (FORMATTED) TIME INTO A TEXT FILE.
 * DAEMON SHOULD READ A CONF FILE TO DETERMINE THE OUTPUT FILE.
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

/* 
 * IO functions
 *
 * formatTime
 * Format a time to be human readable.
 *
 * reread
 * read the file conf, determine logfile used elsewhere.
 */
int formatTime();

void reread(void){

  FILE *rr;
  rr = fopen(conf,"r");
  char tc;
  if (rr == NULL){
    exit(1);
  }
  // memmove away from config variable name
  while(fgets(line,sizeof(line),rr)!= NULL){
    memmove(line,line+10, strlen(line));
  }
  // Tell user the first output file.
  // Since we're a daemon printf won't go anywhere after we daemonize.
  printf("%s",line);
  logfile = (char *)line;
  // config variables contain eol or eof, null it.
  logfile[strlen(logfile)-1] = '\0';
  fclose(rr);
}
/*
 * Thread to catch SIGHUP
 * To call reread.
 */
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
/*
 * main
 * initialing read conf file, daemonize, error catch, and
 * call necessary functions.
 */
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
 
/*
 * clocker
 * Write to logfile the current time.
 */
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
// Format a Y m D H M S time struct into a string.
int formatTime(){
  time_t timer;
  struct tm* tm_i;
  
  time(&timer);
  tm_i = localtime(&timer);
  strftime(timebuffer, 26, "%Y:%m:%d %H:%M:%S",tm_i);
  
  return 0;
}
