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
#ifndef WSH_READLINE_H  /* prevent circular inclusions */
#define WSH_READLINE_H  /* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
// General Functions
// -----------------------------------------------------------------------------

int wsh_readline_init();
const char* wsh_readline();


#ifdef __cplusplus
}
#endif 

#endif //WSH_READLINE_H
