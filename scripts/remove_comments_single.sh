#!/bin/bash

OLDF=$1
NEWDIR=$2

mkdir -p $(dirname $NEWDIR/$OLDF)

#gcc -fpreprocessed -dD -E $1 > $1.tmp
/home/riveale/git/scc/scc $OLDF > $NEWDIR/$OLDF
