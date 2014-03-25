#!/usr/bin/env bash

ndk-build -C lib/src/main/jni-src
cp -rvf lib/src/main/jni-src/libs/ lib/src/main/jniLibs/

./gradlew build installDebug
