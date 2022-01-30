#! /bin/bash

SSHAPID=$(eval $(ssh-agent) | awk '{ print $3 }')

echo $SSHAPID

ssh-add /home/pi/.ssh/id_github
