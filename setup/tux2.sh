ifconfig eth0 172.16.11.1/24 up
route add -net 172.16.10.0/24 gw 172.16.11.253
route add default gw 172.16.11.254
sysctl net.ipv4.conf.eth0.accept_redirects=0
sysctl net.ipv4.conf.all.accept_redirects=0

