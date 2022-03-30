/*
 * adder.c - a minimal CGI program that adds two numbers together
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE    8192        /* 8KB line buffer */
#define MAX_BUF     1048576     /* 1MB buffer size */

int main(void) {
    char *buf, *p;
    char arg1[MAX_LINE], arg2[MAX_LINE], content[MAX_LINE], line_buf[MAX_LINE];
    int n1=0, n2=0;

    /* Extract the two arguments */
    if ((buf = getenv("QUERY_STRING")) != NULL) {
        p = strchr(buf, '&');
        *p = '\0';
        strcpy(arg1, buf);
        strcpy(arg2, p+1);
        n1 = atoi(arg1);
        n2 = atoi(arg2);
    }

    /* Make the response body */
    content[0] = '\0';
    strcat(content, "Welcome to Tiny adder\r\n<p>");

    sprintf(line_buf, "The answer is: %d + %d = %d\r\n<p>", 
            n1, n2, n1 + n2);
    strcat(content, line_buf);
    strcat(content, "Thanks for visiting!\r\n");

    /* Generate the HTTP response */
    printf("Connection: close\r\n");
    printf("Content-length: %d\r\n", (int)strlen(content));
    printf("Content-type: text/html\r\n\r\n");
    printf("%s", content);
    fflush(stdout);

    exit(0);
}
