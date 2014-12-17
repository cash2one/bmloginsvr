#!/bin/bash

set -x

cd `dirname $0`
cd ../../

export GOPATH=$(pwd)
go_path=$(pwd)
cd src/lua
src_dir=$(pwd)

chmod +x ./update_c_code.bash
./update_c_code.bash
chmod +x ./update_inc_files.bash
./update_inc_files.bash

#install
go install
