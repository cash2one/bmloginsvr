#!/bin/bash

set -x

cd `dirname $0`
cd ../../

export GOPATH=$(pwd)
go_path=$(pwd)
cd src/lua
src_dir=$(pwd)

# delete old version
if	[ -f "./$.*.c" ];	then
	rm	./$.*.c
fi

#copy files
cd ../../../../cpp/lua/lua/src

for files in *.c
do
echo "#include \"../../../../cpp/lua/lua/src/${files}\"" > ${src_dir}/$.$files
done

cd $src_dir

# clear up
rm	./$.lua.c
rm	./$.luac.c
