#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

/*     MYNAME 
 * (homework-one.c)
 *
 * A small program that creates a file and writes your name into it.
 * argv[1] (input_string) takes your name as input from the command line.
 *
 */

int main(int argc, char *argv[]){
  // Need one command line argument (argv[0] = myname, argv[1] = input_string)
  if(argc != 2){
    printf("Not enough arguments error\n");
  }  
  int fd, length, wr;
  // Integer represents return value of creating the File. Negative return values indicate an error. Argv[1] is file name and 0777 is permission for new file (everyuser can r/w/x)
  fd = creat(argv[1], 0777);
  if( fd < 0  ){
    perror("File not created error");
    exit(0);
  }
    
  length = strlen(argv[1]);
  wr = write(fd,argv[1], length);
  // Write argv[1] to the file. Negative return values indicate an error.
  if( ( wr < 0 ) ){
    perror("File not written error");
  }
  close(wr);
  exit(0);
  
}



// write a two page history of UNIX which should include an enumeration and short description of the current flavor of unix

// a small program that creates a file and writes your name in it taking your name as input from command line.
// it sohuld use creat and write consult section 2 of man pages of yor system
