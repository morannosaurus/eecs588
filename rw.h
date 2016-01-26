#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/unistd.h>
#include <asm/fcntl.h>
#include <asm/errno.h>
#include <linux/types.h>
#include <linux/dirent.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/highmem.h>
#include <asm/unistd.h>

int make_rw(unsigned long address)
{
   unsigned int level;
   pte_t *pte = lookup_address(address, &level);
   if(pte->pte &~ _PAGE_RW)
      pte->pte |= _PAGE_RW;
   return 0;
}

/* Make the page write protected */
int make_ro(unsigned long address)
{
   unsigned int level;
   pte_t *pte = lookup_address(address, &level);
   pte->pte = pte->pte &~ _PAGE_RW;
   return 0;
}
