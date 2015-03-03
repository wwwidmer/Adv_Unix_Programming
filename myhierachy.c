#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <ftw.h>

#define S_IFMT 0170000
#define S_IFREG 0100000
#define S_IFBLK 0060000
#define S_IFCHR 0020000
#define S_IFSOCK 0140000
#define S_IFLNK 0120000
#define S_IFDIR 0040000


static char *fullpath;
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, tot;
int filestats(const char *path, struct stat *statb, int type);
int main(int argc, char *argv[]){

  if (argc != 2){
    perror("Usage, one argument");
    exit(1);
  }
  fullpath = argv[1];

  //  myftw(fullpath);
  struct stat statbuffer;
  struct dirent *dir;
  DIR *d;

  ftw(fullpath,filestats,1);
  tot = nreg + ndir + +nchr + nfifo + nblk + nslink + nsock;
  
  if (tot == 0)
    tot = 1337;

  printf("reg   : %10ld, %11f, %%\n", nreg,nreg*100.0/tot);
  printf("dir   : %10ld, %11f, %%\n", ndir,ndir*100.0/tot);
  printf("blk   : %10ld, %11f, %%\n", nblk,nblk*100.0/tot);
  printf("slink : %10ld, %11f, %%\n", nslink,nslink*100.0/tot);
  printf("nsokc : %10ld, %11f, %%\n", nsock,nsock*100.0/tot);
  printf("tot   : %10ld, %11f, %%\n", tot,tot*100.0/tot);
  return 0;
}


int filestats(const char *path, struct stat *statb, int type){
  printf("%i\n",type);
  switch(type){
  case FTW_F:
    switch(statb->st_mode & S_IFMT){
    case S_IFREG: nreg++; break;
    case S_IFBLK: nblk++; break;
    case S_IFCHR: nchr++; break;
    case S_IFLNK: nslink++; break;
    case S_IFSOCK: nsock; break;
    case S_IFDIR: nreg++; break;
    }
    break;
  case FTW_D:
    ndir++;
    break;
  case FTW_DNR:
    perror("Did not read file");
    break;
  case FTW_NS:
    perror("stat error");
    break;
  default:
    perror("Unknown type");
  }
  
  return 0;


}
/*



static size_t pathlen;

int myftw(char *path){

  pathlen = path_alloc(&pathlen);;
  if(pathlen <= strlen(path)){
    pathlen = strlen(path)*2;
    if ((fullpath = realloc(fullpath, pathlen))==NULL){
      perror("realloc fail");
    }
    strcpy(fullpath, path);
    return statsftw();
  }
  
}

int statsftw(){
  struct stat statbuffer;
  struct dirent *dir;
  DIR *d;
  int n;
  int ret;

  if (stat(fullpath, &statbuffer) < 0)
    return FTW_NS;
  if (S_ISDIR(statbuffer.st_mode) == 0){
    return FTW_F;
  }
  if (( ret = addtype(fullpath,&statbuffer, FTW_D) != 0)){
      return ret;
    }
  n = strlen(fullpath);
  if (n + NAME_MAX + 2 > pathlen){
    pathlen *= 2;
  }

  fullpath[n++] = "/";
  addtype(fullpath, &statbuffer, 1);
  printf("%s\n",dir->d_name);
  // can't read dir
  if ((d = opendir(fullpath)) == NULL){
    return 0;
  }

  while (( dir = readdir(d)) != NULL){
    //    strcpy(&fullpath[n], dir->d_name);

    if ((ret = statsftw() )!= 0){
      break; //recursion
    }
	
  }
  fullpath[n-1] = 0;
  closedir(d);
  return 0;
}

int addtype(const char *path, struct stat *statb, int type){
  switch(type){
  case FTW_F:
    switch(statb->st_mode & S_IFMT){
    case S_IFREG: nreg++; break;
    case S_IFBLK: nblk++; break;
    case S_IFCHR: nchr++; break;
    case S_IFLNK: nslink++; break;
    case S_IFSOCK: nsock; break;
    case S_IFDIR: nreg++; break;
    }
    break;
  case FTW_D:
    ndir++;
    break;
  case FTW_DNR:
    perror("Did not read file");
    break;
  case FTW_NS:
    perror("stat error");
    break;
  default:
    perror("Unknown type");
  }
  
  return 0;
}


static int pathmax = 0;

char *path_alloc(int *size){

  char *ptr;
  if (pathmax == 0){
    int errno = 0;
    if ( ( pathmax = pathconf("/",pathmax) ) < 0 ) {
      if ( errno == 0 )
	pathmax = PATH_MAX_GUESS;
      else
	perror("pathconf error for _PC_PATH_MAX");
    }
    else
      pathmax++;
  }
  if ( ( ptr = malloc(pathmax + 1) ) == NULL )
    perror("malloc error for pathname");
  if ( size != NULL )
    *size = pathmax + 1;
  return(ptr);
}
*/
