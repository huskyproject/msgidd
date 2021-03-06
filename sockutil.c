/* sockutil.c - simple utility functions */

/* $Id$ */

#include <stdio.h>
#include <stdlib.h>

#include "sockutil.h"

/* issue an error message via perror() and terminate the program */
void die(char * message) {
    perror(message);
    exit(1);
}
