#/bin/bash
#

while true
do
buf=`pacmd list-sources |grep -A 1 "* index:" |grep usb`
echo $buf >> /root/press.log
temp=`echo $buf |grep monitor`
[ -n "$temp" ] && exit 0
pkill pulseaudio
sleep 1.5
done
