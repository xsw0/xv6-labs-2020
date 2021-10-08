#include "kernel/types.h"
#include "user/user.h"

typedef unsigned char byte;

int main(int argc, char *argv[])
{
  int pl[2];
  int pr[2];
  pipe(pl);
  for (int i = 2; i <= 35; ++i)
  {
    write(pl[1], &i, sizeof(int));
  }
  close(pl[1]);

  int prime = 0;
  int buf;

  while (read(pl[0], &buf, sizeof buf) != 0)
  {
    if (prime == 0)
    {
      prime = buf;
      fprintf(1, "prime %d\n", prime);
      pipe(pr);
      if (fork() == 0)
      {
        prime = 0;
        close(pl[0]);
        close(pr[1]);
        pl[0] = pr[0];
      }
      else
      {
        close(pr[0]);
      }
    }
    else
    {
      if (buf % prime != 0)
      {
        write(pr[1], &buf, sizeof buf);
      }
    }
  }

  close(pl[0]);
  close(pr[1]);

  wait(0);
  exit(0);
}
