#
# spec file for p3scan
#
%define appver 2
%define appmaj 3
%define appmin 2
%define apprel 0
%define rpmrel 4


Summary: Virus scanning transparent proxy server for POP3 POP3S SMTP
Name: p3scan
Version: %{appver}.%{appmaj}.%{appmin}
Release: %{rpmrel}%{?dist}
License: GPL
Group: System Environment/Daemons
URL: http://p3scan.sourceforge.net/
Packager: Jack S. Lai <laitcg at gmail dot com>
BuildRoot: /var/tmp/%{name}-root
Prefix: /usr
Source0: http://prdownloads.sourceforge.net/p3scan/%{name}-%{appver}.%{appmaj}.%{appmin}.tar.gz
Source1: p3scand.sh
Source2: p3scan.service
Patch1: p3scan-2.3-rpmtargetopts.patch
Patch2: p3scan-2.3.2-block-sigchld.patch
BuildRequires: pcre-devel,openssl-devel,clamav-server,systemd
Requires: iptables,pcre
Requires(post): systemd
Requires(preun): systemd
Requires(postun): systemd
Conflicts: pop3vscan

%define user amavis

%description
This is a fully transparent proxy-server for POP3, SMTP, and limited
POP3S Clients. It runs on a Linux box with iptables (for port re-
direction). It can be used to provide email scanning from/to the
internet, to/from any internal network and is ideal for helping to
protect your "Other OS" LAN from harm, especially when used
synergically with a firewall and other Internet Proxy servers.
It is designed to enable scanning of email messages for Viruses,
Worms, Trojans, Spam (read as "Un-solicited Bulk Email"), and harmfull
attachments. Because viewing HTML mail can enable a "Spammer" to
validate an email address (via web bugs), it can also provide
dangerous HTML tag stripping.

%prep

%setup -n %{name}-%{appver}.%{appmaj}.%{appmin}

%patch1 -p1
%patch2 -p1

%build
if [ -f /usr/include/pcre/pcre.h ] || [ -f /usr/local/include/pcre/pcre.h ] ; then
patch -p1 < p3scan-2.3-pcre.patch ;
fi
OPT_FLAGS=`echo $RPM_OPT_FLAGS | sed 's/-Wp,-D_FORTIFY_SOURCE=2 //'`
RPM_OPT_FLAGS="$OPT_FLAGS"
make OPTS="$RPM_OPT_FLAGS" 

%install
[ "$RPM_BUILD_ROOT" != "" ] && rm -rf ${RPM_BUILD_ROOT}
rm -rf ${RPM_BUILD_ROOT}/var/spool/p3scannotify
mkdir -p ${RPM_BUILD_ROOT}/var/spool/p3scan/children
mkdir -p ${RPM_BUILD_ROOT}/var/spool/p3scan/notify
mkdir -p ${RPM_BUILD_ROOT}%{_sysconfdir}/p3scan
mkdir -p ${RPM_BUILD_ROOT}%{_sbindir}
mkdir -p ${RPM_BUILD_ROOT}/var/run/p3scan
mkdir -p ${RPM_BUILD_ROOT}%{_mandir}/man8
mkdir -p ${RPM_BUILD_ROOT}%{_docdir}/%{name}-%{version}

mkdir -p $RPM_BUILD_ROOT%{_unitdir}
install -m 644 %{SOURCE2} $RPM_BUILD_ROOT%{_unitdir}

install -m 750 p3scan ${RPM_BUILD_ROOT}%{_sbindir}
install -m 644 p3scan-*.mail ${RPM_BUILD_ROOT}%{_sysconfdir}/p3scan/
install -m 644 p3scan.8.gz ${RPM_BUILD_ROOT}%{_mandir}/man8
install -m 644 p3scan_readme.8.gz ${RPM_BUILD_ROOT}%{_mandir}/man8
install -m 644 p3scan.sh AUTHORS CHANGELOG CONTRIBUTERS LICENSE LICENSE.OpenSSL NEWS README README-rpm spamfaq.html TODO.list ${RPM_BUILD_ROOT}%{_docdir}/%{name}-%{version}

# install /usr/lib/tmpfiles.d/p3scan.conf
mkdir -p ${RPM_BUILD_ROOT}%{_tmpfilesdir}
cat > ${RPM_BUILD_ROOT}%{_tmpfilesdir}/p3scan.conf <<EOF
# See tmpfiles.d(5) for details

d /var/run/p3scan 0755 amavis amavis - -
EOF

%post
%systemd_post p3scan.service

%clean
rm -rf ${RPM_BUILD_ROOT}

%preun
%systemd_preun p3scan.service

%postun
%systemd_postun_with_restart p3scan.service

%files
%{_sbindir}/p3scan
%{_sysconfdir}/p3scan/p3scan*.mail
%{_unitdir}/p3scan.service
%{_tmpfilesdir}/p3scan.conf
%{_mandir}/man8/p3scan.8.gz
%{_mandir}/man8/p3scan_readme.8.gz
%dir %{_mandir}/man8
%{_docdir}/%{name}-%{version}/*
%defattr(-,%{user},%{user})
%dir /var/run/p3scan
%dir /var/spool/p3scan
%dir /var/spool/p3scan/children
%dir /var/spool/p3scan/notify

%changelog
* Fri Mar 20 2015 Filippo Carletti <filippo.carletti@nethesis.it>
- Block SIGCHLD before fork() - Thanks to efw (FDD)

* Tue May 20 2014 Filippo Carletti <filippo.carletti@nethesis.it>
- initial build 2.3.2
