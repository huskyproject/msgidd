/* msgidclient.c - a main file of the
   simple msgid client for unix enviroments.
   It is based on the tclient code from the book
   "Linux Application Development" */

/* Msgidclient connects to the server whose hostname or IP address is given as
   an command line argument, at port 16980/TCP (msgidd port). Once connected,
   write a string "getMsgId" to the socket, read the data from socket and print
   this data to stdout until server closes a connection. */

/* $Id$ */

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>

#include "sockutil.h"          /* some utility functions */
#include "msgid.h"             /* createMsgId() */


int getMsgId(char *hostName, char *msgId)
{
    struct sockaddr_in address;
    struct in_addr inaddr;
    struct hostent * host;
    int sock, rc;

    memset(msgId, 0, 9);

    /* If the argument can be converted to an IP, do so. If not, try
       to look it up in DNS. */
    if (inet_aton(hostName, &inaddr))
        host = gethostbyaddr((char *) &inaddr, sizeof(inaddr),
                               AF_INET);
    else
        host = gethostbyname(hostName);

    if (!host) {
	/* We can't find an IP number */
        herror("error looking up host");
	exit(1);
    }

    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        die("socket");

    address.sin_family = AF_INET;
    address.sin_port = htons(16890);

    /* Take the first IP address associated with this hostname */
    memcpy(&address.sin_addr, host->h_addr_list[0],
           sizeof(address.sin_addr));

    if (connect(sock, (struct sockaddr *) &address, sizeof(address)))
      {
	printf("Could not connect to msgid Server, create my own msgid\n");
	sprintf(msgId, "%08lX", createMsgId()&0xFFFFFFFF);
	rc = 1;
      }
    else {
      printf("Connected to the msgid Server, getting unique msgid\n");
      write(sock, "getMsgId", 8);
      read(sock, msgId, 8);
      close(sock);
      rc = 0;
    }

    return rc;
}

int main(int argc, char ** argv) {
  int rc;
  char msgId[9];
  int i;

  if (argc != 2) {
    fprintf(stderr, "only a single argument is supported\n");
    return 1;
  }

  for (i = 0; i < 1 ; i++)
      {
	  rc = getMsgId(argv[1], msgId);
	  printf("Got msgId: %s\n", msgId);
	  printf("rc:        %d\n", rc);
      }

  return rc;
}
