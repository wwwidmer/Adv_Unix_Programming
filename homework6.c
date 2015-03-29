/* Add functionality to this program to print the error messages associated with the numeric termination status values <signal.h>. also add functionality to print the values of the members of the struct rusage. which will get filled in by calling wait3.  Check these entries with man 2.

 */
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <signal.h>

#define BUFFSIZE 4096
#define FILESIZE 1474560

void printRusage(struct rusage rus);
void Input(int);
void PrintStatus(int status);
void signalerrno(int signum);

int main(int argc, char *argv[]){
  int fd=open("/Users/awise/Stevens/Lecture10/file5.txt",
	      O_CREAT|O_RDWR|O_APPEND, S_IRUSR|S_IWUSR); 
  int ret, status,n;
  struct rusage rus;
  for (n=0; n<3; n++) 
    {
      if ( (ret=fork())==0 )
	{ 
	  Input(fd); 
	  printf("Child process, ret=%d.\n", ret); 
	  status=wait(&status); 
	  switch(n) 
	    { 
	    case 0:
	      exit(7); 
	      break; 
	    case 1: 
	      abort(); 
	      break; 
	    case 2: 
	      status=status/0;
	      break; 
	    } 
	} else 
	  { 
	    printf("Parent process, child ret==%d.\n", ret);
	    wait3(&status,&rus);
	    signal(SIGINT,signalerrno);
	    getrusage(RUSAGE_SELF, &rus);
	    printRusage(rus);
	    PrintStatus(status); 
	  }
    }
  
  //  printRusage();
  
  return 0;
}

void signalerrno(int signum){
  printf("Signal Error Numer: %d",signum);
}


void printRusage(struct rusage rus){
  printf("RUSAGE: USER TIME = %d\n",rus.ru_utime);
  printf("RUSAGE: SYS TIME = %d\n",rus.ru_stime);
  printf("RUSAGE: PAGE FAULTS = %d\n",rus.ru_majflt);
  printf("RUSAGE: MAX SET SIZE = %d\n",rus.ru_maxrss);
  printf("RUSAGE: SHARED MEMORY SIZE = %d\n",rus.ru_ixrss);
  printf("RUSAGE: CONTEXT SWITCHS = %d\n",rus.ru_nvcsw);
  printf("RUSAGE: SWAPS = %d\n",rus.ru_nswap);
  printf("RUSAGE: SIGNALS = %d\n",rus.ru_nsignals);
}

void  Input(int filedes) {
    char **name=calloc(3, 20*sizeof(char)); 
    char **it; 
    name[0]="Adriana Wise\n"; 
    name[1]="Richard Stevens\n"; 
    name[2]="Evi Nemeth\n";
    
    for (it=&name[0]; *it =NULL; it++) 
    { 
      ssize_t num_bytes=write(filedes, *it, strlen(*it)); 
      sleep(2); 
    }
}


void PrintStatus(int status) 
{ 
    int lower_8_bits; 
    if ((lower_8_bits=WIFEXITED(status))==true) 
      { 
	printf("WIFEXITED(status)=%d\n",
	       WIFEXITED(status)); 
	printf("Exit status for child=%d\n",
	       WEXITSTATUS(status)); 
	printf("lower_8_bits=%x\n", status); 
	signal(SIGINT,signalerrno);
      } 
    else if (WIFSIGNALED(status)==true) 
      
      {
	signal(SIGINT,signalerrno);
	printf("WIFSIGNALED(status)=%d\n",
	       WIFSIGNALED(status)); 
	printf("Exit status for child=%d\n",
	       WTERMSIG(status)); 
	//	printf("Exit status for child=%d\n",
	//     WCOREDUMP(status)); 
	
      } 
  else if (WIFSTOPPED(status)==true) 
    
    {
      signal(SIGINT,signalerrno);
      printf("WIFSTOPPED(status)=%d\n",
		    WIFSTOPPED(status)); 
      printf("Exit status for child=%d\n",
	     WSTOPSIG(status)); 
    }
}
 
