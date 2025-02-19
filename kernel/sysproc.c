#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  //task3
  uint64 pointer;
  char msg [32];
  argaddr(1, &pointer);
  if(pointer == 0){
    char *no_msg = "No exit message";
    strncpy(msg, no_msg, strlen(no_msg));
  }
  else{
    fetchstr(pointer, msg, 32);
    }
  exit(n, msg);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

//task2
uint64
sys_memsize(void)
{
  return myproc()->sz;
}

//task5
//task6
uint64
sys_set_affinity_mask(void){
  int mask;
  argint(0, &mask);
  myproc()->affinity_mask = mask;
  myproc()->effective_affinity_mask = mask;
  return 0;
} 

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  //task3
  uint64 msg;
  argaddr(1, &msg);
  return wait(p, msg);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
