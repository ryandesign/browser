# SPDX-FileCopyrightText: © 2022 Ryan C Schmidt <https://github.com/ryandesign>
#
# SPDX-License-Identifier: MIT

file(READ ${infile} data)
string(REGEX REPLACE "[ \t]*[\n\r]+[ \t]*" "\r" data "${data}")
string(REGEX REPLACE "\r$" "" data "${data}")
string(HEX "${data}" data)
string(REGEX MATCHALL "..?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?" data "${data}")
file(WRITE "${outfile}" "data '${rsrc_type}' (${rsrc_id}, purgeable) {\n")
foreach(chunk ${data})
    string(REGEX MATCHALL "..(..)?" chunk "${chunk}")
    list(JOIN chunk " " chunk)
    file(APPEND "${outfile}" "\t\$\"${chunk}\"\n")
endforeach()
file(APPEND "${outfile}" "};\n")
