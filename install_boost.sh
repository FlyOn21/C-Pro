#!/bin/bash

set -e

INSTALL_DIR="/usr/local/boost"

INSTALL=false
for arg in "$@"
do
    case $arg in
        --install)
        INSTALL=true
        shift
        ;;
        --prefix=*)
        INSTALL_DIR="${arg#*=}"
        shift
        ;;
        *)
        echo "Unknown option: $arg"
        exit 1
        ;;
    esac
done
cd ./external/boost
chmod +x ./bootstrap.sh
./bootstrap.sh
./b2 headers
./b2 --with-program_options --with-filesystem stage -j4
if $INSTALL; then
    echo "Installing Boost to $INSTALL_DIR..."
    ./b2 install --prefix="$INSTALL_DIR"
    echo "Boost installed in $INSTALL_DIR"
else
    echo "Boost built but not installed. Use --install to install it."
fi
cd ../..