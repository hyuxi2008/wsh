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

#include "wsh/wsh.h"
#include "wsh/wsh_readline.h"
#include "wsh/wsh_wrapper.h"

#include <stdlib.h>

int wsh_run;

void wsh(void)
{
	char line_tokens[LINE_SIZE + 1];
	char *argv[LINE_SIZE / 2];
	const char *line;
	const char *delims;
	char *result;
	int argc;
	wsh_cmd_t *cmd;
	int err;

	delims = " ";

	wsh_run = 1;
	wsh_readline_init();

	wsh_printf(WSH_VERSION);

	while (wsh_run) {
		line = wsh_readline();
		wsh_strcpy(line_tokens, line);

		argc = 0;
		result = wsh_strtok(line_tokens, delims);
		while (result) {
			argv[argc] = result;
			argc++;
			result = wsh_strtok(0, delims);
		}

		if (argc) {
			cmd = &wsh_cmds[0];

			do {
				if (wsh_strcmp(cmd->name, argv[0]) == 0) {
					err = (*cmd->func) (--argc, &argv[1]);
					if (err < 0)
						wsh_printf("command returned %d\n", err);
					break;
				}

				cmd++;
			} while (cmd->func);

			if (cmd->func != NULL)
				wsh_printf("unknown command '%s'\n", argv[0]);
		}
	}

	wsh_printf("\n  quit wsh()\n");
}
