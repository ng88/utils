
ifdef DEBUGPRINT
    DDPFLAGS=
else
    DDPFLAGS=-DDISABLE_DEBUG_PRINT=1
endif

# disable the plugin system
ifdef DISABLE_PLUGINS
    DONT_USE_DL=1
endif

ifdef DONT_USE_DL
    DUDL=
    LDDL=
else
    DUDL=-DUSE_DL=1
    LDDL=-rdynamic -ldl
endif

ifdef DISABLE_VTTY
    VTTY=
else
    VTTY=-DVTTY=1
endif


ifdef DEBUG
    DEBUGFLAGS=-g -ggdb -dH -D_DEBUG_=1 -DDEBUG=1
    STRIP=@echo
else
    DEBUGFLAGS=
    STRIP=strip
endif

ifdef IPV6
    IPF=-DIPV=6 -DIPV6=1
else
    IPF=-DIPV=4 -DIPV4=1
endif

ifdef NOASSERT
    ASSERTFLAGS=-DDISABLE_ASSERT=1
else
    ASSERTFLAGS=
endif

ifdef REV
    REVFLAGS=-D__SVN_HEAD__=\"$(REV)\"
    REVCMD=true
else
    REVFLAGS=-D__SVN_HEAD__=\"`cat last_svn_rev~`\"
    REVCMD=../../common/get_svn_rev.sh
endif

CC=gcc
CFLAGS=-W -Wall -Wno-unused -O3 $(REVFLAGS) $(DEBUGFLAGS) $(ASSERTFLAGS) $(DDPFLAGS) $(DUDL) $(PLUGFLAGS) $(IPF) $(VTTY)
LDFLAGS=

SRC=common.c vector.c md5.c misc.c

SRC1=btund.c server.c stats.c channel.c user.c $(SRC)
OBJS1=$(SRC1:.c=.o)
EXE1=btund

SRC2=btun.c client.c plugin.c $(SRC)
OBJS2=$(SRC2:.c=.o)
EXE2=btun

all: $(EXE1) $(EXE2)

btun.o: assert.h bool.h client.h version.h protocol.h misc.h vector.h plugin.h
btund.o: server.h user.h version.h protocol.h misc.h bool.h assert.h
channel.o: vector.h user.h bool.h channel.h server.h assert.h
client.o: bool.h common.h plugin.h client.h assert.h misc.h
common.o: protocol.h md5.h common.h bool.h assert.h
md5.o: md5.h assert.h
misc.o: bool.h misc.h
plugin.o: plugin_def.h assert.h vector.h plugin.h
server.o: user.h channel.h common.h server.h assert.h vector.h stats.h
stats.o: bool.h stats.h assert.h
user.o: vector.h bool.h protocol.h user.h assert.h
vector.o: vector.h assert.h

$(EXE1): svnrev $(OBJS1)
	$(CC) $(OBJS1) -o $@ $(LDFLAGS)
	$(STRIP) $@ > /dev/null

$(EXE2): svnrev $(OBJS2)
	$(CC) $(OBJS2) -o $@ $(LDFLAGS) $(LDDL)
	$(STRIP) $@ > /dev/null
	@[ -z $(DUDL) ] || echo Making plugins...
	@[ -z $(DUDL) ] || $(MAKE) -C plugins

.PHONY: clean mrproper svnrev win

svnrev:
	@$(REVCMD) > last_svn_rev~

clean:
	@rm -f *.o */*.o *~ core *.core core.* *.tmp
	$(MAKE) -C plugins clean

mrproper: clean
	@rm -f $(EXE1) $(EXE2)
	$(MAKE) -C plugins mrproper

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

win:
	@echo Building Windows version...
	@$(MAKE) NOASSERT=1 DISABLE_PLUGINS=1 DONT_USE_DL=1 WIN=1 DISABLE_VTTY=1 btun btund
