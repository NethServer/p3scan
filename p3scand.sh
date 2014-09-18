#!/bin/bash
#
# Init file for p3scan POP3 proxy
#
# chkconfig: 2345 90 05
# description: p3scan proxy server
#
# processname: p3scan
# config: /etc/p3scan/p3scan.conf
# pidfile: /var/run/p3scan.pid

# source function library
. /etc/rc.d/init.d/functions

# Source function library.
. /etc/init.d/functions

test -x /usr/sbin/p3scan || exit 0

RETVAL=0

#
#	See how we were called.
#

prog="p3scan"

start() {
	# Check if p3scan is already running
	if [ ! -f /var/lock/subsys/p3scan ]; then
	    echo -n $"Starting $prog: "
	    daemon /usr/sbin/p3scan
	    RETVAL=$?
	    if [ $RETVAL -eq 0 ] ; then
                touch /var/lock/subsys/p3scan
            fi
	    echo
	fi
	return $RETVAL
}

start_fw() {
    iptables -t nat -I PREROUTING -p tcp -i eth0 --dport pop3  -j REDIRECT --to 8110 2> /dev/null
    iptables -t nat -I OUTPUT  -p tcp --dport pop3 -j REDIRECT --to 8110 2> /dev/null
    iptables -t nat -I OUTPUT  -p tcp --dport pop3 -m owner --uid-owner mail -j ACCEPT 2> /dev/null
}

stop_fw() {
    iptables -t nat -D PREROUTING -p tcp -i eth0 --dport pop3  -j REDIRECT --to 8110 2> /dev/null
    iptables -t nat -D OUTPUT  -p tcp --dport pop3 -j REDIRECT --to 8110 2> /dev/null
    iptables -t nat -D OUTPUT  -p tcp --dport pop3 -m owner --uid-owner mail -j ACCEPT 2> /dev/null
}

stop() {
	echo -n $"Stopping $prog: "
	killproc /usr/sbin/p3scan
	RETVAL=$?
	if [ $RETVAL -eq 0 ] ; then
            rm -f /var/lock/subsys/p3scan
        fi
	echo
        return $RETVAL
}


restart() {
	stop
	start
}	

reload() {
	restart
}	

status_at() {
 	status /usr/sbin/p3scan
}

case "$1" in
start)
	start
	;;
stop)
	stop
	;;
start_fw)
	start_fw
	;;
stop_fw)
	stop_fw
	;;
reload|restart)
	restart
	;;
condrestart)
	if [ -f /var/lock/subsys/p3scan ]; then
	    restart
	fi
	;;
status)
	status_at
	;;
*)
	echo $"Usage: $0 {start|stop|restart|condrestart|status|start_fw|stop_fw}"
	exit 1
esac

exit $?
exit $RETVAL
