#!/bin/bash

source set_env.sh

jailcommand="timeout 5 cpulimit -l 10 -i firejail --profile=$MBNT_ABSPATH/spawner/jail.profile"

if [[ $# -lt 3 ]]; then
    echo "You must specify at least 3 args !"
    exit 1
fi

MatchID=$1
shift
Scenario=$MBNT_BINDIR/scenarii/$1
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
 cmdArgs="$cmdArgs cd '$MBNT_HOME/$TeamID; $jailcommand $startFile'"
done

OUTPUT="$($MBNT_BINDIR/marabunta_engine -s $Scenario -h localhost $cmdArgs)"
echo "${OUTPUT}"

Score=""
RunningTeam=""

IFS=$'\n'
for l in $OUTPUT; do
 [[ $l = '' ]] && continue
 if [[ $l == *"executable"* ]]; then
  RunningTeam=$(echo $l | cut -d " " -f 3 | cut -d "/" -f 4)
 fi
 if [[ $l == *"API::"* ]]; then
  Score=$(echo $l | cut -d " " -f 2)
 fi
#  if [[ $l == *"Score"* ]]; then
#   Score=$(echo $l | cut -d " " -f 2)
#  fi
 if [[ $RunningTeam -ne "" && $Score -ne "" ]]; then
  curl $MBNT_URL/be/t:$MBNT_TOKEN/register_score/$MatchID/$RunningTeam/$Score
  echo curl $MBNT_URL/be/t:$MBNT_TOKEN/register_score/$MatchID/$RunningTeam/$Score
  Score=""
  RunningTeam=""
 fi
done
