#!/bin/bash

#This will attempt to configure the system variables. 
echo trying to determine variables. if it is unsuccessful you will need to set all of the variables listed below.
pamheader="`find /usr/ -name pam_appl.h`"
if [ ! -f "$pamheader" ] ; then echo "Pam auth module pam_app or pam-dev libpam-dev needs to be installed in a /usr folder"; fi
pamheader="`find /usr/ -name pam_misc.h`"
if [ ! -f "$pamheader" ] ; then echo "pam_misc header and so needs to be installed"; fi
export INCLUDE="$(dirname "${pamheader}")"
if [ `echo $INCLUDE|grep -ic "security"` -gt 0 ] ; then export INCLUDE="`echo $INCLUDE|sed 's/security//g'`" ; fi
SO_DIR="`find /usr/ -name pam_access.so`"
export SO_DIR="$(dirname "$SO_DIR")"
if [ -f "$SO_DIR" ]; then export SO_DIR="`echo $SO_DIR|sed 's/security//g'`"; fi
export USER="`whoami`"
export CPU_ARCH="`lscpu|grep Architecture|sed "s/Architecture://g"|tr -d [:space:]'\n'`"
echo Include directory INCLUDE: $INCLUDE
echo Shared Object directory SO_DIR: $SO_DIR
echo CPU architecture CPU_ARCH: $CPU_ARCH
echo User USER:  $USER
if [ "$CPU_ARCH" == "x86_64" ] ; then export CPU_ARCH="x86-64"; fi 
echo CPU atchitecture CPU_ARCH: $CPU_ARCH
#this are the directores on ubuntu 
#export INCLUDE='/usr/include/'
#export SO_DIR='/usr/lib/x86_64-linux-gnu/'
export DEF_COMPILE_OPTIONS="-Wall -Wextra -pedantic -fsanitize=address -fsanitize=undefined -o3 --std=c++20"
echo Default Compile Options DEF_COMPILE_OPTIONS: $DEF_COMPILE_OPTIONS
echo if the variables are not set for the make file you may have to source ./configure.sh so it can can set environment variables. 
