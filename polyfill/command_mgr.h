#ifndef __FORM_COMMAND_MGR_H__
#define __FORM_COMMAND_MGR_H__

#include "javax/microedition/lcdui.h"
#include <cibyl.h>

/* FormCommandMgr class (this is not in J2ME) */
typedef int NOPH_FormCommandMgr_t;

#define __NR_NOPH_FormCommandMgr_new 0 /* sys */
static inline _syscall1(NOPH_FormCommandMgr_t,NOPH_FormCommandMgr_new, NOPH_Form_t, form) 
#define __NR_NOPH_FormCommandMgr_addCommand 1 /* sys */
static inline _syscall3(void,NOPH_FormCommandMgr_addCommand, NOPH_FormCommandMgr_t, fc, int, name, int, callback_name) 
#define __NR_NOPH_FormCommandMgr_Setup 2 /* sys */
static inline _syscall1(void,NOPH_FormCommandMgr_Setup, NOPH_FormCommandMgr_t, fc) 

#endif /* !__FORM_COMMAND_MGR_H__ */
