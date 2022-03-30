#ifndef _SIO_H_
#define _SIO_H_

#include <sys/types.h>

#define SIO_BUFSIZE 8192    /* 8KB buffer */

typedef struct {
    char sio_buf[SIO_BUFSIZE]; /* Internal buffer */
    char *sio_bufptr;          /* Next unread byte in internal buf */
    int sio_fd;                /* Descriptor for this internal buf */
    int sio_cnt;               /* Unread bytes in internal buf */
} Sio;

void
sio_initbuf(Sio *sio, int fd); 

ssize_t
sio_readn(Sio *sio, void *usrbuf, size_t n);

ssize_t
sio_read_line(Sio *sio, void *usrbuf, size_t maxlen);

ssize_t
sio_writen(int fd, void *usrbuf, size_t n);

#endif
