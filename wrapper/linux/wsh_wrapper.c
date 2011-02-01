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

// Project-specific includes including hardware abstraction layer (hal)
#include "wsh/wsh_wrapper.h"	// function prototypes

//#include "hal/my_printf.h"    // unified print functions for non-stdout UARTs

#include <string.h>		// string functions (strcmp)
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>


// enable in-depth debugging output
//#define WSH_DEBUG 1

extern int xil_vprintf(const char *format, va_list arg_p);	// located in xil_printf.c


// -----------------------------------------------------------------------------
// Implementation of Function Prototypes
// -----------------------------------------------------------------------------
// Need to be implemented in wsh_wrapper.c for each project separately to
// ensure portability.

int wsh_getchar(void)
{
	return getchar();
}

int wsh_putchar(int c)
{
	return putchar(c);
}

int wsh_strcmp(const char *s1, const char *s2)
{
	return strcmp(s1, s2);
}

char *wsh_strcpy(char *dest, const char *src)
{
	return strcpy(dest, src);
}

int wsh_strlen(const char *s)
{
	return strlen(s);
}

char *wsh_strtok(char *str, const char *delim)
{
	return strtok(str, delim);
}

unsigned long wsh_strtoul(const char *nptr, char **endptr, int base)
{
	return strtoul(nptr, endptr, base);
}

int wsh_printf(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	int r = vprintf(format, ap);	// official vprintf()
	fflush(stdout);		// required when using official vprintf()
	va_end(ap);
	return r;
}

int wsh_cache_data_flush(const void *restrict addr_p, unsigned int length)
{
	//return cache_data_flush(addr_p, length);
	return 0;
}

// wsh debug functionality, uncomment if needed
void wsh_debug_printf(const char *format, ...)
{
#ifdef WSH_DEBUG
	va_list ap;
	char buffer[256];
	va_start(ap, format);
	vsprintf(buffer, format, ap);
	printf(buffer);		// send debug output to second RS232 using my_printf.c
	va_end(ap);
#endif
}
