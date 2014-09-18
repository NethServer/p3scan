/*
 * P3Scan v2.3.2
 *
 * (C) 2003-2005 by Jack S. Lai <laitcg@cox.net>
 *
 * It's intent is to provide a follow on program to POP3-Virusscan-Proxy 0.4
 * by Folke Ashberg <folke@ashberg.de>.
 *
 * It is based upon his program but provides numerous changes to include
 * scanning pop3 mail for spam, hardening the program, addaption to todays
 * email environment, and many other changes.
 *
 * The initial release of p3scan includes patches made and submitted to the
 * original project but were never incorporated. Please see the README for
 * further information.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * This program is released under the GPL with the additional exemption that
 * compiling, linking, and/or using OpenSSL is allowed."
 * (http://www.openssl.org/support/faq.html#LEGAL2)
 *
 */

#ifndef _GETLINE_H
#define _GETLINE_H

/* return values */
#define GETLINE_OK         0  /* ok, there is a line */
#define GETLINE_NOLINE     1  /* there's no complete line*/
#define GETLINE_NEED_READ  2  /* we need to read from int (but select returned < 0 ) */
#define GETLINE_EOF        -1 /* eof */
#define GETLINE_ERR        -2 /* an error occured */
#define GETLINE_TOO_LONG   -3 /* line is too long for buf */
#define GETLINE_PIPE       -4 /* Socket went away */

/* writeline[_*] \r / \r\n control */
#define WRITELINE_LEADING_NONE   0  /* no lading characters */
#define WRITELINE_LEADING_N      1  /* leading \n */
#define WRITELINE_LEADING_RN     2  /* leading \r\n */

/* values of linelen */
#define GETLINE_LINE_INCOMPLETE  -1 /* line is not complete */
#define GETLINE_LINE_NULL        -2 /* line is NULL (not zero-line)  */

typedef struct linebuf {
    /* these values can be touched if you need */
    char *line;      /* where the line starts */
    int linelen;     /* strlen of line, -1 if not complete */
    /* do not touch these values, we need it */
    char *buf;       /* buffer for data */
    int max;         /* malloced size for buf */
    int len;         /* size which is in buffer */
    char *lineend;   /* pointer to end of line (including [\r]\n */
    char *bufend;    /* pointer to last data char in buf */
    int moredata;    /* is 1 when there is more data then line */
} linebuf;

/* reads a line from fd or l->buf is there any
 * this function is non blocking.
 * return values:
 * GETLINE_OK        : there is a (complete line)
 * GETLINE_NOLINE    : we've received data, but no complete line
 * GETLINE_NEED_READ : we need to read, but select returned no data
 * GETLINE_EOF       : fd is EOF
 * GETLINE_TOO_LONG  : The line can't be retrieved because buf is too
 *                     small. The (incomplete) line is accessable!
 *                     You can call getline again, but it will return
 *                     the rest of that line and not the next one!
 */
int getlinep3(int fd, struct linebuf * l);

/* writes line to fd
 * set WRITELINE_LEADING_[NONE|N|RN] in leading
 */
int writeline(int fd, int leading, char * c);

/* writes a formatted line to fd
 * set WRITELINE_LEADING_[NONE|N|RN] in leading
 */
int writeline_format(int fd, int leading, char * fmt, ...);

/* write len bytes of buf to fd
 * if fd can't receive whole len at once, write repeats
 * until all is sent */
int secure_write(int fd, char * buf, int len);

/* mallocs and zeroes a linebuf
 * buf should be len bytes long */
struct linebuf * linebuf_init(int len);

/* set all values from l to zero
 * any data which are in buf (also unseen lines)
 * will be lost */
void linebuf_zero(struct linebuf * l);

/* frees l */
void linebuf_uninit(struct linebuf * l);

/* runs select (read) on fd with a timeval of 0 (not NULL!) */
int select_fd_read(int fd);

#endif /* ifndef _GETLINE_H */
