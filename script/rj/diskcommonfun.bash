#!/bin/bash

g_diskcommonfun_pathdir=$1
g_sizeG_request=90
g_size_request=$(($g_sizeG_request*1000*1000*1000))

kecho()
{
#  echo "[$0] $@" > /dev/ttyprintk
  echo "[$0]:${FUNCNAME[1]} $@" >> /dev/ttyprintk
}

function getAllBlockDevList()
{
    local devpath
    local devlist
    local ret=1
    kecho param:$*
    # /etc/sethwinfo record all old idv machine disk type. rg_lessons_type=sdx is ssd type, rg_lessons_type=mmcblkx_sdx is emmc+hdd type.
    [ -f $g_diskcommonfun_pathdir/sethwinfo ] && . $g_diskcommonfun_pathdir/sethwinfo >/dev/null 2>&1
    if [ "$rg_lessons_type" = "sdx" ]; then
        # maybe old ssd idv machine have emmc disk, we do not care it. ignore emmc
        devpath=`ls /sys/block/sd[a-z] -d 2>/dev/null`
    else
        devpath=`ls /sys/block/sd[a-z] /sys/block/mmcblk[0-9] -d 2>/dev/null`
    fi
    if [ -n "$devpath" ]; then
        # not contain of usb devices
        for path in `realpath $devpath  | grep -v -E "/usb[0-9]+/"`
        do
            devlist="$devlist `basename $path`"
            ret=0
        done
    fi

    kecho devlist:$devlist ret:$ret
[ -n "$gtest_dev" ] && echo $gtest_dev && return 0
    [ $ret -eq 0 ] && echo $devlist
    return $ret
}

function paramBlockDevType()
{
    local sdev=$1
    local ssdflag
    local devtype
    local ret=1
    kecho param:$*
    kecho sdev:$sdev
    if [ -b /dev/$sdev ]; then
        # emmc ssd device, rota flag is 0, is not a rotation devices.
        # hdd device, rota flag is 1, is a rotation devices.
        ssdflag=`LC_ALL=C lsblk -d -o name,rota /dev/$sdev | grep $sdev | awk '{print $2}'`
        ret=$?
        if [ ! $ret -eq 0 ]; then
            kecho failed to run cmd: lsblk
        elif [ $ssdflag -eq 0 ]; then
            if [[ $sdev =~ mmcblk ]]; then
                devtype=EMMC
            elif [[ $sdev =~ sd ]]; then
                devtype=SSD
            else
                ret=1
            fi
        elif [ $ssdflag -eq 1 ]; then
            if [[ $sdev =~ sd ]]; then
            #usb storage will error
                devtype=HDD
            else
                ret=1
            fi
        else
            kecho unknow type ssdflag:$ssdflag
            ret=1
        fi
    fi
    kecho ssdflag:$ssdflag ssdflag:$ssdflag ret:$ret
    [ $ret -eq 0 ] && echo $devtype
    return $ret
}

function getXXXBlockDevList()
{
    local stype=$1
    local dlist
    local devlist
    local devtype
    local ret=1
    kecho param:$*
    kecho get stype:$stype BlockDevList
    [ "$stype" = "EMMC" ] || [ "$stype" = "SSD" ] || [ "$stype" = "HDD" ] || return 1
    if [ $# -gt 1 ]; then
        shift
        dlist="$*"
        kecho stype:$stype, dlist:$dlist
    else
        dlist=`getAllBlockDevList`
        ret=$?
        kecho stype:$stype,use default dlist:$dlist
    fi
    for d in $dlist
    do
        devtype=`paramBlockDevType $d`
        ret=$?
        [ ! $ret -eq 0 ] && break
        if [ "$devtype" = "$stype" ]; then
            devlist="$devlist $d"
        elif [ -z "$devlist" ]; then
            ret=1
        fi
    done
    kecho devlist:$devlist, devtype:$devtype, ret:$ret
    [ $ret -eq 0 ] && echo $devlist
    return $ret
}

function getEMMCBlockDevList()
{
    local ret=1
    local d
    kecho param:$*
    d=`getXXXBlockDevList EMMC $*`
    ret=$?
    kecho list:$d, ret:$ret
    [ $ret -eq 0 ] && echo $d
}

function getSSDBlockDevList()
{
    local ret=1
    local d
    kecho param:$*
    d=`getXXXBlockDevList SSD $*`
    ret=$?
    kecho list:$d, ret:$ret
    [ $ret -eq 0 ] && echo $d
}

function getHDDBlockDevList()
{
    local ret=1
    local d
    kecho param:$*
    d=`getXXXBlockDevList HDD $*`
    ret=$?
    kecho list:$d, ret:$ret
    [ $ret -eq 0 ] && echo $d
}

function getAllBlockDevNum()
{
    local ret=1
    local n
    kecho param:$*
    n=(`getAllBlockDevList`)
    ret=$?
    kecho num:${#n[@]}, ret:$ret
    [ $ret -eq 0 ] && echo ${#n[@]}
}

function getEMMCBlockDevNum()
{
    local ret=1
    local n
    kecho param:$*
    n=(`getEMMCBlockDevList $*`)
    ret=$?
    kecho num:${#n[@]}, ret:$ret
    [ $ret -eq 0 ] && echo ${#n[@]}
}

function getSSDBlockDevNum()
{
    local ret=1
    local n
    kecho param:$*
    n=(`getSSDBlockDevList $*`)
    ret=$?
    kecho num:${#n[@]}, ret:$ret
    [ $ret -eq 0 ] && echo ${#n[@]}
}

function getHDDBlockDevNum()
{
    local ret=1
    local n
    kecho param:$*
    n=(`getHDDBlockDevList $*`)
    ret=$?
    kecho num:${#n[@]}, ret:$ret
    [ $ret -eq 0 ] && echo ${#n[@]}
}

function getListNum()
{
    local n
    n=($*)
    echo ${#n[@]}
}

function paramBlockDevSectorSize()
{
    local sfile=$1
    local size
    local ret=1
    kecho param:$*
    if [ -n "$sfile" ] && [ -f $sfile ]; then
# file example:
#Model: MMC BJNB4R (sd/mmc)
#Disk /dev/mmcblk0: 61071360s
#Sector size (logical/physical): 512B/512B
#Partition Table: gpt
#Disk Flags: 
#
#Number  Start      End        Size       File system     Name          Flags
# 1      2048s      976895s    974848s    fat32           RainBoot      boot, esp
# 2      976896s    4882431s   3905536s   linux-swap(v1)
# 3      4882432s   9764863s   4882432s   fat32           RainRecovery  msftdata
# 4      9764864s   23437311s  13672448s  ext4            RainOS
# 5      23437312s  61071326s  37634015s

        # get size=61071360s
        size=`echo $(cat $sfile | grep -E "^Disk " | awk -F: '{print $2}')`
        # check size unit is s or S ?
        if [ "${size:0-1:1}" = "s" -o "${size:0-1:1}" = "S" ]; then
            # 1 Sector = 512 byte
            size=$((${size:0:0-1} * 512))
            ret=0
        fi
    fi
    kecho size:$size, ret:$ret
    [ $ret -eq 0 ] && echo $size
    return $ret
}

function getBlockDevByteSize()
{
    local sfile=/tmp/parted.$$
    local devlist
    local devsize
    local devsizelist
    local ret=1
    kecho param:$*
    if [ $# -gt 0 ]; then
        devlist="$*"
    else
        devlist=`getAllBlockDevList`
        ret=$?
    fi
    for d in $*
    do
        if [ -b /dev/$d ]; then
            d=/dev/$d
            LC_ALL=C parted -s $d unit s print > $sfile 2>/dev/null
            devsize=`paramBlockDevSectorSize $sfile`
            ret=$?
            [ $ret -eq 0 ] && devsizelist="$devsizelist $devsize"
        else
            ret=1
        fi
        [ ! $ret -eq 0 ] && break
    done
    kecho devsizelist:$devsizelist, ret:$ret
    [ $ret -eq 0 ] && echo $devsizelist
    [ -f $sfile ] && rm $sfile
    return $ret
}

function getImageDiskByteSize()
{
    local imagepath=$1
    local size
    local ret=1
    kecho param:$*
    [ ! -d $imagepath ] && return 1
    # the file $imagepath/*-pt.parted of image is saved the image disk size.
    for d in `ls $imagepath/*-pt.parted 2>/dev/null`
    do
        size=`paramBlockDevSectorSize $d`
        ret=$?
        if [ $ret -eq 0 ]; then
            break
        fi
    done
    kecho size:$size, ret:$ret
    [ $ret -eq 0 ] && echo $size
    return $ret
}

function getSizeGreaterThenXX()
{
    local ret=1
    local gtsize=$1
    local devlist
    local sizetmp
    local gtdevlist
    kecho param:$*
    shift
    devlist=$*
    [ -z "$gtsize" ] && return $ret

    for d in $devlist
    do
        sizetmp=`getBlockDevByteSize $d`
        ret=$?
        kecho dev:$d, sizetmp:$sizetmp ret:$ret
        [ ! $ret -eq 0 ] && return $ret
        if [ $sizetmp -ge $gtsize ]; then
            gtdevlist="$gtdevlist $d"
        fi
    done
    kecho gtdevlist:$gtdevlist, ret:$ret
    [ $ret -eq 0 -a -n "$gtdevlist" ] && echo $gtdevlist
    return $ret
}

function getRainOSDevList()
{
    local ret=1
    local sdev=$*
    local partn
    local dlist
    local partsize
    # the RainOS system disk partation size
    local RainOSpartsize=(500 2000 2500 7000)
    local sizediff
    local tmpret
    kecho param:$*

    [ -z "$sdev" ] && return $ret
    for d in $sdev
    do
        if [[ $d =~ mmcblk ]]; then
            partn=p4
        else
            partn=4
        fi
        # if disk's PARTLABEL contain RainOS or disk parts size fit RainOSpartsize, then the disk is a RainOSDev
        if [ -b /dev/${d}${partn} ]; then
            if blkid /dev/${d}${partn} | grep 'PARTLABEL="RainOS' > /dev/null ; then
                dlist="$dlist $d"
                ret=0
                continue
            fi
            if [ ! -b /dev/${d} ]; then
                continue
            fi
            if blkid /dev/${d} | grep 'PTTYPE="gpt"' > /dev/null ; then
                partsize=(`parted -s /dev/${d} unit MB print | grep -A4 "^Number" | grep "^ [1-4]" | awk '{print $4}' | sed s/MB//g`)
                tmpret=$?
                for i in `seq 0 $((${#RainOSpartsize[@]}-1))`
                do
                    sizediff=$(( ${RainOSpartsize[$i]} - ${partsize[$i]} ))
                    # we only allow the part size diff between -5 ~ 5 MB
                    if [ $sizediff -lt -5 -o $sizediff -gt 5 ]; then
                        kecho RainOSpartsize:${RainOSpartsize[$i]}, partsize:${partsize[$i]}, failed to match
                        tmpret=1
                        break
                    fi
                done
                if [ $tmpret = 0 ]; then
                    dlist="$dlist $d"
                    ret=0
                fi
            fi
        fi
    done
[ -n "$gtest_sysdev" ] && [[ "$sdev" =~ "$gtest_sysdev" ]] && echo $gtest_sysdev && return 0
    kecho dlist:$dlist, ret:$ret
    [ $ret -eq 0 ] && echo $dlist
    return $ret
}

# param:
# newsysdevlist: choose disk list that can install rainos. return the disk list.
# oldsysdev: find the rainos disk (has installed rainos) from all disk. return the rainos disk.
# backingdev: get bcking device on the 500HD machine, return the hdd disk.
function getOprationDisk()
{
    local ret=1
    local mode
    local devlist
    local devnum
    local devemmclist
    local devemmcnum
    local sysdevlist
    local sysdevnum
    local dev90Glist
    local dev90Gnum
    local tmp_ocs_sr_dev

    kecho param:$*
    mode=$1
    if [ ! "$mode" = "newsysdevlist" ] && [ ! "$mode" = "oldsysdev" ] && [ ! "$mode" = "backingdev" ]; then
        kecho !!!Unknow mode parm:$mode!!!
        return 1
    fi
    shift

    devlist=(`getAllBlockDevList`)
    ret=$?

    devnum=${#devlist[@]}
    kecho devlist=${devlist[@]}, devnum=$devnum
    if [ $devnum -eq 0 ]; then
        kecho !!!Can not find any block device!!!
        return 201
    fi

    devemmclist=(`getEMMCBlockDevList ${devlist[@]}`)
    devemmcnum=${#devemmclist[@]}
    if [ ! $devemmcnum -eq 0 ] ;then
        kecho devemmclist=${devemmclist[@]}, devemmcnum=$devemmcnum
        if [ $devnum -eq 2 ]; then
            if [[ "${devlist[0]}" =~ "mmcblk" ]]; then
                dev90Glist=(`getSizeGreaterThenXX $g_size_request ${devlist[1]}`)
            else
                dev90Glist=(`getSizeGreaterThenXX $g_size_request ${devlist[0]}`)
            fi
            if [ ${#dev90Glist[@]} -eq 1 ]; then
                if [ "$mode" = "oldsysdev" ]; then
                    # oldsysdev will return rainos disk, if another rainos exist, need return failed.
                    # newsysdevlist operation on 500HD machine, we do not care of hdd has rainos, hdd will format when install rainos.
                    sysdevlist=(`getRainOSDevList $dev90Glist`)
                    if [ ! ${#sysdevlist[@]} -eq 0 ]; then
                        kecho !!!Backing device ${sysdevlist[@]} is RainOS disk!!!
                        return 202
                    fi
                fi
                if [ "$mode" = "backingdev" ]; then
                    echo $dev90Glist
                    kecho ======dev90Glist:$dev90Glist======
                    return 0
                else
                    tmp_ocs_sr_dev=$devemmclist
                    echo $tmp_ocs_sr_dev
                    kecho ======tmp_ocs_sr_dev=$tmp_ocs_sr_dev======
                    return 0
                fi
            else
                kecho !!!Baching device is small than ${g_sizeG_request}G!!!
                return 203
            fi
        elif [ $devnum -eq 1 ]; then
            kecho !!!Can not find backing device!!!
            return 204
        elif [ $devnum -gt 2 ]; then
            kecho !!!Found $devnum block device!!!
            return 205
        fi
        kecho !!!Unknow error!!!
        return 1
    fi

    if [ "$mode" = "backingdev" ]; then
        kecho !!!Do not need backing device, is not a BCACHE machine!!!
        return 1
    fi

    sysdevlist=(`getRainOSDevList ${devlist[@]}`)
    sysdevnum=${#sysdevlist[@]}

    if [ $sysdevnum -eq 1 ]; then
        dev90Glist=(`getSizeGreaterThenXX $g_size_request $sysdevlist`)
        if [ ${#dev90Glist[@]} -eq 1 ]; then
            tmp_ocs_sr_dev=$sysdevlist
            echo $tmp_ocs_sr_dev
            kecho ======tmp_ocs_sr_dev=$tmp_ocs_sr_dev======
            return 0
        fi
        kecho !!!RainOS disk $sysdevlist is small than ${g_sizeG_request}G!!!
        return 206
    elif [ $sysdevnum -gt 1 ]; then
        kecho !!!Found $sysdevnum RainOS disk \(${sysdevlist[@]}\), only one RainOS disk allowed, remove other RainOS disk!!!
        return 207
    fi

    if [ "$mode" = "oldsysdev" ]; then
        kecho !!!Can not find any RainOS Disk!!!
        return 208
    fi

    dev90Glist=(`getSizeGreaterThenXX $g_size_request ${devlist[@]}`)
    dev90Gnum=${#dev90Glist[@]}

    if [ $dev90Gnum -eq 0 ]; then
        kecho !!!Can not find any disk as RainOS disk, all disk \(${devlist[@]}\) is small than ${g_sizeG_request}G!!!
        return 209
#    elif [ $dev90Gnum -eq 1 ]; then
#        tmp_ocs_sr_dev=$dev90Glist
#        echo $tmp_ocs_sr_dev
#        kecho ======tmp_ocs_sr_dev=$tmp_ocs_sr_dev======
#        return 0
#    elif [ $dev90Gnum -gt 1 ]; then
    elif [ $dev90Gnum -ge 1 ]; then
        tmp_ocs_sr_dev=${dev90Glist[@]}
        echo $tmp_ocs_sr_dev
        return 0
    fi
    kecho !!!Unknow error!!!
    return 1
}

function isDataDiskDev()
{
    local sdev=$1
    local ret=1

    kecho param:$*

    if [ -z "$sdev" ] || [ ! -b $sdev ]; then
        kecho can not find block dev.
        return 1
    fi

    devpartn=`ls ${sdev}[0-9] 2>/dev/null | wc -l`

    if [ ! "$devpartn" -eq 1 ]; then
        kecho find unknow part of $sdev
        return 1
    fi

    devp=${sdev}1

    blkid $devp | grep 'TYPE="xfs"' >/dev/null 2>&1
    ret=$?
    if [ ! $ret -eq 0 ];then
        kecho unknow fstype of $devp
        return 1
    fi

    xfs_admin -l $devp | grep "label" | grep "DataDisk" >/dev/null 2>&1
    ret=$?
    if [ $ret -eq 0 ];then
        kecho $devp is a Rain Data Disk
        return 0
    fi
    kecho $devp is not a Rain Data Disk
    return $ret
}

function getDevModeInfo()
{
    local ret=1
    local dev=/dev/$1
    local devmode

    if [ -n "$dev" ] && [ -b $dev ]; then
        devmode=`hdparm -i $dev | grep Model | awk -F, '{print $1}' | awk -F= '{print $2}'`
        ret=$?
    fi
    [ $ret -eq 0 ] && echo $devmode
    return $ret
}

function getDevSnInfo()
{
    local ret=1
    local dev=/dev/$1
    local devsn

    if [ -n "$dev" ] && [ -b $dev ]; then
        devsn=`hdparm -i $dev | grep SerialNo | awk -F, '{print $3}' | awk -F= '{print $2}'`
        ret=$?
    fi
    [ $ret -eq 0 ] && echo $devsn
    return $ret
}

function getDevPerUsage()
{
    local devp=$1
    local ret=1
    local perofusage

    kecho param:$*

    if [ -z "$devp" ] || [ ! -b $devp ]; then
        kecho can not find block dev.
        return 1
    fi

    if ! mount | grep "^${devp}\s" >/dev/null; then
        mkdir /tmp/.dir.$$ >/dev/null 2>&1
        if ! mount ${devp} /tmp/.dir.$$ >/dev/null 2>&1; then
            kecho mount ${devp} failed
            return 1
        fi
        sync
        perofusage=`df -h ${devp} | grep ${devp} | awk '{print $5}'`
        ret=$?
        umount /tmp/.dir.$$
        rm -rf /tmp/.dir.$$
    else
        perofusage=`df -h ${devp} | grep ${devp} | awk '{print $5}'`
        ret=$?
    fi
    [ $ret -eq 0 ] && echo $perofusage
    return $ret
}

function getDevReadyStatus()
{
    local sdev=/dev/$1
    local devfstype
    local perofusage

    kecho param:$*
    if [ -z "$sdev" ] || [ ! -b $sdev ]; then
        kecho can not find $sdev block.
        return 1
    fi

    isDataDiskDev $sdev
    ret=$?
    
    if [ ! $ret -eq 0 ]; then
        kecho $sdev is not a rain disk
        return 1
    fi

    perofusage=`getDevPerUsage ${sdev}1`
    ret=$?

    if [ $ret -eq 0 ]; then
        echo $perofusage
        kecho $sdev is ready
    fi
    return $ret
}

function getDevBaseInfo()
{
    local ret=1
    local devname
    local devmode
    local devsn
    local devtype
    local devsize
    local devready
    local perofusage

    devname=$1
    if [ ! -b /dev/$devname ]; then
        kecho dev name error!
        return 1
    fi
    [[ ! "$devname" =~ "mmcblk" ]] && devmode=`getDevModeInfo $devname`
    [[ ! "$devname" =~ "mmcblk" ]] && devsn=`getDevSnInfo $devname`
    devtype=`paramBlockDevType $devname`
    devsize=`getBlockDevByteSize $devname`
    perofusage=`getDevReadyStatus $devname`
    if [ $? -eq 0 ]; then
        devready=ready
    else
        devready=unknow
    fi
    echo "{name:$devname; vendor:$devmode; sn:$devsn; type:$devtype; size:$devsize; status:$devready; perofusage=$perofusage}"
}

function stripStr2FromStr1()
{
    local str1=$1
    local str2=$2

    for s in $str2
    do
        str1=`echo $str1 | sed s/$s//`
    done
    echo $str1
}

function dogetDataDiskInfoList()
{
    local devlist
    local sysdev
    local datadisk

    devlist=`getAllBlockDevList`
    if [ $? -eq 0 ]; then
        sysdev=`getOprationDisk oldsysdev`
        if [ $? -eq 0 ]; then
            if [[ ! "$sysdev" =~ "mmcblk" ]]; then
                # del the system disk from disk device list.
                datadisk=`stripStr2FromStr1 "$devlist" "$sysdev"`
                for d in $datadisk
                do
                    getDevBaseInfo $d
                done
                return 0
            else
                kecho Bcache machine.
            fi
        else
            kecho Failed to get system device.
        fi
    else
        kecho Failed to get all block device.
    fi
    return 1
}

function formatDataDiskDev()
{
    local devp=${1}1
    local ret=1

    kecho param:$*

    if [ ! -b $devp ]; then
        kecho can not find block dev.
        return 1
    fi

    mkfs.xfs -f $devp
    ret=$?
    if [ ! $ret -eq 0 ]; then
        kecho mkfs.xfs -f $devp failed:$ret
        return $ret
    fi
    xfs_admin -L DataDisk $devp
    ret=$?
    if [ ! $ret -eq 0 ]; then
        kecho xfs_admin -L DataDisk $devp failed:$ret
        return $ret
    fi

    return 0
}

function createDataDiskDev()
{
    local sdev=$1
    local cnt

    if [ -z "$sdev" ] || [ ! -b $sdev ]; then
        kecho can not find dev:$sdev
        return 1
    fi
# fdisk cmd required param.
    cat > /tmp/part.l << EOF
g
p
n



p
w
EOF
    fdisk $sdev < /tmp/part.l
    ret=$?
    rm -f /tmp/part.l
    if [ ! $ret -eq 0 ]; then
        kecho parted $sdev failed:$ret.
        return $ret
    fi

    devp=${sdev}1

    sync

    cnt=50
    while [ ! -b ${devp} ]
    do
        cnt=$(($cnt-1))
        sleep 0.1
        if [ $cnt -lt 1 ]; then
            kecho timeout to wait ${devp}.
            return 1
        fi
    done

    return 0
}

function doformatPersonalDisk()
{
    local sdev=/dev/$1
    local ret=1

    kecho param:$*

    if [ -z "$sdev" ] || [ ! -b $sdev ]; then
        kecho can not find block dev:$sdev.
        return 1
    fi

    if ! isDataDiskDev $sdev ;then
        createDataDiskDev $sdev
        ret=$?
        if [ ! $ret -eq 0 ];then
            kecho createDataDiskDev $sdev failed.
            return $ret
        fi
    fi

    formatDataDiskDev $sdev 
    ret=$? 
    if [ ! $ret -eq 0 ];then
        kecho formatDataDiskDev $sdev failed.
        return $ret
    fi
    return 0
}

function domountPart()
{
    local dev="$1"
    local mountpoint="$2"
    local options="$3"
    kecho param:$*
    if [ -z "$dev" ] || [ -z "$mountpoint" ]; then
        kecho invalue param.
        return 1
    fi
    mount $options "$dev" "$mountpoint"
    return $?
}

function doumountPart()
{
    local mountpoint="$1"
    kecho param:$*
    if [ -z "$mountpoint" ]; then
        kecho invalue param.
        return 1
    fi
    if mount | grep "\s${mountpoint}\s" >/dev/null; then
        umount "$mountpoint"
        return $?
    else
        kecho $mountpoint is not mounted.
        return 0
    fi
}

function domountDataDisk()
{
    local dev="$1"
    local mountpoint="$2"
    local options="$3"
    local ret=1

    kecho param:$*
    if [ -z "$dev" ] || [ -z "$mountpoint" ]; then
        kecho invalue param.
        return 1
    fi
    dev=/dev/$dev
    if ! isDataDiskDev $dev; then
        kecho Unknow dev:$dev
        return 1
    fi
    if [[ ! "$mountpoint" =~ "/vm/disk" ]]; then
        kecho Unknow mountpoint:$mountpoint
        return 1
    fi
    domountPart "${dev}1" "$mountpoint" "$options"
    return $?
}

function doumountDataDisk()
{
    local mountpoint="$1"

    kecho param:$*

    if [ -z "$mountpoint" ]; then
        kecho invalue param.
        return 1
    fi

    if [[ ! "$mountpoint" =~ "/vm/disk" ]]; then
        kecho Unknow mountpoint:$mountpoint
        return 1
    fi
    doumountPart "$mountpoint"
    return $?
}

