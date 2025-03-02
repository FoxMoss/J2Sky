#ifndef __FORM_COMMAND_MGR_H__
#define __FORM_COMMAND_MGR_H__

#include "javax/microedition/lcdui.h"
#include <cibyl.h>

/* FormCommandMgr class (this is not in J2ME) */
typedef int NOPH_FormCommandMgr_t;

NOPH_FormCommandMgr_t NOPH_FormCommandMgr_new(NOPH_Form_t form);
void NOPH_FormCommandMgr_addCommand(NOPH_FormCommandMgr_t fc, int name, int callback_name);
void NOPH_FormCommandMgr_Setup(NOPH_FormCommandMgr_t fc);

#endif /* !__FORM_COMMAND_MGR_H__ */
