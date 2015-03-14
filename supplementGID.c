#include <sys/types.h>
#include <sys/param.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <grp.h>

int listGroupName(gid_t gid);

int main (){
  int num_groups;
  gid_t grouplist[NGROUPS_MAX];
  gid_t *gi;

  num_groups = getgroups(NGROUPS_MAX, grouplist);
  printf("There are %d supplementary groups\n", num_groups);

  int i = 0;
  for (i;i < num_groups;i++){
    printf("group %d ", grouplist[i]);
    listGroupName(grouplist[i]);
  }
  return 0;
}


int listGroupName(gid_t gid){
  struct group *ptrgid;
  ptrgid=getgrgid(gid);
  printf("%s \n",ptrgid->gr_name);
  
  return 0;
}
