#!/bin/bash

set -euo pipefail

if [[ ${BASH_SOURCE[0]} = */* ]]; then
    cd -- "${BASH_SOURCE%/*}/" || exit
fi

./make.sh "$@" && ./run.sh "$@"
