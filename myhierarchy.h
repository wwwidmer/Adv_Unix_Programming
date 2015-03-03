#include <errno.h>
#include <limits.h>
#include <linux/limits.h>
#include <stddef.h>
#include <sys/types.h>          /* some systems still require this */
#include <sys/stat.h>
#include <sys/termios.h>        /* for winsize */
#include <sys/ioctl.h>
#include <stdio.h>              /* for convenience */
#include <stdlib.h>             /* for convenience */
#include <stddef.h>             /* for offsetof */
#include <string.h>             /* for convenience */
#include <unistd.h>             /* for convenience */
#include <signal.h>   

static long pathmax = 0;
static long posix_version = 0;
static long xsi_version = 0;

#define PATH_MAX_GUESS 1024
#define S_IFMT 0170000
#define S_IFREG 0100000
#define S_IFBLK 0060000
#define S_IFCHR 0020000
#define S_IFSOCK 0140000
#define S_IFLNK 0120000
#define S_IFDIR 0040000
#ifdef NAME_MAX

#else
#define NAME_MAX 1
#endif

static void
pr_sysconf(char *mesg, int name)
{
  long    val;

  fputs(mesg, stdout);
  errno = 0;
  if ((val = sysconf(name)) < 0) {
    if (errno != 0) {
      if (errno == EINVAL)
	fputs(" (not supported)\n", stdout);
      else
	perror("sysconf error");
    } else {
      fputs(" (no limit)\n", stdout);
    }
  } else {
    printf(" %ld\n", val);
  }
}




char * path_alloc (size_t *sizep){
  char *ptr;
  size_t size;

  if(posix_version == 0)
    posix_version = sysconf(_SC_VERSION);
  if (xsi_version == 0)
    xsi_version = sysconf(_SC_XOPEN_VERSION);

  if (pathmax ==0){
    errno = 0;
    if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0 ){
      if (errno == 0)
	pathmax = PATH_MAX_GUESS;
      else
	perror("pathconf");

    } else{
      pathmax++;
    }
  }


  if((posix_version < 20112L) && (xsi_version < 4))
    size = pathmax + 1;
  else
    size = pathmax;

  if ((ptr = malloc(size)) == NULL)
    perror("malloc error");
  if (sizep != NULL)
    *sizep = size;
  return(ptr);
  
}




