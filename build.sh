#!/bin/bash

BUILD_CONFIG=$1
OPENRTI_VERSION=$2

# Abort on error
set -eu

source /etc/os-release
DISTRO="$ID"
DISTRO_VERSION="${VERSION_ID/./}"

cmake -DOPENRTI_ENABLE_TESTS=ON -DOPENRTI_ENABLE_RTI13=Off -DOPENRTI_ENABLE_RTI1516=Off -DCMAKE_BUILD_TYPE=${BUILD_CONFIG} -DCMAKE_INSTALL_PREFIX=install ..
make -j4 install

if [ ! -z "$OPENRTI_VERSION" ]; then	
	mkdir pkg
	mv install/bin/rtinode pkg/
	mv install/bin/fom2cpp pkg/
	test -d install/lib && mv install/lib/*.so pkg/
	test -d install/lib64 && mv install/lib64/*.so pkg/

	PKG_NAME="openrti-${OPENRTI_VERSION}-${DISTRO}${DISTRO_VERSION}-x64-${BUILD_CONFIG}.tgz"
	echo $PKG_NAME

	tar czvf ${PKG_NAME} -C pkg .
fi