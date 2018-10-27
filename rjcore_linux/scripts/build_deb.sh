#!/bin/bash
#

# building process num, for make processes.
BUILD_PROCESSES=48

curdirname=$(basename $PWD)
if [ "$curdirname" = "scripts" ]; then
    cd ..
fi

cp -f scripts/local.mak.deb interfaces/local.mak
cp -f scripts/Makefile.deb interfaces/Makefile

# clear old outputs.
rm -f *.deb
rm -f backup-*.tgz

# We start IDV client building with project top dir.
TOPDIR=$PWD
pkgName=rjcore-linux

while [ $# -ne 0 ] 
do
    case "$1" in
        --version)
            issue_ver=$2
            shift 2
            ISSUEVER=$issue_ver
            ;; 
        --release)
            release_ver=$2
            shift 2
            RELEASE=${release_ver##*_}
            ;; 
        *) 
            echo "[*] Error: Unknown argument \"$1\""
            exit 1
            ;; 
    esac
done

#VERSION_FILE=$TOPDIR/version/version_rainsys.ini
#mainVersion=`cat $VERSION_FILE | awk -F= '/mainVersion/{print $2}' | tr -d '\r'`
#minorVersion=`cat $VERSION_FILE | awk -F= '/minorVersion/{print $2}' | tr -d '\r'`
#patchVersion=`cat $VERSION_FILE | awk -F= '/fourVersion/{print $2}' | tr -d '\r'`
#releaseVersion=`cat $VERSION_FILE | awk -F= '/extra1/{print $2}' | tr -d '\r'`
#pkgVersion=$mainVersion.$minorVersion.$patchVersion
#pkgRelease=${releaseVersion##*_}
pkgVersion=${ISSUEVER}
pkgRelease=${RELEASE}

echo "EXTRA_CFLAGS := -DIDV -DVDI -DVERSION=${pkgVersion}" > platform.mak
echo "LIB_NAME = sysrjcore_linux" >> platform.mak
echo "JAR_NAME = sysrjcore_linux" >> platform.mak
echo "LIB_VERSION = ${pkgVersion}" >> platform.mak

# do compiling.
make clean
make -j$BUILD_PROCESSES $@
if [ $? -ne 0 ]; then
    echo
    echo "Error occured while compiling. Stop!"
    echo
    exit 1
fi

# make a package.
sudo make uninstall
sudo checkinstall --pkgname=$pkgName --pkgversion=$pkgVersion --pkgrelease=$pkgRelease -y --install=no --strip=no
cp ${pkgName}*.deb ${pkgName}.deb

# re-package the rcd.deb as an uncompressed package.
dpkg -X ${pkgName}.deb ${pkgName}_tmp
echo
pushd ${pkgName}_tmp
dpkg -e $TOPDIR/${pkgName}.deb
if [ -d $TOPDIR/DEBIAN/ ]; then
    cp -rf $TOPDIR/DEBIAN/ .
    #echo "Depends: wpasupplicant (>=2.6-1)" >> ./DEBIAN/control
    chmod +x DEBIAN/*
fi
popd
rm -f ${pkgName}.deb
dpkg-deb -b -Znone ${pkgName}_tmp ${pkgName}.deb
rm -rf ${pkgName}_tmp
echo
echo "$pkgName packaging OK! find output: ${pkgName}.deb"
echo
