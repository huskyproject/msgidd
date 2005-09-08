/* sockutil.h - simple utility functions declarations */

/* $Id$ */

#ifndef __HUSKY__SOCKUTIL_H
#define __HUSKY__SOCKUTIL_H

void die(char * message);

#ifndef CMSG_DATA
#define CMSG_DATA(cmsg) ((cmsg)->cmsg_data)
#endif

#endif
