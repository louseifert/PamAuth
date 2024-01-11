cur-dir:=$(shell pwd)
whoami:=$(shell whoami)
host-type:=$(shell arch)
opts:="-Wall -Wextra -pedantic -fsanitize=address -fsanitize=undefined -o3 --std=c++20"
include-dir:="$(INCLUDE)/security/"
so-dir:="$(SO_DIR)/security/"
main-file=main.cpp
target-file:=pamauthtest
arch:=$(CPU_ARCH)

archguess:=$(shell g++ -Q --help=target|grep -e '-march' |grep -iv Known| awk '{print $2}'|tr -d [:space:] ) 
check:
	clear
	@if [ ! -d "${so-dir}" ]; then echo "set so_dir to your pam install location read the readme"; exit 1; else echo "found shared objects dir ${so-dir}"; fi;
	@if [ ! -d "${include-dir}" ]; then echo "set include_dir to your include/security directory, read the readme"; exit 1; else echo "found include dir ${include-dir}"; fi 
	@if [ -z "${arch}" ] ; then echo "CPU_ARCH not set format is export CPU_ARCH=  try one of the following: nocona core2 nehalem corei7 westmere sandybridge corei7-avx ivybridge core-avx-i haswell core-avx2 broadwell skylake skylake-avx512 cannonlake icelake-client rocketlake icelake-server cascadelake tigerlake cooperlake sapphirerapids alderlake bonnell atom silvermont slm goldmont goldmont-plus tremont knl knm x86-64 x86-64-v2 x86-64-v3 x86-64-v4 eden-x2 nano nano-1000 nano-2000 nano-3000 nano-x2 eden-x4 nano-x4 k8 k8-sse3 opteron opteron-sse3 athlon64 athlon64-sse3 athlon-fx amdfam10 barcelona bdver1 bdver2 bdver3 bdver4 znver1 znver2 znver3 btver1 btver2 native ${arch}" ; exit 1;else echo ${arch} ; fi;
	g++ $(DEF_COMPILE_OPTIONS) -I./include -I${include_dir} -L${so-dir}  ${main-file}  -march=${arch} -lpam -lpam_misc -o ${target-file}

make: check 

syscheck:
	echo currentdir: ${cur-dir} whoami: ${whoami} hosttype:${host-type} archguess:${archguess}
test:
	g++ $(DEF_COMPILE_OPTIONS) -I./include/ -I$(INCLUDE) -L${so-dir} -L/usr/lib pamauth_test.cpp -lgtest -lpam  -lpam_misc -o unit-test1
	g++ $(DEF_COMPILE_OPTIONS) -I./include/ -I$(INCLUDE) -L${so-dir} -L/usr/lib include/str2ucstr_test.cpp -lgtest -lpam  -lpam_misc -o unit-test2
	if [ -f ./unit-test1 ] ; then ./unit-test1 < infile; rm unit-test1; fi;
	if [ -f ./unit-test2 ] ; then ./unit-test2; rm unit-test2; fi
	
test1:
	g++ $(DEF_COMPILE_OPTIONS) -I./include/ -I$(INCLUDE) -L${so-dir} -L/usr/lib pamauth_test1.cpp -lgtest -lpam  -lpam_misc -o unit-test3
	if [ -f ./unit-test3 ] ; then ./unit-test3; rm unit-test3; fi

fulltest: test test1

clean:
	if [ -f ./unit-test1 ]; then rm unit-test1; fi
	if [ -f ./unit-test2 ]; then rm unit-test2; fi
	if [ -f ./unit-test3 ]; then rm unit-test3; fi
	if [ -f ./pamauthtest ]; then rm pamauthtest; fi
