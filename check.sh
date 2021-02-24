#!/bin/bash
HERE=$(cd $(dirname $BASH_SOURCE) && pwd)
cd $HERE

# VARS of DIRs
export BASE_DIR=$HERE
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HERE/Cbuild/lib:$HERE/Cbuild/lib/HCNetSDKCom

python3 main.py