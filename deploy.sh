#!/bin/bash -x

function deployLocal {
    cp $1 /etc/fastcgi-daemon/
    cp $2 /usr/local/lib/fastcgi2/dev/
    if [ -f /tmp/fastcgi_daemon.pid ]; then
        kill -9 `cat /tmp/fastcgi_daemon.pid`
    fi
    nohup fastcgi-daemon2 --config="/etc/fastcgi-daemon/$(basename "$1")" &>/dev/null &
}

function deployRemote {
    scp $1 $3:/etc/fastcgi-daemon/
    scp $2 $3:/usr/local/lib/fastcgi2/dev/

    ssh $3 "bash ./start-fastcgi.sh $(basename "$1")"
}

deployLocal $1 $2
#deployRemote $1 $2 192.168.1.35