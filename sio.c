#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "sio.h"

static ssize_t
sio_read(Sio *sio, char *usrbuf, size_t n);

static const char *broken_pipe_msg = "broken pipe: writing to a connection that has already been closed by the client";
static const char *read_error_msg = "Read error: reading from a socket that has been prematurely close";

/*
 * sio_initbuf - Associate a descriptor with a read buffer and reset buffer
 */
void
sio_initbuf(Sio *sio, int fd)
{
    sio->sio_fd = fd;  
    sio->sio_cnt = 0;  
    sio->sio_bufptr = sio->sio_buf;
}

/*
 * sio_readn - Safely read n bytes (buffered)
 */
ssize_t
sio_readn(Sio *sio, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;
    
    while (nleft > 0) {
	    if ((nread = sio_read(sio, bufp, nleft)) < 0) 
            return -1;          /* errno set by read() */ 
	    else if (nread == 0)
	        break;              /* EOF */
	    nleft -= nread;
	    bufp += nread;
    }
    return (n - nleft);         /* return >= 0 */
}

/* 
 * sio_read_line - Safely read a text line (buffered)
 */
ssize_t
sio_read_line(Sio *sio, void *usrbuf, size_t maxlen)
{
    int n, rc;
    char c, *bufp = usrbuf;

    for (n = 1; n < maxlen; n++) { 
        if ((rc = sio_read(sio, &c, 1)) == 1) {
	    *bufp++ = c;
	    if (c == '\n') {
                n++;
     		break;
            }
	} else if (rc == 0) {
	    if (n == 1)
		return 0;       /* EOF, no data read */
	    else
		break;          /* EOF, some data was read */
	} else
	    return -1;	    /* Error */
    }
    *bufp = 0;
    return n-1;
}

/*
 * sio_writen - Safely write n bytes (unbuffered)
 */
ssize_t
sio_writen(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = usrbuf;

    while (nleft > 0) {
	    if ((nwritten = write(fd, bufp, nleft)) <= 0) {
	        if (errno == EINTR) {           /* Interrupted by sig handler return */
	    	    nwritten = 0;               /* and call write() again */
            } else if (errno == EPIPE) {    /* Interrupted by SIGPIPE */
                fprintf(stderr, "%s\n", broken_pipe_msg);
                errno = 0;              /* Reset errno */     
                return -1;
            } else {
	    	    return -1;              /* errno set by write() */
            }
	    }
	    nleft -= nwritten;
	    bufp += nwritten;
    }
    return n;
}

/* 
 * sio_read - This is a wrapper for the Unix read() function that
 *    transfers min(n, sio_cnt) bytes from an internal buffer to a user
 *    buffer, where n is the number of bytes requested by the user and
 *    sio_cnt is the number of unread bytes in the internal buffer. On
 *    entry, sio_read() refills the internal buffer via a call to
 *    read() if the internal buffer is empty.
 */
static ssize_t
sio_read(Sio *sio, char *usrbuf, size_t n)
{
    int cnt;

    while (sio->sio_cnt <= 0) {  /* Refill if buf is empty */
	    sio->sio_cnt = read(sio->sio_fd, sio->sio_buf, 
	    		   sizeof(sio->sio_buf));
	    if (sio->sio_cnt < 0) {
	        if (errno != EINTR) {/* Interrupted by sig handler return */
	    	    return -1;
            } else if (errno == ECONNRESET) {
                fprintf(stderr, "%s\n", read_error_msg);
                errno = 0;
                return -1;
            }
	    } else if (sio->sio_cnt == 0) { /* EOF */
	            return 0;
        } else {
	        sio->sio_bufptr = sio->sio_buf; /* Reset buffer ptr */
        }
    }

    /* Copy min(n, sio->sio_cnt) bytes from internal buf to user buf */
    cnt = n;          
    if (sio->sio_cnt < n)   
	    cnt = sio->sio_cnt;
    memcpy(usrbuf, sio->sio_bufptr, cnt);
    sio->sio_bufptr += cnt;
    sio->sio_cnt -= cnt;
    return cnt;
}
