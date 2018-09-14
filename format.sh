#!/bin/bash

function format {
  clang-format -i --style=google $@
}

cd src
format $(find . -name "*.c")
format $(find . -name "*.cpp")
format $(find . -name "*.h")
format $(find . -name "*.hpp")
