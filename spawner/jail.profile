quiet

private
private-dev
private-etc lsb-release
private-tmp

include /etc/firejail/disable-common.inc
include /etc/firejail/disable-programs.inc
include /etc/firejail/disable-passwdmgr.inc

caps.drop all
hostname ant
net none
netfilter
no3d
nonewprivs
noroot
nosound
protocol netlink
shell none
seccomp

read-only /
rlimit-fsize 1024
rlimit-nofile 100
rlimit-nproc 1000
rlimit-sigpending 5
nice -5
