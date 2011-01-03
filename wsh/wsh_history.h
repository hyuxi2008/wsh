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
#ifndef WSH_HISTORY_H  /* prevent circular inclusions */
#define WSH_HISTORY_H  /* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

#include "wsh/wsh.h"          // support for embedded shell

typedef struct _hist_entry {
  char line[LINE_SIZE+1];
  int end;
} history_entry_t;

typedef enum _hist_mode {
  history_mode_normal,  // no history element is displayed 
  history_mode_browse,  // get_next & get_prev 
} history_mode_t;

typedef struct _hist_state {
  history_entry_t entries[HISTORY_ENTRIES];
  history_mode_t mode;
  int offset;
  int size;
  int base;
} history_state_t;

// -----------------------------------------------------------------------------
// General Functions
// -----------------------------------------------------------------------------

history_entry_t* history_get_new (history_state_t* st);
history_entry_t* history_get_next (history_state_t* st);
history_entry_t* history_get_prev (history_state_t* st);
history_entry_t* history_get (history_state_t* st, int idx);
void history_print (history_state_t* st);
void history_print_debug (history_state_t* st);


#ifdef __cplusplus
}
#endif 

#endif //WSH_HISTORY_H