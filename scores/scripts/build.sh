#! /bin/bash

source set_env.sh
mkdir $MBNT_HOME 2> /dev/null || echo "$MBNT_HOME already present, skipping."

repo_file_url="$MBNT_URL/be/t:$MBNT_TOKEN/repofile"
repo_file=$(curl -s $repo_file_url)

cd $MBNT_HOME
echo "Removing all in $MBNT_HOME ? [N/y]"
read ans

if [[ "$ans" = "y" || "$ans" = "Y" ]]; then
	echo "rm -r $MBNT_HOME/*"
else
	echo  "Right... but you are no fun. Exiting."
	exit 0
fi

IFS=$'\n'
for l in $repo_file; do
	[[ $l == '' ]] && continue

	team_id=$(echo $l | cut -d ' ' -f 1)
	team_repo=$(echo $l | cut -d ' ' -f 2)

	echo "Processing team #$team_id w/ repo $team_repo"

	git clone "$team_repo" "$team_id"
	cd "$team_id"
	./build.sh || "Exiting."

	cd "$MBNT_HOME"
done

echo "Finished building"
