#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
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
  return wait(p);
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

uint64
sys_trace(void){
    int mask;

    // get the argument from the user.
    argint(0, &mask);

    if(mask < 0){
        return -1;
    }

    // convert int mask to uint32 bitmask.
    uint32 bitmask = (uint32) mask;

    // 4294967295 is the max value of uint32.
    myproc()->tracemask = bitmask;
    return 0;

}

extern uint64 kfree_bytes(void);
extern uint64 get_unused_proc(void);

uint64
sys_info(void){
    struct sysinfo info;

    // get all needed infomation in kernel mode.
    info.freemem = kfree_bytes();
    info.nproc = get_unused_proc();

    // get the virtual address from the user.
    uint64 destaddr;
    argaddr(0, &destaddr);

    // copy the information from kernel to user.
    if(copyout(myproc()->pagetable, destaddr, (char *)&info, sizeof(info)) < 0){
        return -1;
    }

    return 0;
}
