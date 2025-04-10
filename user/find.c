#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void find(char *path, char *name) 
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;


  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if(st.type != T_DIR){
    fprintf(2, "find: %s is not a directory\n", path);
    close(fd);
    return;
  }

  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
    printf("ls: path too long\n");
    return;
  }
  strcpy(buf, path);
  p = buf+strlen(buf);
  *p++ = '/';
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
      continue;

    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = '0';
    if(stat(buf, &st) < 0){
      fprintf(2, "ls: cannot stat %s\n", buf);
      continue;
    }
    if(st.type == T_DIR){
      if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        continue;
      find(buf, name);
    } else {
      // fprintf(1, "---------%s----------%s---------\n", buf, fmtname(name));
      if(strcmp(de.name, name) == 0){
        fprintf(1, "%s\n", buf);
      }
    }
  }
  close(fd);
}

int main(int argc, char *argv[])
{

  if(argc != 3){
    fprintf(2, "Usage: find <path> <name>\n");
    exit(1);
  }
  char *path = argv[1];
  char *name = argv[2];
  find(path, name);
  exit(0);
}