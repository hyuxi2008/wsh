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
#ifndef WSH_H  /* prevent circular inclusions */
#define WSH_H  /* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

#define WSH_VERSION "wsh version 0.3\n"

// history size: nr of entries + 1 (for the current command line)
#define HISTORY_ENTRIES 11
#define LINE_SIZE       63
#define PROMPT_STRING   "$> "
#define PROMPT_LENGTH   3

// shell commands
typedef int (*wsh_cmd)(int argc, char** argv);

typedef struct _wsh_cmd_t {
  const char* name;
  wsh_cmd     func;
} wsh_cmd_t;

// -----------------------------------------------------------------------------
// General Functions
// -----------------------------------------------------------------------------

// general wsh start-up function
void wsh (void);


#ifdef __cplusplus
}
#endif 

#endif //WSH_H
