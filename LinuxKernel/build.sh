#!/bin/bash
echo "configure script is runing...";
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
if [ ! -f $DIR/netlinkKernel/*.ko ] && [ -f $DIR/netlinkKernel/*.c ];
then
    echo "step 1: compiling the netlink files...";
    (cd netlinkKernel ;make -C $DIR/netlinkKernel/); 
    echo "insert netlink module";
    insmod $DIR/netlinkKernel/*.ko >/dev/null 2>&1 || echo "module exist";
else
if [ -f $DIR/netlinkKernel/*.ko ];
then
    echo "insert netlink module";
    insmod $DIR/netlinkKernel/*.ko >/dev/null 2>&1 || echo "module exist";
else
    echo "netlinkKernel.c and netlinkKernel.ko is not exist";
    echo "exit";
    exit 1;
fi;
fi;
if [ ! -f $DIR/chardev/*.ko ] && [ -f $DIR/chardev/*.c ];
then
    echo "step 1: compiling the netlink files...";
    (cd chardev ;make -C $DIR/chardev/);
    echo "insert chardev module";
    insmod $DIR/chardev/*.ko >/dev/null 2>&1 || echo "module exist";
else
if [ -f $DIR/chardev/*.ko ];
then
    echo "insert chardev module";
    insmod $DIR/chardev/*.ko >/dev/null 2>&1 || echo "module exist";
else
    echo "chardev.c and chardev.ko is not exist";
    echo "exit";
    exit 1;
fi;
fi;
echo "mknod /dev/chardev c 246 0";
mknod /dev/chardev c 246 0 >/dev/null 2>&1 || echo "/dev/chardev file exist";
echo "compiling the userspace client file...";
gcc -o netlinkUser netlinkUser.c >/dev/null 2>&1;
echo "Done!";
