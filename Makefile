#
# WSH
#
# Copyright (C) 2007-2011  Daniel Wagner <wagi@monom.org>
# Copyright (C) 2007-2011  Peter Luethi <luethi@electronic-engineering.ch>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

sources = wsh/wsh.h wsh/wsh.c \
	wsh/wsh_cmds.h wsh/wsh_cmds.c \
	wsh/wsh_history.h wsh/wsh_history.c \
	wsh/wsh_readline.h wsh/wsh_readline.c \
	wsh/wsh_wrapper.h 

# Enable Linux build by default
sources += wrapper/linux/wsh_cmds.c \
	wrapper/linux/wsh_wrapper.c \
	wrapper/linux/main.c

wshell: $(sources)
	cc -I. -std=c99 -Wall -g $(filter %.c,$(sources)) -o $@

clean:
	rm -f $(patsubst %.c,%.o,$(filter %.c,$(sources)))
	rm -f wshell
