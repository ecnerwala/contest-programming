#!/bin/bash

DIRECTORY=$( cd "$( dirname $0 )" && pwd )
PYTHON="/usr/bin/python2.7"

$PYTHON $DIRECTORY/dcj.py $@
