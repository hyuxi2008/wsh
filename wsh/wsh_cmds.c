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
#include "wsh/wsh_wrapper.h"

// --------------------------------------------------

static unsigned long extract_val(const char *nptr)
{
	char *end;
	return wsh_strtoul(nptr, &end, 0);	// should handle errno somehow, but didn't get it working...
}

// --------------------------------------------------

int wsh_cmd_rd(int argc, char **argv)
{
	if (!argc) {
		wsh_printf("not enough arguments\n");
		wsh_printf("usage: rd ADDR [LENGTH]\n");
		return -1;
	}

	unsigned long addr = extract_val(argv[0]);
	unsigned long nr_words = 1;

	addr &= ~0x3;

	if (argc > 1)
		nr_words = extract_val(argv[1]);

	wsh_cache_data_flush((unsigned long *) addr, nr_words * 4);

	for (int i = 0; i < nr_words; i += 4) {
		wsh_printf("[0x%08lx] ", addr);
		for (int j = i; j < i + 4 && j < nr_words; j++) {
			wsh_printf("0x%08lx ", *(unsigned long *) addr);
			addr += 4;
		}
		wsh_printf("\n");
	}

	return 0;
}

int wsh_cmd_wr(int argc, char **argv)
{
	if (argc < 2) {
		wsh_printf("not enough arguments\n");
		wsh_printf("usage: wr ADDR VALUE [LENGTH]\n");
		return -1;
	}

	unsigned long addr = extract_val(argv[0]);
	unsigned long val = extract_val(argv[1]);
	unsigned long nr_words = 1;
	unsigned long addr_bak;

	addr &= ~0x3;
	addr_bak = addr;

	if (argc == 3)
		nr_words = extract_val(argv[2]);	// extract length

	for (int i = 0; i < nr_words; i++) {
		*(unsigned long *) addr = val;
		addr += 4;
	}

	wsh_cache_data_flush((unsigned long *) addr_bak, nr_words * 4);

	return 0;
}

int wsh_cmd_clear(int argc, char **argv)
{
	wsh_printf("\e[2J");	// erase entire screen (cursor doesn't move)
	return 0;
}

int wsh_cmd_quit(int argc, char **argv)
{
	wsh_stop();
	return 0;
}

int wsh_cmd_help(int argc, char **argv)
{
	wsh_printf(WSH_VERSION);

	wsh_printf("\nregistered commands:\n");
	for (int i = 0; wsh_cmds[i].name; i++) {
		wsh_printf("  %s\n", wsh_cmds[i].name);
	}

	return 0;
}
