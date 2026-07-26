#!/bin/zsh -l

set -e

gcc -o nob nob.c && ./nob ${@}