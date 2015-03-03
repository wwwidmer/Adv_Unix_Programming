#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

// Main
// Retrieves command line input, reports errors, calls stat functions, exits.


char* getType(char* arg, struct stat buf);
int getModes(struct stat buf);
int printMeanings();

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
  printf("stats for %20s\n===============\n", argv[1]);
  printf("set-usr-id: ");
  printf( (buf.st_mode & S_ISUID) ? "1\n" : "0\n" );
  printf("set-grp-id: ");
  printf( (buf.st_mode & S_ISGID) ? "1\n" : "0\n" );
  printf("Sticky: ");
  printf( (buf.st_mode & 0b01000) ? "1\n" : "0\n");
  printf("File-type: ");
  printf("%s \n",getType(argv[1],buf));
  printf("Access: ");
  getModes(buf);
  printf("\n");
  printMeanings();
  exit(0);
}

int printMeanings(){/*
  printf("==============\n");
  printf("File types\n");
  printf("==============\n");
  printf("REGULAR FILE: 0001\n");
  printf("DIRECTORY: 0010\n");
  printf("CHR FILE: 0011\n");
  printf("BLK FILE: 0100\n");
  printf("SYMBOLIC LINK: 0101\n");
  printf("ERR: 0110 \n");
  printf("==============\n");
  printf("Permissions (in bit order)\n");
  printf("==============\n");
  printf("User can read \n");
  printf("User can write \n");
  printf("User can execute \n");
  printf("Group can read \n");
  printf("Group can wrote \n");
  printf("Group can execute \n");
  printf("World can read \n");
  printf("World can write \n");
  printf("World can execute \n");
*/
  printf("==============\n");
}

int getModes(struct stat buf){
  printf( (buf.st_mode & S_IRUSR) ? "1" : "0");
  printf( (buf.st_mode & S_IWUSR) ? "1" : "0");
  printf( (buf.st_mode & S_IXUSR) ? "1" : "0");
  printf( (buf.st_mode & S_IRUSR) ? "1" : "0");
  printf( (buf.st_mode & S_IWGRP) ? "1" : "0");
  printf( (buf.st_mode & S_IXGRP) ? "1" : "0");
  printf( (buf.st_mode & S_IROTH) ? "1" : "0");
  printf( (buf.st_mode & S_IWOTH) ? "1" : "0");
  printf( (buf.st_mode & S_IXOTH) ? "1" : "0");
  return 0;
}

char* getType(char* arg, struct stat buf){
  if(lstat(arg, &buf)<0){
    printf("lstat error\n");
    exit(1);
  }
  if(S_ISREG(buf.st_mode)){
    return "0001";
  } else if (S_ISDIR(buf.st_mode)){
    return "0010";
  } else if (S_ISCHR(buf.st_mode)){
    return "0011";
  } else if (S_ISBLK(buf.st_mode)){
    return "0100";
  } else if (S_ISFIFO(buf.st_mode)){
    return "0101";
  } else if (S_ISLNK(buf.st_mode)){
    return "0110";
  } else return "0111";
}
