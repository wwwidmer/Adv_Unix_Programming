#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
//#include "apue.h"

/* write a program that displays all information from the stat structure 
about a file in a formatted table. constant names on left values on right.
st_mode of the stat structure which encodes file type and permissions should be broken down into the values of its components.list the meanings of each bit of st_mode and its values for your file. convert into binary using % on an octal value of st_mode then test against the values obtained by selecting each bit using bitwise operations.

16 bits for st_mode
1 - set user id
1 - set group id
1 - stick bit
4 - file type
9 - access permissions

due 2/24


*/


// Main
// Retrieves command line input, reports errors, calls stat functions, exits.
int main(int argc, char *argv[]){
  struct stat buf;
  if (argc != 2 ){
    printf("Improper use, please use a file path argument");
    exit(1);
  }

  if(stat(argv[1],&buf) < 0){
    printf("Stat error");
    exit(1);
  }
  printf("stats for %s\n-------------\n", argv[1]);
  printf("set-usr-id: ");
  printf( (buf.st_mode & S_ISUID) ? "1" : "0" );
  printf("\n");
  printf("set-grp-id: ");
  printf( (buf.st_mode & S_ISGID) ? "1" : "0" );
  printf("\n");
  printf("Sticky: ");
  //  S_ISVTX;
  printf("\n");
  printf("File-type: ");
  // s_isreg
  printf("\n");
  printf("Access: ");
  printf( (buf.st_mode & S_IRUSR) ? "1" : "0");
  printf( (buf.st_mode & S_IWUSR) ? "1" : "0");
  printf( (buf.st_mode & S_IXUSR) ? "1" : "0");
  printf( (buf.st_mode & S_IRGRP) ? "1" : "0");
  printf( (buf.st_mode & S_IWGRP) ? "1" : "0");
  printf( (buf.st_mode & S_IXGRP) ? "1" : "0");
  printf( (buf.st_mode & S_IROTH) ? "1" : "0");
  printf( (buf.st_mode & S_IWOTH) ? "1" : "0");
  printf( (buf.st_mode & S_IXOTH) ? "0" : "0");
  printf("\n");
  
  exit(0);
}




