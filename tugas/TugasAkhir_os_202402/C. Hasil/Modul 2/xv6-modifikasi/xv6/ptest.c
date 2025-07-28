#include "types.h"
#include "stat.h"
#include "user.h"

void busy() {
  for (volatile int i = 0; i < 100000000; i++);
}

int main() {
  int pid2 = fork();  // Buat Child 2 lebih dulu → prioritas tinggi
  if (pid2 == 0) {
    set_priority(10); // prioritas tinggi
    busy();
    printf(1, "Child 2 selesai\n");
    exit();
  }

  // Tunggu sebentar agar child 2 bisa jalan lebih dulu
  sleep(1); // ⬅️ Tambahan penting

  int pid1 = fork();  // Buat Child 1 belakangan → prioritas rendah
  if (pid1 == 0) {
    set_priority(90); // prioritas rendah
    busy();
    printf(1, "Child 1 selesai\n");
    exit();
  }

  wait(); wait();
  printf(1, "Parent selesai\n");
  exit();
}

