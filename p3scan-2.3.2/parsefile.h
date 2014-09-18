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
/*
 * parsefile.h
 * (C) 2002 by Folke Ashberg <folke@ashberg.de>
 *
 * $Id: parsefile.h,v 1.4 2002/06/11 23:09:16 folke Exp $
 *
 * parsefile.c provides functions for parsing text with replacing keywords.
 * it uses my getline.c for string handling
 *
 * This stuff provides functions for linehandlin on file descriptors,
 * especially using netork sockets, because the getline function is 
 * non blocking (the write stuff not!)
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
 */

#ifndef _PARSEFILE_H
#define _PARSEFILE_H

typedef struct paramlist {
    char * name;
    char * value;
    struct paramlist * next;
} paramlist;

/* parses infile to outfile, all words given in params will be replaced
 * leading (\r\n or \n) is one of WRITELINE_LEADING_[NONE|N|RN]
 */
int parsefile(char * infile, char * outfile, paramlist * params, int leading);

/* parses file descriptor in to out, all words given in params will be replaced
 * leading (\r\n or \n) is one of WRITELINE_LEADING_[NONE|N|RN]
 */
int parsefds(int in, int out , paramlist * params, int leading);

/* Adds/Updates name to paramlist.
 * To delete a name, call it with value set to NULL.
 */
int paramlist_set(struct paramlist * params, char * name, char * value);

/* returnes the value of name */
char * paramlist_get(struct paramlist * params, char * name);

/* initialize paramlist */
struct paramlist * paramlist_init(void);
    
/* unitializes the paramlist */
void paramlist_uninit(struct paramlist ** params);

#endif /* ifndef _PARSEFILE_H */
