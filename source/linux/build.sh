#!/bin/bash
BASEDIR="$(dirname "$(readlink -e "${BASH_SOURCE}")")/../.."

set -euxo pipefail

BUILD_QAPPLE=""
if [ "${INSTALL_DEPS:-}" = "QT" ] ; then
    BUILD_QAPPLE="-DBUILD_QAPPLE=1"
    apt-get update
    apt-get install -y --no-install-recommends qtbase5-dev qtmultimedia5-dev libqt5gamepad5-dev
fi

if [ ! -z "${INSTALL_DEPS:-}" ] ; then
    apt-get update
    apt-get install -y --no-install-recommends cmake make gcc g++ libyaml-dev libminizip-dev libboost-program-options-dev libncurses-dev libevdev-dev libsdl2-dev libsdl2-image-dev libgl-dev libpcap-dev
    apt-get install -y --no-install-recommends libslirp-dev || echo "Skipping libslirp"
fi

cd "$BASEDIR"
mkdir -p build/packages
cd build

cmake "$@" -DBUILD_APPLEN=1 $BUILD_QAPPLE -DBUILD_SA2=1 -DCMAKE_BUILD_TYPE=RELEASE "$BASEDIR"
make -j $(nproc)
cpack -G DEB -B "$BASEDIR/build/packages"
