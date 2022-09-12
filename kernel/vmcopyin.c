#include "param.h"
#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "spinlock.h"
#include "proc.h"

//
// This file contains copyin_new() and copyinstr_new(), the
// replacements for copyin and coyinstr in vm.c.
//

static struct stats {
  int ncopyin;
  int ncopyinstr;
} stats;

int
statscopyin(char *buf, int sz) {
  int n;
  n = snprintf(buf, sz, "copyin: %d\n", stats.ncopyin);
  n += snprintf(buf+n, sz, "copyinstr: %d\n", stats.ncopyinstr);
  return n;
}

// Copy from user to kernel.
// Copy len bytes to dst from virtual address srcva in a given page table.
// Return 0 on success, -1 on error.
int
copyin_new(pagetable_t pagetable, char *dst, uint64 srcva, uint64 len)
{
  struct proc *p = myproc();
  // printf("----------------------------------------\n.");
  // vmprint(p->k_pgtbl);
  // printf("----------------------------------------\n.");
  // vmprint(p->pagetable);
  // printf("----------------------------------------\n.");
  //printf("p's pgtbl:%d\n.", p->pagetable);
  // pte_t *pte1 = walk(p->k_pgtbl, srcva, 0);
  // uint64 pa1 = PTE2PA(*pte1);
  // printf("pa1%p\n.", pa1);
  // if((*pte1 & PTE_V) == 0) {
  //   printf("INVALID PTE1\n.");
  // } else {
  //   printf("VALID PTE1\n.");
  // }
  // printf("walk1 done%p\n.", pte1);


  // pte_t *pte2 = walk(p->pagetable, srcva, 0);
  // uint64 pa2 = PTE2PA(*pte2);
  // printf("pa2%p\n.", pa2);
  // if((*pte2 & PTE_V) == 0) {
  //   printf("INVALID PTE2\n.");
  // } else {
  //   printf("VALID PTE2\n.");
  // }
  // printf("walk2 done%p\n.", pte2);

  if (srcva >= p->sz || srcva+len >= p->sz || srcva+len < srcva)
    return -1;
  memmove((void *) dst, (void *)srcva, len);
 // printf("memmove done\n.");
  stats.ncopyin++;   // XXX lock
  return 0;
}

// Copy a null-terminated string from user to kernel.
// Copy bytes to dst from virtual address srcva in a given page table,
// until a '\0', or max.
// Return 0 on success, -1 on error.
int
copyinstr_new(pagetable_t pagetable, char *dst, uint64 srcva, uint64 max)
{
  struct proc *p = myproc();
  char *s = (char *) srcva;
  
  stats.ncopyinstr++;   // XXX lock
  for(int i = 0; i < max && srcva + i < p->sz; i++){
    dst[i] = s[i];
    if(s[i] == '\0')
      return 0;
  }
  return -1;
}
