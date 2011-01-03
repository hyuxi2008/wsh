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
#ifndef WSH_WRAPPER_H  /* prevent circular inclusions */
#define WSH_WRAPPER_H  /* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

#include "wsh/wsh.h"          // support for embedded shell

extern wsh_cmd_t *wsh_cmds;   // implemented in project-specific file wsh_cmds.c

// -----------------------------------------------------------------------------
// Function Prototypes
// -----------------------------------------------------------------------------
// Need to be implemented in wsh_wrapper.c for each project separately to
// ensure portability.

int wsh_getchar (void);
int wsh_putchar (int c);
int wsh_strcmp (const char *s1, const char *s2);
char *wsh_strcpy (char *dest, const char *src);
int wsh_strlen (const char *s);
char *wsh_strtok (char *str, const char *delim);
unsigned long wsh_strtoul (const char *nptr, char **endptr, int base);
int wsh_printf (const char *format, ...);
void wsh_debug_printf (const char* format, ...);
int wsh_cache_data_flush (const void *restrict addr_p, unsigned int length);


#ifdef __cplusplus
}
#endif 

#endif //WSH_WRAPPER_H
