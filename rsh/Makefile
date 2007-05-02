
ifdef ENABLE_READLINE
    RD_LDF=-lreadline -ltermcap
    RD_CF=-DADV_READLINE
endif



CC=gcc
CFLAGS=-Wall $(RD_CF)
LDFLAGS=$(RD_LDF)

rsh: main.o conf.o rsh.o adv_shell.o

main.o: conf.h rsh.h adv_shell.h
conf.o: conf.h rsh.h
rsh.o: conf.h rsh.h
adv_shell.o: adv_shell.h

.PHONY: clean

clean:
	rm -f *.o
