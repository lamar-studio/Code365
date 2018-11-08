#!/bin/bash
#

file_name=$1

`rm -rf /tmp/log`
`mkdir /tmp/log`
`cp -rf /var/log/* /tmp/log`
`chmod 0777 -R /tmp/log/*`
`tar zcvPf /tmp/log/$file_name.tar.gz /tmp/log/* > /dev/null`
echo /tmp/log/$file_name.tar.gz
