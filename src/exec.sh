#!/bin/bash
qmake ke.pro
make
echo "---------------------------------------------------------------------------------------------------------"
#rm log.txt
#./bin/KE 2> log.txt
#cat log.txt|grep -v --before-context=1 --after-context=1 mouseMoveEvent

./bin/KE 3>&1 >&2 2>&3 3>&- |tee log.txt

