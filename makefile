include ../huskymak.cfg

CC = gcc

ifeq ($(DEBUG), 1)
  CFLAGS  = -c -I$(INCDIR) -Wall -ggdb -O6 -DUNIX
  LFLAGS  = -L$(LIBDIR)
else
  CFLAGS  = -c -I$(INCDIR) -Wall -O3 -s -DUNIX
  LFLAGS  = -L$(LIBDIR)
endif


%.o: %.c
	$(CC) $(CFLAGS) $*.c

msgidd: msgidd.o sockutil.o 
	$(CC) -o msgidd msgidd.o sockutil.o

msgidclient: msgidclient.o sockutil.o
	$(CC) -o msgidclient msgidclient.o sockutil.o

all: msgidd msgidclient 

run: msgidd 
	./msgidd

clean:
	rm *.o
	rm *~

distclean: clean
	rm msgidd