#include "kernel/types.h"
#include "user/user.h"

typedef unsigned char byte;

int main(int argc, char *argv[])
{
  if (argc != 1)
  {
    fprintf(2, "Do not accept any arguments");
    exit(1);
  }

  byte buf;

  int parent_to_child[2];
  int child_to_parent[2];

  pipe(parent_to_child);
  pipe(child_to_parent);

  if (fork() == 0)
  {
    read(parent_to_child[0], &buf, 1);
    fprintf(1, "%d: received ping\n", getpid());
    write(child_to_parent[1], &buf, 1);
    close(parent_to_child[0]);
    close(parent_to_child[1]);
    close(child_to_parent[0]);
    close(child_to_parent[1]);
    exit(0);
  }

  write(parent_to_child[1], &buf, 1);
  read(child_to_parent[0], &buf, 1);
  fprintf(1, "%d: received pong\n", getpid());
  close(parent_to_child[0]);
  close(parent_to_child[1]);
  close(child_to_parent[0]);
  close(child_to_parent[1]);

  exit(0);
}
