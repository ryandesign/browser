#!/bin/bash

# SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
#
# SPDX-License-Identifier: MIT

set -euo pipefail

if [[ ${BASH_SOURCE[0]} = */* ]]; then
    cd -- "${BASH_SOURCE%/*}/" || exit
fi

./make.sh "$@" && ./run.sh "$@"
