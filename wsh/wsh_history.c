/*
 *  WSH
 *
 *  Copyright (C) 2007-2011  Daniel Wagner <wagi@monom.org>
 *  Copyright (C) 2007-2011  Peter Luethi <luethi@electronic-engineering.ch>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

/*
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
 */

#include "wsh/wsh_history.h"  // command history
#include "wsh/wsh_wrapper.h"  // function prototypes


static inline int 
history_incr_base (history_state_t* st) 
{ 
  st->base = st->base >= HISTORY_ENTRIES - 1 ? 0 : st->base + 1; 
  return st->base;
}

static inline int
history_index_top (history_state_t* st)
{
  int idx = st->base + (st->size - 1);
  return idx >= HISTORY_ENTRIES ? idx - HISTORY_ENTRIES : idx;
}

history_entry_t*
history_get (history_state_t* st, int idx) 
{
  if (idx >= st->size)
    return 0;

  int elm_idx = history_index_top(st) - idx;
  elm_idx = elm_idx < 0 ? elm_idx + HISTORY_ENTRIES : elm_idx;

  return &st->entries[elm_idx];
}

history_entry_t*
history_get_prev (history_state_t* st)
{
  if (st->offset < st->size - 1)
    st->offset++;

  wsh_debug_printf("history_get_prev: st->size = %d, st->base = %d, st->offset = %d\n", 
		   st->size, st->base, st->offset);

  return history_get(st, st->offset);
}

history_entry_t*
history_get_next (history_state_t* st)
{
  if (st->offset > 0)
    st->offset--;

  wsh_debug_printf("history_get_next: st->size = %d, st->base = %d, st->offset = %d\n", 
		   st->size, st->base, st->offset);

  return history_get(st, st->offset);
}

history_entry_t*
history_get_new (history_state_t* st)
{
  if (st->size < HISTORY_ENTRIES)
    st->size++;
  else
    history_incr_base(st);
  
  // find next slot to store the new entry
  int idx = history_index_top(st);

  return &st->entries[idx];
}

void
history_print (history_state_t* st)
{
  int l = st->size == HISTORY_ENTRIES? 1 : 0;
  int h = st->size == HISTORY_ENTRIES? HISTORY_ENTRIES : st->size;
  for (int i=l; i<h; i++) {
    int idx = st->base + i;
    if (idx >= HISTORY_ENTRIES)
      idx -= HISTORY_ENTRIES;

    wsh_printf("%3d  %s\n", i, st->entries[idx].line);
  }
}

void 
history_print_debug (history_state_t* st)
{
  for (int i=0; i<HISTORY_ENTRIES; i++) {
    wsh_debug_printf("[%2d] %p, len %d, %s\n", 
		     i, &st->entries[i], st->entries[i].end, st->entries[i].line);
  }
  wsh_debug_printf("mode   = %d\n", st->mode);
  wsh_debug_printf("offset = %d\n", st->offset);
  wsh_debug_printf("size   = %d\n", st->size);
  wsh_debug_printf("base   = %d\n", st->base);
  wsh_debug_printf("top    = %d\n", history_index_top(st));
}
