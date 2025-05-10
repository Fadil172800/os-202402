# 📘 Tutorial Praktikum: Penjadwalan Proses pada xv6 (dengan Docker)

---

## 🧭 **Pendahuluan**

Tutorial ini akan memandu Anda untuk memodifikasi algoritma penjadwalan pada sistem operasi xv6 menggunakan Docker. Anda akan belajar menambahkan fitur penjadwalan berbasis prioritas, membuat syscall baru, dan menjalankan sistem menggunakan container Docker.

---

## 📁 **1. Struktur Folder Proyek**

Struktur berikut disarankan untuk mempermudah pengelolaan file:

```
/os-scheduler
│
├── Dockerfile                # Dockerfile untuk build environment
├── src                       # Sumber kode untuk sistem operasi atau implementasi penjadwalan
│   ├── proc.h                # Header file untuk mendefinisikan proses
│   ├── proc.c                # Implementasi logika penjadwalan dan proses
│   ├── scheduler.c           # Implementasi algoritma penjadwalan
│   ├── setprio.c             # Program user untuk mengatur prioritas
│   ├── main.c                # Entry point aplikasi (jika bukan xv6)
│   └── Makefile              # File untuk membangun aplikasi
│
├── scripts                   
│   └── run.sh                # Skrip untuk menjalankan sistem
│
├── tests                     # (Opsional) Pengujian otomatis
│   └── run_tests.sh          
│
├── Dockerfile                # Dockerfile untuk membuat container image
└── README.md                 # Dokumentasi proyek
```

---

## 🐳 **2. Dockerfile untuk Environment xv6**

Berikut adalah contoh `Dockerfile` untuk membangun lingkungan pengembangan xv6:

```dockerfile
# filepath: Dockerfile
FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    make \
    git \
    qemu \
    gdb \
    curl \
    vim \
    && apt-get clean

WORKDIR /os-scheduler

COPY . /os-scheduler

RUN make -C src

CMD ["./scripts/run.sh"]
```

---

## 🛠️ **3. Implementasi: Modifikasi xv6 Scheduler**

### A. Tambahkan `priority` ke Struktur Proses

Tambahkan atribut `priority` ke struktur proses di `proc.h`:

```c
// filepath: src/proc.h
struct proc {
  ...
  int priority; // nilai prioritas, makin kecil makin penting
  ...
};
```

### B. Modifikasi Fungsi `scheduler()`

Modifikasi fungsi `scheduler()` agar memilih proses dengan prioritas tertinggi:

```c
// filepath: src/proc.c
void
scheduler(void)
{
  struct proc *p;
  struct proc *highp;

  for(;;){
    sti();
    acquire(&ptable.lock);
    highp = 0;

    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state != RUNNABLE) continue;
      if(highp == 0 || p->priority < highp->priority)
        highp = p;
    }

    if(highp){
      p = highp;
      proc = p;
      switchuvm(p);
      p->state = RUNNING;
      swtch(&cpu->scheduler, proc->context);
      switchkvm();
      proc = 0;
    }
    release(&ptable.lock);
  }
}
```

---

## 🔧 **4. Tambahkan syscall `setpriority()`**

### A. Tambahkan Deklarasi Syscall

Tambahkan deklarasi syscall di beberapa file berikut:

```c
// filepath: user/user.h
int setpriority(int pid, int priority);
```

```assembly
// filepath: user/usys.S
SYSCALL(setpriority)
```

```c
// filepath: kernel/syscall.h
#define SYS_setpriority 24
```

```c
// filepath: kernel/syscall.c
extern int sys_setpriority(void);
[SYS_setpriority] sys_setpriority,
```

### B. Implementasi Syscall

Implementasikan syscall di `sysproc.c`:

```c
// filepath: kernel/sysproc.c
int
sys_setpriority(void)
{
  int pid, newprio;
  if(argint(0, &pid) < 0 || argint(1, &newprio) < 0)
    return -1;

  struct proc *p;
  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->pid == pid){
      p->priority = newprio;
      break;
    }
  }
  release(&ptable.lock);
  return 0;
}
```

---

## 👤 **5. Program User: `setprio.c`**

Buat program user untuk mengatur prioritas proses:

```c
// filepath: src/setprio.c
#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
  if(argc != 3){
    printf(2, "Usage: setprio pid priority\n");
    exit();
  }
  int pid = atoi(argv[1]);
  int priority = atoi(argv[2]);
  if(setpriority(pid, priority) < 0){
    printf(2, "Failed to set priority\n");
  }
  exit();
}
```

Tambahkan ke `Makefile`:

```make
# filepath: src/Makefile
UPROGS=\
  _setprio\
```

---

## 📈 **6. Visualisasi Gantt Chart**

Tambahkan fungsi untuk menampilkan Gantt Chart:

```c
// filepath: src/proc.c
void display_gantt_chart() {
    struct proc *p;
    for(p = proc; p < &proc[NPROC]; p++) {
        if(p->state == TERMINATED) {
            printf("P%d: [%d-%d] ", p->pid, p->start_time, p->completion_time);
        }
    }
    printf("\n");
}
```

---

## 🏃 **7. Skrip Eksekusi: `run.sh`**

Buat skrip untuk menjalankan sistem:

```bash
#!/bin/bash
echo "Compiling and Running Scheduler..."
cd src
make
./scheduler
```

Jangan lupa beri izin eksekusi:

```bash
chmod +x scripts/run.sh
```

---

## 🔬 **8. Testing dan Debugging**

Tambahkan pengujian otomatis di folder `tests/`:

```bash
#!/bin/bash
# filepath: tests/run_tests.sh

echo "Test: Priority Scheduling"
./setprio 3 1
./setprio 4 5
# tambahkan test lain
```

Jalankan dengan:

```bash
docker run --rm os-scheduler ./tests/run_tests.sh
```

---

## 🧪 **9. Menjalankan di Docker**

### A. Build Docker image:

```bash
docker build -t os-scheduler .
```

### B. Jalankan program:

```bash
docker run --rm os-scheduler
```

### C. Interaktif debugging:

```bash
docker run -it os-scheduler /bin/bash
```

---

## ✅ **10. Laporan Praktikum**

Buat laporan berisi:

1. Deskripsi perubahan pada `scheduler()`.
2. Cuplikan kode hasil modifikasi.
3. Output uji coba (tangkapan layar).
4. Penjelasan efek perubahan terhadap eksekusi proses.
5. Gantt Chart hasil uji coba.

---

## 📚 **11. Referensi**

* [MIT xv6 Book](https://pdos.csail.mit.edu/6.828/2023/xv6/book-rev11.pdf)
* [xv6 GitHub](https://github.com/mit-pdos/xv6-public)
* [Docker Documentation](https://docs.docker.com/)

---
