#!/bin/bash
devname=`echo $1`
address=$2
netmask=$3
gateway=$4
#network=$5
#broadcast=$6

if [ -z $devname ];then
  exit 1
fi

. /etc/rc_script/function.sh

netifnamelist=`/etc/rc_script/getnetifnamelist.sh`

find_strunit "$devname" "$netifnamelist"

if [ ! $? = 1 ];then
  echo invaled netif
  exit 1
fi

cd /etc/network/rc_inet/

echo "auto $devname" > $devname.tmp 
if [ -z $address ];then
echo "iface $devname inet dhcp" >> $devname.tmp
else
echo "iface $devname inet static" >> $devname.tmp
[ ! -z $address ] && echo "	address $address" >> $devname.tmp
[ ! -z $netmask ] && echo "	netmask $netmask" >> $devname.tmp
[ ! -z $gateway ] && echo "	gateway $gateway" >> $devname.tmp
[ ! -z $network ] && echo "	network $network" >> $devname.tmp
[ ! -z $broadcast ] && echo "	broadcast $broadcast" >> $devname.tmp
fi
echo "" >> $devname.tmp
mv $devname.tmp $devname

cat base > ../interfaces.tmp
for dev in $netifnamelist
do
  if [ -f "$dev" ];then
    cat $dev >> ../interfaces.tmp
    [ -e mk_${dev}_* ] && cat mk_${dev}_* >> ../interfaces.tmp
  fi
done

touch /tmp/.br0.lock

ifdown $devname
[ ! -z "`LC_ALL=c ifconfig | grep "^$devname\s"`" ] && ifconfig $devname down
[ -e mk_${devname}_* ] && realdevname=`echo mk_${devname}_* | cut -d_ -f3` && ifconfig $realdevname down
mv ../interfaces.tmp ../interfaces
sync
sleep 1
ifup $devname
retifup=$?
if [ -z "`LC_ALL=c ifconfig | grep "^$devname\s"`" ];then
  ifdown $devname
  [ ! -z "`LC_ALL=c ifconfig | grep "^$devname\s"`" ] && ifconfig $devname down
  [ ! -z $realdevname ] && ifconfig $realdevname down
  sleep 1
  ifup $devname
  retifup=$?
  if [ -z "`LC_ALL=c ifconfig | grep "^$devname\s"`" ];then
    dev_drv=`LC_ALL=c ethtool -i $devname | awk '/driver/{print $2}'`
    [ ! -z $realdevname ] && realdev_drv=`LC_ALL=c ethtool -i $realdevname | awk '/driver/{print $2}'`
    rmmod $dev_drv
    [ ! -z $realdev_drv ] && rmmod $realdev_drv
    sleep 1
    [ ! -z $realdev_drv ] && modprobe $realdev_drv
    modprobe $dev_drv
    sleep 1
    ifdown $devname
    [ ! -z "`LC_ALL=c ifconfig | grep "^$devname\s"`" ] && ifconfig $devname down
    [ ! -z $realdevname ] && [ ! -z "`LC_ALL=c ifconfig | grep "^$realdevname\s"`" ] &&  ifconfig $realdevname down
    sleep 1
    ifup $devname
    retifup=$?
    systemctl restart libvirt-bin
    if [ -z "`LC_ALL=c ifconfig | grep "^$devname\s"`" ];then
	  rm -rf /tmp/.br0.lock
      exit 1
    fi
  fi
fi

rm -rf /tmp/.br0.lock
exit $retifup

