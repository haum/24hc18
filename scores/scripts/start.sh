#!/bin/bash

source set_env.sh

if [[ $# -lt 3 ]]; then
    echo "You must specify at least 3 args !"
    exit 1
fi

MatchID=$1
shift
Scenario=$MBNT_SCEN_HOME/$1
shift

i=0
while [ $# -gt 0 ]
do
Teams[i]=$1
i=$((i+1))
shift
done

echo TeamsIDs are : ${Teams[@]}
NbTeams=${#Teams[@]}

echo "Launching match number #$MatchID"

cmdArgs=""

for TeamID in ${Teams[@]}
do
 startFile="$MBNT_HOME/$TeamID/start.sh"
 echo "Getting team #$TeamID's start.sh file !"
 cmdArgs="$cmdArgs $startFile"
done

$MBNT_BIN -s $Scenario -h localhost $cmdArgs
