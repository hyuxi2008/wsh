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
 *******************************************************************************/

#include "wsh/wsh.h"          // support for embedded shell
#include "wsh/wsh_wrapper.h"  // function prototypes


extern int wsh_run;  // global variable to allow to quit wsh()


// --------------------------------------------------

unsigned long wsh_extract_val (const char *nptr)
{
  char* end;
  return wsh_strtoul(nptr, &end, 0);  // should handle errno somehow, but didn't get it working...
}

// --------------------------------------------------

int wsh_cmd_rd (int argc, char** argv)
{
  if (!argc) {
    wsh_printf("not enough arguments\n");
    wsh_printf("usage: rd ADDR [LENGTH]\n");
    return -1;
  }

  unsigned long addr = wsh_extract_val(argv[0]);
  unsigned long nr_words = 1;

  addr &= ~0x3;

  if (argc > 1)
    nr_words = wsh_extract_val(argv[1]);

  wsh_cache_data_flush((unsigned long*) addr, nr_words*4);

  for (int i=0; i<nr_words; i+=4) {
    wsh_printf("[0x%08lx] ", addr);
    for (int j=i; j<i+4 && j<nr_words; j++) {
      wsh_printf("0x%08lx ", *(unsigned long*)addr);
      addr += 4;
    }
    wsh_printf("\n");
  }
  
  return 0;
}

int wsh_cmd_wr (int argc, char** argv)
{
  if (argc < 2) {
    wsh_printf("not enough arguments\n");
    wsh_printf("usage: wr ADDR VALUE [LENGTH]\n");
    return -1;
  }

  unsigned long addr = wsh_extract_val(argv[0]);
  unsigned long val = wsh_extract_val(argv[1]);
  unsigned long nr_words = 1;
  unsigned long addr_bak;

  addr &= ~0x3;
  addr_bak = addr;

  if (argc == 3)
    nr_words = wsh_extract_val(argv[2]);  // extract length

  for (int i=0; i<nr_words; i++) {
    *(unsigned long*)addr = val;
    addr += 4;
  }

  wsh_cache_data_flush((unsigned long*) addr_bak, nr_words*4);

  return 0;
}

int wsh_cmd_clear (int argc, char** argv)
{
  wsh_printf("\e[2J");  // erase entire screen (cursor doesn't move)
  return 0;
}

int wsh_cmd_quit (int argc, char** argv)
{
  wsh_run = 0;  // quit wsh()
  return 0;
}

int wsh_cmd_help (int argc, char** argv)
{
  wsh_printf(WSH_VERSION);

  wsh_printf("\nregistered commands:\n");
  for (int i=0; wsh_cmds[i].name; i++) {
    wsh_printf("  %s\n", wsh_cmds[i].name);
  }

  return 0;
}