#!/bin/sh
#SRC_RT=$(shell pwd)
SRC_RT=/mnt/d/360MoveData/Users/asus/Desktop/os2020-labs/lab3/src
echo $SRC_RT

make clean

make

if [ $? -ne 0 ]; then
	echo "make failed"
else
	echo "make succeed"
	qemu-system-i386 -kernel output/myOS.elf -serial pty &
fi