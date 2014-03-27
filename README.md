Leveldb-Android
===============

Port leveldb to Android

======================

[![Build Status](https://drone.io/github.com/googolmo/Leveldb-Android/status.png)](https://drone.io/github.com/googolmo/Leveldb-Android/latest)

###Build
1. Configure Project
```bash
./configure.sh
```

2. Build jni and copy so file
```bash
./build.sh
```

3. Build Project
```bash
./gradlew build
```

### How to import

* aar (may be can't work)

```groovy
dependencies {
    compile 'im.amomo.leveldb:leveldb:1.0.0@aar'
}
```

* remote jar and local so

```groovy
dependencies {
    compile 'im.amomo.leveldb:leveldb:1.0.0@jar'
}
```

save [armeabi/libleveldbjni.so](https://raw.githubusercontent.com/googolmo/Leveldb-Android/master/lib/src/main/jniLibs/armeabi/libleveldbjni.so) to jniLibs/armeabi/libleveldbjni.so
save [armeabi-v7a/libleveldbjni.so](https://raw.githubusercontent.com/googolmo/Leveldb-Android/master/lib/src/main/jniLibs/armeabi-v7a/libleveldbjni.so) to jniLibs/armeabi-v7a/libleveldbjni.so

* local jar and local so

save [jar](https://drone.io/github.com/googolmo/Leveldb-Android/files/lib/build/libs/lib-1.0.0.jar) to libs/

### Usage (TODO)

