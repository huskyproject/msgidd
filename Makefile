include ../huskymak.cfg

CC = gcc

ifeq ($(DEBUG), 1)
  CFLAGS  = -c -I$(INCDIR) -Wall -ggdb -O6 -DUNIX
  LFLAGS  = -L$(LIBDIR)
else
  CFLAGS  = -c -I$(INCDIR) -Wall -O3 -s -DUNIX
  LFLAGS  = -L$(LIBDIR)
endif

default: all

%.o: %.c
	$(CC) $(CFLAGS) $*.c

msgidd: msgidd.o sockutil.o msgid.o
	$(CC) -o $@ $+

msgidclient: msgidclient.o sockutil.o msgid.o
	$(CC) -o $@ $+

MsgIdClient.class: MsgIdClient.java
	javac $+

all: msgidd msgidclient MsgIdClient.class

run: msgidd
	./msgidd

clean:
	-rm *.o

distclean: clean
	-rm msgidd
	-rm msgidclient
	-rm MsgIdClient.class
