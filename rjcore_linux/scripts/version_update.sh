#!/bin/bash
#

curdirname=$(basename $PWD)
if [ "$curdirname" = "scripts" ]; then
    cd ..
fi

TOPDIR=$PWD
VERSION_FILE=$TOPDIR/version/version_rainsys.ini
VERSION_RELEASE=R0

if [ -n "$1" ]; then
    if [ "$1" = "revert" ]; then
        git checkout -- $VERSION_FILE
        exit 0
    fi
    if echo $1 | grep -q '\.'; then
        mainVersion=`echo $1 | awk -F. '{print $1}'`
        minorVersion=`echo $1 | awk -F. '{print $2}'`
        patchVersion=`echo $1 | awk -F. '{print $3}'`
    else
        mainVersion=`cat $VERSION_FILE | awk -F= '/mainVersion/{print $2}' | tr -d '\r'`
        minorVersion=`cat $VERSION_FILE | awk -F= '/minorVersion/{print $2}' | tr -d '\r'`
        patchVersion=$1
    fi
else
    mainVersion=`cat $VERSION_FILE | awk -F= '/mainVersion/{print $2}' | tr -d '\r'`
    minorVersion=`cat $VERSION_FILE | awk -F= '/minorVersion/{print $2}' | tr -d '\r'`
    patchVersion=`cat $VERSION_FILE | awk -F= '/fourVersion/{print $2}' | tr -d '\r'`
fi
if [ -n "$2" ]; then
    release_ver=$2
    VERSION_RELEASE=${release_ver##*_}
fi
buildDate=$(date +%F)
HEADcommit=$(git log -1 | awk '/^commit/ {print $2}')
curBranch=$(git branch | awk '/^\*/ {print $2}')

cat > $VERSION_FILE.$$ << EOF
[LINUX-rcd.rainsys]
ruijie.product=rcd.rainsys
ruijie.rcd.rainsys.mainVersion=$mainVersion
ruijie.rcd.rainsys.minorVersion=$minorVersion
ruijie.rcd.rainsys.threeVersion=0
ruijie.rcd.rainsys.extra1=_$VERSION_RELEASE
ruijie.rcd.rainsys.fourVersion=$patchVersion
ruijie.rcd.rainsys.extra2=
ruijie.rcd.rainsys.buildDate=$buildDate
ruijie.rcd.rainsys.buildHEAD=$HEADcommit
ruijie.rcd.rainsys.buildBranch=$curBranch
EOF

mv $VERSION_FILE.$$ $VERSION_FILE
cat $VERSION_FILE
