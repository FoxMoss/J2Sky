#ifndef __CIBYL_MEM_H__
#define __CIBYL_MEM_H__

#include <cibyl.h>
#include <javax/microedition/io.h>
typedef int NOPH_String_t;

int NOPH_CibylMem_memorySize(void);
void NOPH_CibylMem_printException(NOPH_Exception_t e);
NOPH_String_t NOPH_CibylMem_getIso(void);

#endif
