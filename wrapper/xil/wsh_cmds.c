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
 * 2010/11/08 PL added prototype functions for wsh prologue and epilogue
 */

// Project-specific includes including hardware abstraction layer (hal)
#include "hal/hal.h"		// hardware abstraction layer (HAL)
#include "hal/modelsim.h"	// printf() handling for ModelSim - PowerPC co-simulation (SIM_MODE)
#include "wsh/wsh.h"		// support for embedded shell
#include "wsh/wsh_cmds.h"	// wsh standard commands
#include "wsh/wsh_wrapper.h"	// function prototypes

#include <string.h>		// string functions (strcmp)


// implementation of prototype function: wsh() prologue
int wsh_prologue(void)
{
	return 0;
}

// implementation of prototype function: wsh() epilogue
int wsh_epilogue(void)
{
	return 0;
}

// --- example functions ------------------------------

static uint8 demo = 1;

static void set_demo(uint8 val)
{
	demo = val;
}

static uint8 get_demo(void)
{
	return demo;
}

// --------------------------------------------------

static void print_unknown_arg(char *arg)
{
	xil_printf("*** unknown argument '%s'\n", arg);
}

static void print_unknown_cmd(char *cmd)
{
	xil_printf("*** unknown command '%s'\n", cmd);
}

static void print_too_many_args(void)
{
	xil_printf("*** too many arguments\n");
}

//static void print_non_numeric_arg (char* cmd)
//{
//  xil_printf("*** non-numeric argument '%s'\n", cmd);
//}

// --------------------------------------------------

static int check_argc(int argc, int lower, int upper, char *usage)
{
	if (argc < lower) {
		xil_printf("*** argument missing:\n");
		xil_printf("usage: %s\n", usage);
		return -1;
	}
	if (argc > upper) {
		print_too_many_args();
		return -1;
	}
	return 0;
}

static int check_cmd(int argc, char *top_usage, char **usage)
{
	if (argc < 1) {
		xil_printf("*** command missing\n");
		xil_printf("usage: %s\n", top_usage);
		while (*usage != 0) {
			xil_printf("         %s\n", *usage);
			usage++;
		}
		return -1;
	}
	return 0;
}

static int my_cmd_demo(int argc, char **argv)
{
	char *usage[] = { "set   load demo (1 | 2 | 3)",
		"get   print current demo configuration",
		0
	};

	if (check_cmd(argc, "demo", usage))	// check for existing arguments
		return -1;

	if (strcmp("get", argv[0]) == 0) {
		if (check_argc(argc, 1, 1, ""))	// check # of arguments
			return -1;

		// get demo configuration
		xil_printf("demo configuration %d\n", get_demo());

	} else if (strcmp("set", argv[0]) == 0) {
		if (check_argc(argc, 2, 2, "demo set ID"))	// check # of args: 2 required
			return -1;

		// set demo configuration
		unsigned long val = wsh_extract_val(argv[1]);	// extract numeric value

		if ((val < 1) || (val > 3)) {
			print_unknown_arg(argv[1]);
			return -1;
		}

		set_demo(val);
		xil_printf("selecting demo %d\n", get_demo());

	} else {
		print_unknown_cmd(argv[0]);
		return -1;
	}
	return 0;
}


// specify shell commands
wsh_cmd_t shell_commands[] = {
	{"rd", wsh_cmd_rd},
	{"wr", wsh_cmd_wr},
	{"demo", my_cmd_demo},
	{"history", wsh_cmd_history},
	{"clear", wsh_cmd_clear},
	{"quit", wsh_cmd_quit},
	{"help", wsh_cmd_help},
	{0, 0},			// must be the last entry
};

wsh_cmd_t *wsh_cmds = shell_commands;
