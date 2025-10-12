#!/bin/bash

clear

git status
git add .

#git checkout -b main

#git tag -s v0.1.30 -m "v0.1.30"

git status

#exit

git commit -s -m "feature: add ALL"
git push origin main
# feature/${NAME}

exit

git pull origin main

