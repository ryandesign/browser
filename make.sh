#!/bin/bash

# SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
#
# SPDX-License-Identifier: MIT

set -euo pipefail

if [[ ${BASH_SOURCE[0]} = */* ]]; then
    cd -- "${BASH_SOURCE%/*}/" || exit
fi
sourcedir="$PWD"

: "${RETRO68:=/opt/local/libexec/Retro68}"
: "${RESINFO:="$RETRO68"/bin/ResInfo}"
: "${REZ:="$RETRO68"/bin/Rez}"
: "${CMAKE:=cmake}"
: "${CMAKE_BUILD_TYPE:=RelWithDebInfo}"
: "${BUILD_DIR:=build}"
: "${BUILD_TYPE_DIR:="$BUILD_DIR/$CMAKE_BUILD_TYPE"}"
: "${CMAKE_BUILD_PARALLEL_LEVEL:=$(sysctl -n hw.activecpu 2>/dev/null || nproc 2>/dev/null || echo 1)}"
: "${JOBS:="$CMAKE_BUILD_PARALLEL_LEVEL"}"
: "${TERM:=dumb}"

build_68k=0
build_ppc=0
build_fat=0

while (($# > 0)); do
    case $1 in
        68k)
            build_68k=1
            ;;
        ppc)
            build_ppc=1
            ;;
        fat)
            build_fat=1
            ;;
        *)
            printf 'unknown option %s\n' "$1" >&2
            exit 1
    esac
    shift
done

if ((!build_68k && !build_ppc && !build_fat)); then
    build_fat=1
fi
if ((build_fat)); then
    build_68k=1
    build_ppc=1
fi

build_dir_68k="$BUILD_TYPE_DIR/m68k"
build_dir_ppc="$BUILD_TYPE_DIR/powerpc"
build_dir_fat="$BUILD_TYPE_DIR/fat"

common_cmake_flags=(
    -S "$sourcedir"
    -DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE"
    -DCMAKE_VERBOSE_MAKEFILE=ON
)

heading() {
    local -r msg="${1:-}"
    local -r msglen=${#msg}
    local -r cols=$(tput cols)
    local -r leftlen=$(((cols-msglen)/2-1))
    local -r rightlen=$((leftlen+msglen%2))
    local i
    for ((i=0; i<leftlen; ++i)) {
        printf '='
    }
    printf ' %s ' "$msg"
    for ((i=0; i<rightlen; ++i)) {
        printf '='
    }
    printf '\n'
}

if ((build_68k)); then
    heading 'Building for 68K'

    mkdir -p "$build_dir_68k"

    "$CMAKE" \
    -DCMAKE_TOOLCHAIN_FILE="$RETRO68"/m68k-apple-macos/cmake/retro68.toolchain.cmake \
    -B "$build_dir_68k" \
    "${common_cmake_flags[@]}"

    "$CMAKE" \
    --build "$build_dir_68k" \
    -- -j"$JOBS"
fi

if ((build_ppc)); then
    heading 'Building for PowerPC'

    mkdir -p "$build_dir_ppc"

    "$CMAKE" \
    -DCMAKE_TOOLCHAIN_FILE="$RETRO68"/powerpc-apple-macos/cmake/retroppc.toolchain.cmake \
    -B "$build_dir_ppc" \
    "${common_cmake_flags[@]}"

    "$CMAKE" \
    --build "$build_dir_ppc" \
    -- -j"$JOBS"
fi

make_fat() {
    local -r name="$1"
    local -r step_number="${2:-1}"
    local -r total_steps="${3:-1}"
    local -r type="$("$RESINFO" --type "$build_dir_68k/src/$name.bin" | sed -E "s/^'(.{4,})'$/\1/")"
    local -r creator="$("$RESINFO" --creator "$build_dir_68k/src/$name.bin" | sed -E "s/^'(.{4,})'$/\1/")"
    set -x
    "$REZ" \
    --define CFRAG_NAME="\"$name\"" \
    --copy "$build_dir_68k/src/$name.bin" \
    --data "$build_dir_ppc/src/$name.pef" \
    --output "$build_dir_fat/src/$name.bin" \
    --cc "$build_dir_fat/src/%$name.ad" \
    --cc "$build_dir_fat/src/$name.APPL" \
    --cc "$build_dir_fat/src/$name.dsk" \
    --type "$type" \
    --creator "$creator" \
    --include "$RETRO68"/RIncludes \
    "$RETRO68"/RIncludes/RetroPPCAPPL.r || return $?
    { set +x; } 2>/dev/null
    printf '[%3d%%] Created fat binary for %s\n' "$((100*step_number/total_steps))" "$name"
}

if ((build_fat)); then
    heading 'Building fat binary'

    mkdir -p "$build_dir_fat/src"

    make_fat 'app'
fi
