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

#include "wsh/wsh_readline.h"	// shell I/O functionality
#include "wsh/wsh.h"		// support for embedded shell
#include "wsh/wsh_history.h"	// command history
#include "wsh/wsh_wrapper.h"	// function prototypes


enum {
	s_sequence,
	s_no_sequence,
};

typedef enum _edit_mode {
	edit_mode_insert,
	edit_mode_overwrite,
} edit_mode_t;

static history_state_t the_history;

static history_entry_t *the_line;	// current active line
static int cur_pos;		// current positions in line
static edit_mode_t cur_edit_mode;	// current edit mode

static const int prompt_length = PROMPT_LENGTH + 1;
static const char prompt[PROMPT_LENGTH + 1] = PROMPT_STRING;

typedef void (*keymap_func_t) ();

typedef struct _keymap_entry {
	char c;
	keymap_func_t f;
	struct _keymap_entry *next;
} keymap_entry_t;

static void key_up(void);
static void key_down(void);
static void key_right(void);
static void key_left(void);

static void key_home(void);
static void key_insert(void);
static void key_delete(void);
static void key_end(void);

keymap_entry_t keymap_csi[] = {
	{'A', key_up, 0},	// history prev
	{'B', key_down, 0},	// history next
	{'C', key_right, 0},	// move cursor right
	{'D', key_left, 0},	// move cursor left
	{'1', key_home, 0},	// jump to beginning of line
	{'2', key_insert, 0},	// change edit mode (insert or overwrite)
	{'3', key_delete, 0},	// delete char which is right of cursor position
	{'4', key_end, 0},	// jump to end of line
	{0, 0, 0},
};

keymap_entry_t keymap_esc[] = {
	{'[', 0, keymap_csi},
	{0, 0, 0},
};

static void key_backspace(void);

keymap_entry_t keymap[] = {
	{'\e', 0, keymap_esc},
	{'\b', key_backspace, 0},	// delete char which is left of cursor position
	{0, 0, 0},
};

// ----------------------------------------------------------------------

static void move_chars_right(char *line, int pos, int end)
{
	//assert(end < LINE_SIZE);

	for (int i = end + 1; i > pos; i--)
		line[i] = line[i - 1];
}

static void move_chars_left(char *line, int pos, int end)
{
	//assert(pos);

	for (int i = pos; i < end; i++)
		line[i - 1] = line[i];
}

static void update_line(history_entry_t * line)
{
	the_line->line[the_line->end] = '\0';
	if (line) {
		the_line = line;

		//wsh_printf("\e[1K"); // erases only to beginning of line
		wsh_printf("\e[2K");	// erases entire line (including trailing spaces)
		wsh_printf("\e[0G");	// move cursor to begin of line
		wsh_printf("%s%s", prompt, the_line->line);

		cur_pos = the_line->end;
	}
}

static void copy_history_to_current_line(void)
{
	if (the_history.mode == history_mode_browse) {
		the_history.mode = history_mode_normal;

		history_entry_t *elm = the_line;
		the_line = history_get(&the_history, 0);
		wsh_strcpy(the_line->line, elm->line);
		the_line->end = elm->end;
	}
}

// ----------------------------------------------------------------------

static void wsh_debug(const char *msg)
{
	// debug cur_pos and the_line->end
	wsh_debug_printf("%s: cur_pos        = %d\n", msg, cur_pos);
	wsh_debug_printf("%s: the_line->end  = %d\n", msg, the_line->end);
	wsh_debug_printf("%s: the_line->line = '%s'\n", msg, the_line->line);
}

// ----------------------------------------------------------------------

static void key_up(void)
{
	if (the_history.mode == history_mode_normal) {
		the_history.mode = history_mode_browse;
		the_history.offset = 0;
	}

	history_entry_t *prev = history_get_prev(&the_history);
	update_line(prev);
	wsh_debug("key_up");
}

static void key_down(void)
{
	if (the_history.mode == history_mode_normal) {
		the_history.mode = history_mode_browse;
		the_history.offset = 0;
	}

	history_entry_t *next = history_get_next(&the_history);
	update_line(next);
}

static void key_right(void)
{
	if (cur_pos < the_line->end) {
		wsh_printf("\e[C");	// update screen
		cur_pos++;	// update buffer
	}
}

static void key_left(void)
{
	if (cur_pos) {
		wsh_printf("\b");
		cur_pos--;	// update buffer
	}
	wsh_debug("key_left");
}

static void key_home(void)
{
	wsh_getchar();		// consume '~'

	wsh_printf("\e[%dG", prompt_length);
	cur_pos = 0;
}

static void key_insert(void)
{
	wsh_getchar();		// consume '~'

	if (cur_edit_mode == edit_mode_insert)
		cur_edit_mode = edit_mode_overwrite;
	else
		cur_edit_mode = edit_mode_insert;

	wsh_debug_printf("cur_edit_mode = %s\n", cur_edit_mode == edit_mode_insert ? "insert" : "overwrite");
}

static void key_delete(void)
{
	wsh_getchar();		// consume '~'
	copy_history_to_current_line();

	if (cur_pos != the_line->end) {
		wsh_printf("\e[1P");	// erase char at current position

		// update buffer
		//assert(cur_pos < the_line->end);
		move_chars_left(the_line->line, cur_pos + 1,
				the_line->end);
		the_line->end--;
	}
	wsh_debug("key_delete");
}

static void key_end(void)
{
	wsh_getchar();		// consume '~'

	wsh_printf("\e[%dG", prompt_length + the_line->end);
	cur_pos = the_line->end;
}

static void key_backspace(void)
{
	copy_history_to_current_line();

	if (cur_pos) {
		if (cur_pos == the_line->end)
			// cursor is at the end of line: move cursor left and erase line
			wsh_printf("\b\e[K");	// backspace, erase to EOL
		else
			wsh_printf("\b\e[1P");	// backspace, erase char

		// update buffer
		move_chars_left(the_line->line, cur_pos, the_line->end);
		cur_pos--;
		the_line->end--;
	}
}

static int wsh_readchar(void)
{
	int c = 0;
	char state = s_no_sequence;
	keymap_entry_t *map = keymap;

	do {
		c = wsh_getchar();
		if (state == s_no_sequence) {
			// test if there is a entry for 'c' in the first keymap
			keymap_entry_t *entry = map;
			while (entry->c) {
				if (entry->c == c) {
					// there is an entry for this character
					if (entry->f) {
						// function exists so execute it immediately
						(*entry->f) ();
						// consume character so we can read the next char
						c = 0;
					} else {
						// this is a sequence of characters
						map = map->next;
						state = s_sequence;
					}
					break;
				}
				entry++;
			}
		} else {
			// we're in a sequence of character so let's lookup if we
			// find an entry for this character. if not we
			// just jump back to the s_no_sequence state
			keymap_entry_t *entry = map;
			while (entry->c) {
				if (entry->c == c) {
					if (entry->f) {
						(*entry->f) ();
						map = keymap;
						state = s_no_sequence;
						c = 0;
					} else {
						map = map->next;
						break;
					}
				}
				entry++;
			}

			// nothing found on this level so back to the normal state
			if (!entry->c) {
				map = keymap;
				state = s_no_sequence;
			}
		}
	} while (c == 0 || state == s_sequence);

	return c;
}

// ----------------------------------------------------------------------

int wsh_readline_init(void)
{
	for (int i = 0; i < HISTORY_ENTRIES; i++) {
		the_history.entries[i].line[0] = 0;
		the_history.entries[i].end = 0;
	}
	the_history.mode = history_mode_normal;
	the_history.offset = 0;
	the_history.size = 0;
	the_history.base = 0;

	cur_edit_mode = edit_mode_insert;

	wsh_printf("\e[?1l");	// terminal control sequence: set cursor keys in cursor positioning mode

	return 0;
}

const char *wsh_readline(void)
{
	// do not store two identical commands consecutively
	history_entry_t *elm = history_get(&the_history, 1);
	if (elm
	    && the_line
	    && elm->end == the_line->end
	    && wsh_strcmp(elm->line, the_line->line) == 0) {
		the_line->line[0] = '\0';
		the_line->end = 0;
	} else
		the_line = history_get_new(&the_history);

	cur_pos = 0;
	the_line->end = 0;

	wsh_printf("%s", prompt);

	while (1) {
		int c = wsh_readchar();
		copy_history_to_current_line();

		// EOF, newline, carrige return
		if (c == -1 || c == '\n' || c == '\r') {
			if (the_line->end == 0) {
				// if line is empty we just display a new prompt but we don't update the history
				wsh_printf("\n%s", prompt);
				continue;
			} else
				break;
		}
		// if buffer full, ignore character
		if (the_line->end == LINE_SIZE) {
			continue;
		}

		if (cur_edit_mode == edit_mode_overwrite) {
			// edit_mode_overwrite
			wsh_putchar(c);	// update screen
			the_line->line[cur_pos] = c;	// update buffer
			if (cur_pos < the_line->end) {	// fixed bug: don't always increment EOL counter
				cur_pos++;
			} else {
				cur_pos++;
				the_line->end++;	// only increment when being at the end of the line
			}

		} else {
			// edit_mode_insert
			if (cur_pos == the_line->end) {
				wsh_putchar(c);	// update screen
				the_line->line[cur_pos] = c;	// update buffer

			} else {
				wsh_printf("\e[1@%c", c);
				move_chars_right(the_line->line, cur_pos,
						 the_line->end);
				the_line->line[cur_pos] = c;
			}
			cur_pos++;
			the_line->end++;
		}

	}
	the_line->line[the_line->end] = '\0';

	wsh_printf("\n");

	wsh_debug("wsh_readline");

	return the_line->line;
}

// This function belongs to wsh_cmds.c file but it references 'the_history'.
// So either we either export 'the_history' or we move this function here.
int wsh_cmd_history(int argc, char **argv)
{
	history_print(&the_history);
	return 0;
}
