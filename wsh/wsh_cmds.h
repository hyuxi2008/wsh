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

#ifndef WSH_CMDS_H
#define WSH_CMDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "wsh/wsh.h"

unsigned long wsh_extract_val(const char *nptr);
int wsh_cmd_rd(int argc, char **argv);
int wsh_cmd_wr(int argc, char **argv);
int wsh_cmd_history(int argc, char **argv);
int wsh_cmd_clear(int argc, char **argv);
int wsh_cmd_quit(int argc, char **argv);
int wsh_cmd_help(int argc, char **argv);

#ifdef __cplusplus
}
#endif
#endif
