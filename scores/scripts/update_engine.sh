#! /bin/bash
#
# update_engine.sh
#
# Copyright © 2018 Mathieu Gaborit (matael) <mathieu@matael.org>
#
# Licensed under the "THE BEER-WARE LICENSE" (Revision 42):
# Mathieu (matael) Gaborit wrote this file. As long as you retain this notice you
# can do whatever you want with this stuff. If we meet some day, and you think
# this stuff is worth it, you can buy me a beer or coffee in return
#
# 

source set_env.sh

cd $MBNT_BINDIR
git pull --autostash --rebase
make clean
make
