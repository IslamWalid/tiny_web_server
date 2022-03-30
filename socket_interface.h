#ifndef _INTERFACE_H_
#define _INTERFACE_H_

int
open_listenfd(char *port);

int
open_clientfd(char *hostname, char *port);

#endif
