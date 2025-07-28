#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "pinfo.h"

extern struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

int readcount = 0;  // Counter global untuk jumlah syscall read()

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_getreadcount(void) {
  return readcount;
}

int
sys_getpinfo(void)
{
  struct pinfo *info;
  struct proc *p;
  int i = 0;

  if (argptr(0, (void*)&info, sizeof(*info)) < 0)
    return -1;

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state != UNUSED){
      info->inuse[i] = 1;
      info->pid[i] = p->pid;
      info->sz[i] = p->sz;
      info->name[i][0] = '\0';
      safestrcpy(info->name[i], p->name, sizeof(info->name[i]));
    } else {
      info->inuse[i] = 0;
    }
    i++;
  }
  release(&ptable.lock);
  return 0;
}

int
sys_set_priority(void) {
  int prio;
  if (argint(0, &prio) < 0 || prio < 0 || prio > 100)
    return -1;

  myproc()->priority = prio;
  yield();  // supaya proses menyerahkan CPU jika prioritas lebih rendah
  return 0;
}

