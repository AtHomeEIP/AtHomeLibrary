#!/bin/bash

function format {
  clang-format -i --style=google $@
}

cd src
format *.h *.hpp time/* storage/* sensor/* sensor/temperature/* \
       sensor/sound/* sensor/luminosity/* sensor/humidity/* \
       sensor/air_quality/* power/* module/* display/* communication/* \
       communication/network/* communication/network/wifi/*

