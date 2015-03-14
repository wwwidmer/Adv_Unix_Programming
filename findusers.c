#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>

/*
 * Write a program that finds all users on a system. You should test
 * for a large number of UID numbers, since no function returns 
 * that information wholesale.
 *
 * Modify the program that returns supplementary GIDs for a user
 * to also list these group names.
 *
 */ 
int printInfo(struct passwd *pwd);

int main(int argc, char *arvg[]){
  struct passwd *pw;
  for(int i = 0; i < 90000; i++){
    pw = getpwuid( i );
    if (pw == NULL){
      continue;
    } else
      printInfo(pw);
  }
  
  
  return 0;
}


int printInfo(struct passwd *pwd){

  printf("---------------------------------------------------------\n");
  printf("| \t User name:  \t | %20s \t | \n", pwd->pw_name);
  printf("| \t User ID:  \t | %20d \t | \n", pwd->pw_uid);
  printf("| \t Group:  \t | %20d \t | \n", pwd->pw_gid);


  return 0;
}

int suppleGIDS(){

  return 0;
}
