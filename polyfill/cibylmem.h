#ifndef __CIBYL_MEM_H__
#define __CIBYL_MEM_H__

#include <cibyl.h>
#include <javax/microedition/io.h>

#define __NR_NOPH_CibylMem_memorySize 3 /* sys */
static inline _syscall0(int,NOPH_CibylMem_memorySize) 
#define __NR_NOPH_CibylMem_printException 4 /* sys */
static inline _syscall1(void,NOPH_CibylMem_printException, NOPH_Exception_t, e) 

#endif
