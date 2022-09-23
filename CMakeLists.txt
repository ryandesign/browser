# 3.18 needed for string(HEX ...)
cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(hellolite)

option(USE_LITEHTML "Use litehtml" ON)
add_subdirectory(third_party)
add_subdirectory(src)
