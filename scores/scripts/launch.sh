#!/bin/bash

source set_env.sh

id=$1

run_file_path="$MBNT_URL/be/t:$MBNT_TOKEN/runfile/$id"
run_file=$(curl -s $run_file_path)

IFS=$'\n'
for l in $run_file; do
 [[ $l == '' ]] && continue
 echo line : $l
 # do GNU PARALLEL MAGIC
done 
