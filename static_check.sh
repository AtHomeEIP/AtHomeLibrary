#!/bin/bash

cd src/
cppcheck -I $PWD --enable=all --check-config --force \
         --suppress=missingIncludeSystem $(find . -name "*.cpp") \
         *.hpp *.h
cd -
