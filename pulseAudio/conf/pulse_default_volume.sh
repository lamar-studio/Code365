#!/bin/bash

cnt=0

while true
do
    sources=`pacmd dump |grep set-source-volume |grep -v monitor |awk '{print $2}'`
    if [ -z "$sources" ]; then
        cnt=`expr $cnt + 1`
        sleep 1
        echo "loop cnt:$cnt" >> /var/log/source
    else
        break
    fi
done

echo "$sources" >> /var/log/source

if [ -n "$sources" ]; then
    for source in $sources
    do
        /usr/bin/pacmd set-source-volume $source 0x10000
    done
fi

exit 0
