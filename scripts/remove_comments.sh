#!/bin/bash

TOPDIR=$1
NEWDIR=$2

find $TOPDIR -maxdepth 3 -type f -exec bash scripts/remove_comments_single.sh {} $NEWDIR \;
