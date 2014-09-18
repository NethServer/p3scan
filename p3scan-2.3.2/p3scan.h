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

#ifndef _P3SCAN_H
#define _P3SCAN_H

#include <syslog.h>           /* do_log */
#include <arpa/inet.h>        /* config */
#include "getline_ssl.h"

#define PROGNAME              "P3Scan"
#define VERSION               "2.3.2"
#define MAX_PSEUDO_ARGV       50
#define MESSAGE_NOVIRINFO     "<no virusinfo could be examined>"
#define FILEDEL               "The infected message has been deleted."
#define SVRCMD                "NOOP"
#define PERIOD                "."
#define BOGUSX                "X-P3Scan: Due to an extremely large attachment you see this message line."
#define NUL                   '\0'
#define LEN 65536             /* for substr() */

#define CONFIG_STATE_CMDPRE   1
#define CONFIG_STATE_FILE     2
#define CONFIG_STATE_CMD      3

#define SCANNER_INIT_NO       0
#define SCANNER_INIT_OK       1
#define SCANNER_INIT_NULL     2 /* scanner needs no init */
#define SCANNER_INIT_ERR      -1

#define SCANNER_RET_OK        0
#define SCANNER_RET_ERR       -1
#define SCANNER_RET_VIRUS     2
#define SCANNER_RET_CRIT      3

#define MAX_VIRUS_CODES       16

#if defined(__USE_FILE_OFFSET64) || defined(__USE_LARGEFILE64)
#define FSTAT(a, b) fstat64(a,b)
#define LSTAT(a, b) lstat64(a,b)
#define STAT(a, b) stat64(a,b)
#define stat_t stat64
#else
#define FSTAT(a, b) fstat(a,b)
#define LSTAT(a, b) lstat(a,b)
#define STAT(a, b) stat(a,b)
#define stat_t stat
#endif
#ifndef TMP_MAX
# define TMP_MAX 238328
#endif

#define SETIFNULL(a,b) if (!a) a=b  /* wow, that's magic */
#define NONULL(x) ( x==NULL ? "" : x) /* this is nice, found in the mutt code */

#define TRIM(a) \
{ \
   int len=0; \
   while ((a)[len] && isspace((a)[len])) len++; \
   memmove((a),&(a)[len], strlen(&(a)[len])+1); \
   while ((len=strlen(a))>0 && isspace((a)[len-1])) (a)[len-1]='\0'; \
}

/* default configuration, anything can be changed at runtime */
#define LETTERS                  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
#define PORT_NUMBER              8110
#define SMTP_PORT                25
#define POP3_PORT                110
#define POP3SPORT                995
#define MAX_CHILDS               10
#define TIMEOUT                  30
#define RUNAS_USER               "mail"
#define VIRUS_DIR                "/var/spool/p3scan"
#define ABORTFILE                "/var/spool/p3scan/p3scan.abort"
#define NOTIFY_MAIL_DIR          "/var/spool/p3scan/notify"
#define VIRUS_SCANNER            NULL
#define VIRUS_SCANNER_VIRUSCODE  1
#define PID_FILE                 "/var/run/p3scan/p3scan.pid"
#define SYSLOG_NAME              "p3scan"
#define CONFIGFILE               "/etc/p3scan/p3scan.conf"
#define VIRUS_TEMPLATE           "/etc/p3scan/p3scan.mail"
#define DEBUG                    0
#define QUIET                    0
#define OVERWRITE                NULL
#define CHECKSPAM                0
#define SPAMCHECK                "/usr/bin/spamc"
#define MINSPACE                 10000
#define DELIT                    0
#define SUBJECT                  "[Virus] found in a mail to you:"
#define NOTIFY                   "Per instruction, the message has been deleted."
#define SMTPRSET                 "Virus detected! P3scan rejected message!"
#define XMAIL                    "/bin/mail"
#define FOOTER                   "/etc/p3scan/p3scan.footer"
#define EXTRA                    "/etc/p3scan/p3scan.extra"
#define EMERGCON                 "root@localhost postmaster@localhost"
/* Defaut maximum mail size for scanning. ZERO for no limit! */
#define MAX_SIZE_SCAN            0
/* TOS:  do not set, or use IPTOS_[LOWDELAY|THROUGHPUT|RELIABILITY|LOWCOST] */
#define SET_TOS                  IPTOS_THROUGHPUT
#define MOVEIT                   "/bin/mv"
#define COPYIT                   "/bin/cp"
#define POK                      "+OK"
#undef DEBUG_MEM                 /* print meminfo every log message when debugging */
#undef DEBUG_MESSAGE             /* print message lines */
#undef DEBUG_SCANNING            /* print message lines while scanning */
#undef DEBUG_FOOTER              /* print virus scanner version info */
#undef DEBUG_SMTP                /* print smtp messages lines */

/* undef this to not use the ripmime library
   You also need to change the symlink for Makefile to the proper
   Makefile. Either Makefile-ripmime or Makefile-noripmime */
#define DEMIME
/* print TIME with debug output */
#define DTIME
// Logging options defaults
#ifndef LOGOPT
#define LOGOPT LOG_PID|LOG_CONS
#endif
#ifndef LOGFAC
#define LOGFAC LOG_DAEMON
/* To log to file:
   In the Makefile, change LOG_DAEMON to:
   LOGFAC="LOG_LOCAL0"
   or if not defined there, comment above and uncommment below.

   Add to /etc/syslog.conf (and restart it):
   local0.*                                                -/var/log/p3scan
*/
//#define LOGFAC LOCAL0
#endif
/* default configuration ends here */

typedef struct proxycontext {
   struct  linebuf   *serverbuf;
   struct  linebuf   *clientbuf;
   struct  linebuf   *hdrbuf;
   struct  sockaddr_in   client_addr;
   struct  sockaddr_in   server_addr;
   struct  hostent   *server_host;
   size_t        socksize;
   off_t         hdroffset;
   time_t        now;
   int           client_fd;
   int           server_fd;
   int           header_fd;
   int           ismail;
   unsigned int  msgnum;
   int           header_exists;
   int           fakehdrdone;
   int           notified;
   int           noop;
   int           errmsg;
   struct paramlist *params;
   int           mailcount;
   unsigned long bytecount;
   int           gobogus;
   int           boguspos;
   int           hdrdate;
   int           hdrfrom;
   int           hdrto;
   char          mailfile[4096];
   char          maildir[4096]; /* mailfile.content */
   char          p3shdrfile[4096];
   char          cbuff[1];
   char          * filename;
   char          * scanthis; /* depending on demime linked to mailfile / maildir */
   char          * virinfo; /* has to be filled from the scanner */
   char          * filestatus; /* infected mail kept or deleted */
   int           scannerinit; /* see SCANNER_INIT_* */
   char          * extrasubj; /* extra notification subject line */
   int           cksmtp; /* if scanning an smtp submission */
   int           checksmtp; /* used to bypass checking for smtp */
   int           topping;
   int           posttop;
   int           usessl;
   SSL           * ssl;
   SSL_CTX       * ctx;
   BIO           * sbio;
   char          vnmsg[4096];
   char          * dspamuser;
   char *        actsvr;
   int           actport;
   int           extra;
} proxycontext;

typedef struct scanner_t {
   char    *name;
   char    *descr;
   int     (*init1)(void);
   int     (*init2)(struct proxycontext *);
   int     (*scan)(struct proxycontext *, char ** virname);
   void    (*uninit2)(struct proxycontext *);
   void    (*uninit1)(void);
   int     dirscan;
} scanner_t;


typedef struct configuration_t {
   int         maxchilds;
   char *      renattach;
   char *      runasuser;
   char *      virusdirbase;
   char *      virusdir;
   char *      notifydir;
   char *      virustemplate;
   char *      virusscanner;
   int         viruscode[MAX_VIRUS_CODES + 1];
   int         gvcode[MAX_VIRUS_CODES + 1];
   char *      virusregexp;
   int         virusregexpsub;
   int         demime;
   char *      pidfile;
   char *      syslogname;
   char *      configfile;
   int         debug;
   char *      overwrite;
   struct      sockaddr_in addr;
   struct      sockaddr_in targetaddr;
   scanner_t * scanner;
   int         scannerenabled;
   int         quiet;
   int         checkspam;
   char *      spamcheck;
   unsigned long freespace;
   int         delit;
   int         ispam;
   int         broken;
   char *      subject;
   char *      notify;
   char *      ispspam;
   char *      extra;
   char *      smtprset;
   int         smtpsize;
   char *      clamdserver;
   char *      clamdport;
   int         sslport;
   char *      mail;
   int         timeout;
   char *      footer;
   int         altemail;
   int         smtpport;
   int         enabletop;
   int         child;
   int         useurl;
   char *      emergcon;
   char *      emergency;
} configuration_t;

extern void do_log(int level, const char *fmt,...);

void context_uninit(struct proxycontext * p);

// The structure we use to maintain a list of allocated memory
typedef struct memory_list {
  void *address;
  size_t size;
  struct memory_list *next;
  struct memory_list *prev;
} memory_list;

// The global memory allocation list used by the memory management wrappers
memory_list *memory_list_head;
// The global couter indicating the number of bytes allocated
unsigned long global_memory_count;
// A one-time initialization function to setup the memory list and global memory count variables
void w_memory_init(void);

typedef void Sigfunc(int);

#endif
