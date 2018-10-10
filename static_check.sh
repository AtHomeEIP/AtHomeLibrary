#!/bin/bash

cppcheck -I $PWD --enable=all --check-config --force \
         --suppress=missingIncludeSystem $(find src/ -name "*.cpp") \
         src/*.hpp src/*.h
