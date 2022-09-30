#!/bin/bash

# SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
#
# SPDX-License-Identifier: MIT

set -euo pipefail

if [[ ${BASH_SOURCE[0]} = */* ]]; then
    cd -- "${BASH_SOURCE%/*}/" || exit
fi

: "${RETRO68:=/opt/local/libexec/Retro68}"
: "${CMAKE_BUILD_TYPE:=RelWithDebInfo}"
: "${BUILD_DIR:=build}"
: "${BUILD_TYPE_DIR:="$BUILD_DIR/$CMAKE_BUILD_TYPE"}"

arch=fat
while (($# > 0)); do
    case $1 in
        68k|fat|ppc)
            arch="$1"
            ;;
        *)
            printf 'unknown option %s\n' "$1" >&2
            exit 1
    esac
    shift
done

set -x

build_dir_68k="$BUILD_TYPE_DIR/m68k"
build_dir_ppc="$BUILD_TYPE_DIR/powerpc"
build_dir_fat="$BUILD_TYPE_DIR/fat"

build_dir_var="build_dir_$arch"

"$RETRO68"/bin/LaunchAPPL "${!build_dir_var}"/src/app.bin
