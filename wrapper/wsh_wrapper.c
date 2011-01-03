/*******************************************************************************
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
 * 2010/10/18 PL modified the wsh_debug_printf() to print to the second RS232
 * 2010/10/20 PL wsh_debug_printf() is now enabled through #define WSH_DEBUG 1
 * 2010/10/23 PL changed to slim version of vprintf() w/o float and double support
 *******************************************************************************/

// Project-specific includes including hardware abstraction layer (hal)
#include "wsh/wsh_wrapper.h"  // function prototypes
#include "hal/hal.h"          // hardware abstraction layer (HAL)
#include "hal/cache.h"        // extended PPC caching provisions
#include "hal/modelsim.h"     // printf() handling for ModelSim - PowerPC co-simulation (SIM_MODE)
//#include "hal/my_printf.h"    // unified print functions for non-stdout UARTs

#include <string.h>           // string functions (strcmp)
#include <stdlib.h>
#include <stdarg.h>


// enable in-depth debugging output
//#define WSH_DEBUG 1

extern int xil_vprintf (const char *format, va_list arg_p);  // located in xil_printf.c


// -----------------------------------------------------------------------------
// Implementation of Function Prototypes
// -----------------------------------------------------------------------------
// Need to be implemented in wsh_wrapper.c for each project separately to
// ensure portability.

int wsh_getchar (void) {
  return uart_getchar();
}

int wsh_putchar (int c) {
  return uart_putchar(c);
}

int wsh_strcmp (const char *s1, const char *s2) {
  return strcmp(s1, s2);
}

char *wsh_strcpy (char *dest, const char *src) {
  return strcpy(dest, src);
}

int wsh_strlen (const char *s) {
  return strlen(s);
}

char *wsh_strtok (char *str, const char *delim) {
  return strtok(str, delim);
}

unsigned long wsh_strtoul (const char *nptr, char **endptr, int base) {
  return strtoul(nptr, endptr, base);
}

int wsh_printf (const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  //int r = vprintf(format, ap);    // official vprintf()
  //fflush(stdout);                 // required when using official vprintf()
  int r = xil_vprintf(format, ap);  // slim vprintf()
  va_end(ap);
  return r;
}

int wsh_cache_data_flush (const void *restrict addr_p, unsigned int length) {
  return cache_data_flush(addr_p, length);
}

// wsh debug functionality, uncomment if needed
void wsh_debug_printf (const char* format, ...) {
  #ifdef WSH_DEBUG
  va_list ap;
  char buffer[256];
  va_start(ap, format);
  vsprintf(buffer, format, ap);
  xil_printf2(buffer);  // send debug output to second RS232 using my_printf.c
  va_end(ap);
  #endif
}
