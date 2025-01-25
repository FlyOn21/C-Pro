#!/bin/bash

set -e
BUILD_DIR=${1:-build}

if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"/*
else
    mkdir -p "$BUILD_DIR"
fi

cd "$BUILD_DIR"

cmake ..
cmake --build .
./TCPServer

