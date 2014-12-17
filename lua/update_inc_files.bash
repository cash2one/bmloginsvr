#!/bin/bash

set -x

cd `dirname $0`
cd ../../

export GOPATH=$(pwd)
go_path=$(pwd)
cd src/lua
src_dir=$(pwd)

ITEMS="luaconf lua lualib lauxlib "

for	item	in	$ITEMS
do
	if	[ -f "../../inc/lua/${item}.h" ];	then
		rm	../../inc/lua/${item}.h
	fi
	if	[ -f "../../../../cpp/lua/lua/src/${item}.h" ];	then
		cp	../../../../cpp/lua/lua/src/${item}.h	../../inc/lua/
	fi
done
