#!/bin/sh

source /opt/marabunta/venv/bin/activate
cd /opt/marabunta/scores
gunicorn scores.wsgi -b 0.0.0.0:8000
