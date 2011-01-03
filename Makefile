sources = wsh/wsh.h wsh/wsh.c \
	wsh/wsh_cmds.h wsh/wsh_cmds.c \
	wsh/wsh_history.h wsh/wsh_history.c \
	wsh/wsh_readline.h wsh/wsh_readline.c \
	wsh/wsh_wrapper.h 

wshell: $(sources)
	cc -I. -std=c99 -Wall -g $(filter %.c,$(sources)) -o $@

clean:
	rm -f $(patsubst %.c,%.o,$(filter %.c,$(sources)))
	rm -f wsh
