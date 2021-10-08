#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAXLENGTH 1024

int main(int argc, char *argv[])
{
  char *args[MAXARG];
  char buf[MAXLENGTH];

  for (int i = 1; i < argc; ++i)
  {
    args[i - 1] = argv[i];
  }

  int arg = argc - 1;

  char *start = buf;
  char *current = buf;

  while (current < buf + MAXLENGTH && read(0, current, sizeof(char)))
  {
    switch (*current)
    {
    case ' ':
      if (start != current)
      {
        *current = 0;
        args[arg] = start;
        ++arg;
        ++current;
        start = current;
      }
      break;
    default:
      ++current;
      break;
    case '\n':
      if (start != current)
      {
        *current = 0;
        args[arg] = start;
        ++arg;
        ++current;
        start = current;
      }
      args[arg] = 0;
      if (fork() == 0)
      {
        exec(argv[1], args);
      }
      arg = argc - 1;
      start = buf;
      current = buf;
      wait(0);
      break;
    }
  }

  exit(0);
}
