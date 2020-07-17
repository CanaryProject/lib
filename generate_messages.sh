#!/usr/bin/env bash
BUILD="Off"

while getopts "b" opt; do
  case "$opt" in
    b) BUILD="On" ;;
  esac
done

if [ ${BUILD} == "On" ]; then
  ./canary_echo.sh "Building flatbuffers dependencies"
  cd external/flatbuffers
  cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
  make
  cd ../..
fi

./canary_echo.sh "Generating flatbuffers messages"
./external/flatbuffers/flatc -c -o src/protocol/messages/ src/flatbuffers/*
./canary_echo.sh "Finished"