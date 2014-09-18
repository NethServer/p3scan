/*
 * P3Scan v2.3.2
 *
 * (C) 2003-2005 by Jack S. Lai <laitcg@cox.net>
 *
 * This module modified by Simon Santesteban.
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
#ifndef _GETLINE_SSL_H
#define _GETLINE_SSL_H

#include "getlinep3.h"
#include <openssl/ssl.h>
#include <openssl/err.h>

#define BUFSIZZ 1024

extern BIO *bio_err;
int berr_exit (char *string);
int err_exit(char *string);

SSL_CTX *initialize_ctx();
void destroy_ctx(SSL_CTX *ctx);

#ifndef ALLOW_OLD_VERSIONS
#if (OPENSSL_VERSION_NUMBER < 0x00905100L)
#error "Must use OpenSSL 0.9.6 or later"
#endif
#endif


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

int getline_ssl(SSL * ssl, struct linebuf * l);

/* writes line to fd
 * set WRITELINE_LEADING_[NONE|N|RN] in leading
 */
int writeline_ssl(SSL * ssl, int leading, char * c);

/* writes a formatted line to fd
 * set WRITELINE_LEADING_[NONE|N|RN] in leading
 */
int writeline_format_ssl(SSL * ssl, int leading, char * fmt, ...);

/* write len bytes of buf to fd
 * if fd can't receive whole len at once, write repeats
 * until all is sent */
int secure_write_ssl(SSL * ssl, char * buf, int len);

/* runs select (read) on fd with a timeval of 0 (not NULL!) */
int select_fd_read_ssl(SSL * ssl);

/* creates and destroy ssl connection */
int SSL_create_conn(int fd, struct sockaddr * addr, int addr_len,
     SSL ** p_ssl, SSL_CTX ** p_ctx, BIO ** p_sbio);
void SSL_destroy_conn(int sock, SSL * ssl, SSL_CTX * ctx, BIO * sbio);

#endif /* ifndef _GETLINE_H */
