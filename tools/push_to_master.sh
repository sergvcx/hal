#!/bin/bash


git checkout master
git rebase develop
version=$(<../version.txt)
git tag -a v$(version) -m "version v$(version)"
git checkout develop