#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *
filename(char *path)
{
  char *p = path;
  while (p)
  {
    if (*p == '/')
    {
      ++p;
      path = p;
    }
    else
    {
      ++p;
    }
  }
  return path;
}

void find(char *path, char *name)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0)
  {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0)
  {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if (st.type == T_DIR)
  {
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
      if (de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if (stat(buf, &st) < 0)
      {
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      switch (st.type)
      {
      case T_FILE:
        if (strcmp(de.name, name) == 0)
        {
          printf("%s\n", buf);
        }
        break;
      case T_DIR:
        if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0)
        {
          find(buf, name);
        }
        break;
      }
    }
  }

  close(fd);
}

int main(int argc, char *argv[])
{
  find(argv[1], argv[2]);
  exit(0);
}
