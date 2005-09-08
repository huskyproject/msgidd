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
#	$(CC) -o msgidd msgidd.o sockutil.o msgid.o

msgidclient: msgidclient.o sockutil.o msgid.o
	$(CC) -o $@ $+
#	$(CC) -o msgidclient msgidclient.o sockutil.o msgid.o

MsgIdClient.class: MsgIdClient.java
	javac MsgIdClient.java

all: msgidd msgidclient MsgIdClient.class

run: msgidd
	./msgidd

clean:
	-rm *.o

distclean: clean
	-rm msgidd
	-rm msgidclient
	-rm MsgIdClient.class
