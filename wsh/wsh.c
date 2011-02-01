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

#include "wsh/wsh.h"		// support for embedded shell
#include "wsh/wsh_readline.h"	// shell I/O functionality
#include "wsh/wsh_wrapper.h"	// function prototypes


// function prototypes to be defined in wsh_cmds.c (project-specific)
extern int wsh_prologue(void);
extern int wsh_epilogue(void);


int wsh_run;			// global variable to allow to quit wsh()


// general wsh start-up function
void wsh(void)
{
	char line_tokens[LINE_SIZE + 1];

	wsh_run = 1;
	wsh_readline_init();

	wsh_printf(WSH_VERSION);

	wsh_prologue();		// call entry function (user-defined)

	while (wsh_run) {
		const char *line = wsh_readline();
		wsh_strcpy(line_tokens, line);

		// tokenize command line
		int argc = 0;
		char *argv[LINE_SIZE / 2];
		char delims[] = " ";

		char *result = wsh_strtok(line_tokens, delims);
		while (result) {
			argv[argc] = result;
			argc++;
			result = wsh_strtok(0, delims);
		}

		// execute command
		if (argc) {
			wsh_cmd_t *cmd = &wsh_cmds[0];
			//assert(cmd->func);

			do {
				if (wsh_strcmp(cmd->name, argv[0]) == 0) {
					int r = (*cmd->func) (--argc, &argv[1]);
					if (r != 0)
						wsh_printf("command returned %d\n", r);
					break;
				}

				cmd++;
			} while (cmd->func);

			if (!cmd->func)
				wsh_printf("unknown command '%s'\n", argv[0]);
		}
	}

	wsh_epilogue();		// call exit function (user-defined)

	wsh_printf("\n  quit wsh()\n");
}
