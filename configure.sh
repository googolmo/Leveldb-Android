#!/usr/bin/env bash

git submodule init
git submodule update
sh lib/src/main/jni-src/jni/snappy/autogen.sh
