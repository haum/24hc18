#! /bin/bash

echo "This script will create a venv/ dir in the working dir:"
echo "$PWD"
echo ""
echo "Hit Ctrl-C now to cancel, press Return to confirm."

read dummy

virtualenv -ppython3 venv
source venv/bin/activate
pip install -r requirements.txt
