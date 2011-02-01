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

#ifndef WSH_H			/* prevent circular inclusions */
#define WSH_H			/* by using protection macros */

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
typedef int (*wsh_cmd) (int argc, char **argv);

typedef struct _wsh_cmd_t {
	const char *name;
	wsh_cmd func;
} wsh_cmd_t;

// -----------------------------------------------------------------------------
// General Functions
// -----------------------------------------------------------------------------

// general wsh start-up function
void wsh(void);


#ifdef __cplusplus
}
#endif
#endif				//WSH_H
