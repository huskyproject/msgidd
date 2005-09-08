/* simple MSGID server for unix enviroments
   it is based on the tserver code from the book
   "Linux Application Developmend" */

/* Waits for a connection on port husky = 16890. Once a connection has been
   established, get commands and return appropriate return message, and then
   wait for another connection to the socket.
   Last generated MSGID is stored into disk file "msgid" and daemon reads it
   at start time.
 */

/* $Id$ */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <process.h>
#include <errno.h>

#include "sockutil.h"          /* some utility functions */
#include "msgid.h"             /* createMsgId() */


#define VERSION "0.2"             /* Daemon version */
#define MSGID_STORE_FILE "msgid"  /* disk file to store last generated MSGID value */

volatile int terminate = 0;
static unsigned long msgid = 0;

void termHandler(int signum)
{
  terminate = 1;
}

void writeToFile()
{
  FILE *f;

  f = fopen(MSGID_STORE_FILE, "w");
  if (f != NULL)
    {
      printf("Writing msgId File \"%s\"\n", MSGID_STORE_FILE);
      fprintf(f, "%lu", msgid);
      fclose(f);
    }
  else
    {
      printf("Can't write msgId File: %s\n", strerror(errno));
    }
}

void loadFromFile()
{
   FILE *f;
   char buf[9];

   f = fopen(MSGID_STORE_FILE, "r");
   if (f != NULL)
     {
       buf[0]=0;
       printf("Reading msgId File..\n");
       fgets(buf, 9, f);
       msgid = strtoul(buf, NULL, 10);
       fclose(f);
     }
   else
     {
       printf("Can't load msgId from file, generate new.\n");
     }
   if(msgid==0) msgid = createMsgId();
   printf("Starting with msgid: %08lX\n", msgid);
}

void installSignalHandlers()
{
  struct sigaction sa;

  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = termHandler;
  if (sigaction(SIGTERM, &sa, NULL)) perror("sigaction");

  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = termHandler;
  if (sigaction(SIGINT, &sa, NULL)) perror("sigaction");
}

int main(void) {
    struct sockaddr_in address;
    int sock, conn, i, rc;
    size_t addrLength = sizeof(struct sockaddr_in);
    char inBuf[12], outBuf[9];

    printf("Husky MsgId Server v.%s\n",VERSION);

    loadFromFile();

    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        die("socket");

    /* Let the kernel reuse the socket address. This lets us run
       twice in a row, without waiting for the (ip, port) tuple
       to time out. */
    i = 1;					
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));

    address.sin_family = AF_INET;
    address.sin_port = htons(16890);
    memset(&address.sin_addr, 0, sizeof(address.sin_addr));

    if (bind(sock, (struct sockaddr *) &address, sizeof(address)))
        die("bind");

    if (listen(sock, 5))
        die("listen");

    installSignalHandlers();

    while ((conn = accept(sock, (struct sockaddr *) &address,
                          &addrLength)) >= 0) {

      if (terminate == 1) break;

      memset(&inBuf, 0, sizeof(inBuf));
      printf("---- getting data\n");
      rc = read(conn, &inBuf, sizeof(inBuf)-1);
      //printf("%u : %s\n", rc, inBuf);
      //for (i=0; i < rc; i++) printf("%d\n", inBuf[i]);
      if (strncmp(inBuf, "getMsgId", 8)==0)
	{
	  sprintf(outBuf, "%08lX", (msgid++)&0xffffffff);
	  printf("Generated msgid: %s\n", outBuf);
	  writeToFile();
	  write(conn, outBuf, strlen(outBuf));
	}
      else {
	printf("unknown data\n");
	write (conn, "error", 6);
      }
      printf("---- done\n");
      close(conn);
    }

    close(sock);
    return 0;
}
