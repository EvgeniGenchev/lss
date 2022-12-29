#!/bin/sh

if [ "$(id -u)" -ne 0 ]; then
	echo "You should run \"install.sh\" as 'root'"
	exit 1
fi

gcc main.c -o lss
cp lss /usr/bin/.
