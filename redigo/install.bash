#!/bin/bash

set -x

cd `dirname $0`
cd ../../

export GOPATH=$(pwd)
go_path=$(pwd)
cd src/redigo
src_dir=$(pwd)

#install
go install

#subitem
for files in *
do

if [ -d $files ]
then

#enter sub directory
cd $files
#install
echo .		$files
go install

cd $src_dir

fi

done
