#!/usr/bin/env bash

ndk-build -C lib/src/main/jni-src

cp -rv lib/src/main/jni-src/libs/ lib/src/main/jniLibs/

