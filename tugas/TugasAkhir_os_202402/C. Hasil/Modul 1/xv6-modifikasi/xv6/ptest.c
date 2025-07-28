#include "types.h"
#include "stat.h"
#include "user.h"
#include "pinfo.h"    // <- Tambahkan ini

int
main(int argc, char *argv[])
{
  struct pinfo info;

  if (getpinfo(&info) < 0) {
    printf(1, "getpinfo gagal\n");
    exit();
  }

  printf(1, "PID\tMEM\tNAME\n");
  for (int i = 0; i < 64; i++) {
    if (info.inuse[i]) {
      printf(1, "%d\t%d\t%s\n", info.pid[i], info.sz[i], info.name[i]);
    }
  }

  exit();
}

