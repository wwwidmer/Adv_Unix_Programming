/* 2015
 * THREADS.C
 * WILLIAM WIDMER
 * ASSIGNMENT
 * CREATE A PROGRAM TO TAKE INPUT AND RELEASE THAT DATA TO TWO THREADS.
 * HAVE THOSE THREADS WRITE TO THE SAME FILE AND DEMONSTRATE
 * A TYPE OF LOCK SO THE WRITES DO NOT CONFLICT.
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

// Thread implementation variables
pthread_t tid1, tid2;
pthread_mutex_t lock;
static pthread_key_t *key;
// Data used by both threads
static char* booklog = "books.txt";
char* books[25];

void *start_rtn1(void *arg);
void *start_rtn2(void *arg);
void thread_init1(void);
void thread_init2(void);
void destructor(void *k);

/* 
 * Methods to handle input.
 *
 * isEmpty
 * Is the c string a space or null
 *
 * getInput
 * Continue to receive strings from command line until a space.
 * Add to books, an array, at the first non null position.
 */
int isEmpty(const char *line){
  while(*line != '\0'){
    if (isspace(*line) == 0){
      return 0;
    }
    line++;
  }
  return 1;
}

int getInput(){
  char line[25];
  while(( fgets(line, sizeof(line), stdin) != NULL) && (isEmpty(line) == 0 )){
    int k;
    for(k = 0; k < 25; k++){
      if(books[k] == NULL){
	char *cmd = malloc(sizeof(char*));
	books[k] = cmd;
	strcpy(books[k],line);
	break;
      }
    }
  }
  return 0;
}

/*
 * main 
 * Set up threads and initially prompt user for input.
 * Hand out keys, allocate memory, init mutex, and create threads
 */
int main(int argc, char* argv[]){
  int err1, err2;
  key = malloc(sizeof(pthread_key_t));
  printf("Enter records by line\n");
  printf("=====================\n");
  getInput();

  if(pthread_mutex_init(&lock, NULL) != 0){
      printf("Could not create mutex\n");
      exit(1);
  }
  pthread_key_create(key,destructor);
  
  err1 = pthread_create(&tid1, NULL,start_rtn1, NULL);
  if (err1 !=0){
    printf("Could not create thread\n");
    exit(1);
  }
  err2 = pthread_create(&tid2, NULL,start_rtn2, NULL);
  if (err2 !=0){
    printf("Could not create thread\n");
    exit(1);
  }

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  pthread_key_delete(*key);
  pthread_mutex_destroy(&lock);
  return 0;
}
/*
 * Threads
 * Lock the mutex so other thread cannot work in file.
 * Open file, write this threads half of data as an append.
 * Close file, unlock mutex, exit thread.
 */
void *start_rtn1(void *arg){
  pthread_mutex_lock(&lock);

  printf("Thread 1...\n");

  FILE* fd;
  fd = fopen(booklog, "a+");
  int i;
  int j = sizeof(books)/sizeof(books[0]);
  for(i = 0; i < j/2;i++){
    if(books[i] != NULL){
      fprintf(fd,"%s\n",books[i]);
    }
  }
  
  fflush(fd);
  fclose(fd);

  pthread_mutex_unlock(&lock);
}

void *start_rtn2(void *arg){
  pthread_mutex_lock(&lock);

  printf("Thread 2...\n");

  FILE* fd;
  fd = fopen(booklog, "a");
  int i;
  int j = sizeof(books)/sizeof(books[0]);
  for(i = j/2; i < j;i++){
    if(books[i] != NULL){
      char * l = books[i];
      l[strlen(l)-1] = '\0';
      fprintf(fd,"%s",l);
    }
  }
  fflush(fd);
  fclose(fd);

  pthread_mutex_unlock(&lock);
}


// thread lock methods

void destructor(void *k){
  free(k);
}

void thread_init1(void){
  pthread_key_create(key,destructor);
}
void thread_init2(void){
  pthread_key_create(key,destructor);
}
