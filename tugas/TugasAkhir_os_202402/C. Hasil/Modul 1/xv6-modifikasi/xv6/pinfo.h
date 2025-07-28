#ifndef _PINFO_H_
#define _PINFO_H_

#include "param.h"

struct pinfo {
  int inuse[NPROC];          // apakah entri digunakan
  int pid[NPROC];            // ID proses
  int sz[NPROC];             // ukuran memori proses
  char name[NPROC][16];      // nama proses
};

#endif

