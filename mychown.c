#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>


/* MYCHOWN
   WILLIAM WIDMER UNIX TOOLS
   FEB 17 2015
   
   C program demonstrating chown command.
   
   This program accepts one command line argument. A file. 
   The file has its ownership changed to the root user (0).
   cc -o mychown mychown.c 

 */

int main(int argc, char *argv[]){

  if (argc !=2){
    printf("Improper use\n");
    exit(1);
  }
  int userid = getuid();
  int groupid = getgid();
  //  chown(argv[1], userid, groupid);
  chown(argv[1], 0,0);
  exit(0);
  return 0;
}
