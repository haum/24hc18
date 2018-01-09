#!/bin/bash

source set_env.sh

run_file_path="$MBNT_URL/be/t:$MBNT_TOKEN/run_file"
run_file=$(curl -s $run_file_path)
for l in $run_file;
do
 [[ $l == '' ]] && continue
 echo line : $l
 # do GNU PARALLEL MAGIC
done 
