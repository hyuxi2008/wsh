/*******************************************************************************
 * Embedded Shell Functionality
 * ============================
 *
 * Description
 * -----------
 * Minimalistic shell functionality for embedded processors.
 * Created by Daniel Wagner, therefore named wagi shell (wsh).
 *
 * Notes
 * -----
 * Employs VT100 Terminal Control Sequences and VT100 Cursor Control Sequences.
 * 
 * History
 * -------
 * 2007/09/05 DW created
 * 2010/10/15 PL migrated from PPC405 to PPC440
 *******************************************************************************/
#ifndef WSH_CMDS_H  /* prevent circular inclusions */
#define WSH_CMDS_H  /* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

#include "wsh/wsh.h"          // support for embedded shell

// -----------------------------------------------------------------------------
// General Functions
// -----------------------------------------------------------------------------

// all registered for wsh usage in project-specific file wsh_cmds.c
unsigned long wsh_extract_val (const char *nptr);
int wsh_cmd_rd (int argc, char** argv);
int wsh_cmd_wr (int argc, char** argv);
int wsh_cmd_history (int argc, char** argv);
int wsh_cmd_clear (int argc, char** argv);
int wsh_cmd_quit (int argc, char** argv);
int wsh_cmd_help (int argc, char** argv);


#ifdef __cplusplus
}
#endif 

#endif //WSH_CMDS_H
