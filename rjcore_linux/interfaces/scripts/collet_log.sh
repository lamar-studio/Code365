#!/bin/bash
#

file_name=$1

`rm -rf /tmp/log 1> /dev/null 2>&1`
`mkdir /tmp/log`
`cp -rf /var/log/* /tmp/log 1> /dev/null 2>&1`
`chmod 0777 -R /tmp/log/* 1> /dev/null 2>&1`
`tar zcvPf /tmp/log/$file_name.tar.gz /tmp/log/* 1> /dev/null 2>&1`
res=`echo /tmp/log/$file_name.tar.gz`
echo $res
exit 0
