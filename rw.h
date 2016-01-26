#ifndef RW_H
#define RW_H

#include "include.h"

int make_rw(void* address)
{
   unsigned int level;
   pte_t *pte = lookup_address((unsigned long)address, &level);
   if(pte->pte &~ _PAGE_RW)
      pte->pte |= _PAGE_RW;
   return 0;
}

/* Make the page write protected */
int make_ro(void* address)
{
   unsigned int level;
   pte_t *pte = lookup_address((unsigned long)address, &level);
   pte->pte = pte->pte &~ _PAGE_RW;
   return 0;
}
#endif