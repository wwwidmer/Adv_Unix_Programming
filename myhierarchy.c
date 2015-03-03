#include "myhierarchy.h"
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <ftw.h>
#include <limits.h>



/* my file hierarchy
 * usage: < file directory >
 * recursively runs through files and directories to determine % stats
 */

typedef int Myfunc(const char *, const struct stat *, int);

static char *fullpath;
static size_t pathlen;
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, tot;
//static Myfunc  myfunc;

static int myftw(char *pathname, Myfunc *func);
static int dopath(Myfunc *);

static int filestats(const char *path, const struct stat *statb, int type);



int main(int argc, char *argv[]){

  if (argc != 2){
    perror("Usage, one argument");
    exit(1);
  }
  // In case we need the path somewhere else...
  fullpath = argv[1];

  // Call the file tree walk
  ftw(fullpath,filestats,1);
  tot = nreg + ndir +nchr + nfifo + nblk + nslink + nsock;

  // NO divide by 0
  if (tot == 0)
    tot = 1;

  // Using built-in FTW
  printf("Stats using built-in ftw():\n");
  printf("reg   : %10ld, %11f, %%\n", nreg,nreg*100.0/tot);
  printf("dir   : %10ld, %11f, %%\n", ndir,ndir*100.0/tot);
  printf("blk   : %10ld, %11f, %%\n", nblk,nblk*100.0/tot);
  printf("slink : %10ld, %11f, %%\n", nslink,nslink*100.0/tot);
  printf("nsokc : %10ld, %11f, %%\n", nsock,nsock*100.0/tot);
  printf("tot   : %10ld, %11f, %%\n", tot,tot*100.0/tot);


  nreg = 0, ndir = 0, nchr = 0, nfifo = 0, nblk = 0, nslink = 0, nsock = 0;
  myftw(fullpath,filestats);
  
  tot = nreg + ndir +nchr + nfifo + nblk + nslink + nsock;
  printf("Stats using myftw():\n");
  printf("reg   : %10ld, %11f, %%\n", nreg,nreg*100.0/tot);
  printf("dir   : %10ld, %11f, %%\n", ndir,ndir*100.0/tot);
  printf("blk   : %10ld, %11f, %%\n", nblk,nblk*100.0/tot);
  printf("slink : %10ld, %11f, %%\n", nslink,nslink*100.0/tot);
  printf("nsokc : %10ld, %11f, %%\n", nsock,nsock*100.0/tot);
  printf("tot   : %10ld, %11f, %%\n", tot,tot*100.0/tot);

  return 0;
}


static int myftw(char *pathname, Myfunc *func)
{
  fullpath = path_alloc(&pathlen);        /* malloc PATH_MAX+1 bytes */
  if (pathlen <= strlen(pathname)) {
    pathlen = strlen(pathname) * 2;
    if ((fullpath = realloc(fullpath, pathlen)) == NULL)
      perror("realloc failed");
  }
  strcpy(fullpath, pathname);
  return(dopath(func));
}


static int dopath(Myfunc* func)
{
  struct stat statbuf;
  struct dirent *dirp;
  DIR *dp;
  int ret, n;
  
  if (lstat(fullpath, &statbuf) < 0)      /* stat error */
    return(func(fullpath, &statbuf, FTW_NS));
  if (S_ISDIR(statbuf.st_mode) == 0)      /* not a directory */
    return(func(fullpath, &statbuf, FTW_F));

  if ((ret = func(fullpath, &statbuf, FTW_D)) != 0)
    return(ret);
  
  n = strlen(fullpath);
  if (n + NAME_MAX + 2 > pathlen) {       /* expand path buffer */
    pathlen *= 2;
    if ((fullpath = realloc(fullpath, pathlen)) == NULL)
      perror("realloc failed");
  }
  fullpath[n++] = '/';
  fullpath[n] = 0;
  
  if ((dp = opendir(fullpath)) == NULL)   /* can't read directory */
    return(func(fullpath, &statbuf, FTW_DNR));
  
  while ((dirp = readdir(dp)) != NULL) {
    if (strcmp(dirp->d_name, ".") == 0  ||
	strcmp(dirp->d_name, "..") == 0)
      continue;               /* ignore dot and dot-dot */
    strcpy(&fullpath[n], dirp->d_name);     /* append name after "/" */
    if ((ret = dopath(func)) != 0)          /* recursive */
      break;  /* time to leave */
  }
  fullpath[n-1] = 0;      /* erase everything from slash onward */
 
  if (closedir(dp) < 0)
    perror("can't close directory %s");
  return(ret);
}


// FTW calls filestats which determines the file type
// Then adds to the running total of that type.
// If the file type is unrecognized we error.
static int filestats(const char *path, const struct stat *statb, int type){
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
