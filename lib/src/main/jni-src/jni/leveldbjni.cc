#include "leveldbjni.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int register_im_amomo_leveldb_LevelDB(JNIEnv *env);

leveldb::DB* db;
bool isDBOpen;
char* dbPath;

jint throwException(JNIEnv* env, const char* msg) {
    const char* exceptionClass = "im/amomo/leveldb/LevelDBException";

    jclass clazz = env->FindClass(exceptionClass);
    if (!clazz) {
        LOGE("Can't find exception class %s", exceptionClass);
        return -1;
    }

    return env->ThrowNew(clazz, msg);
}

void releaseDB() {
    delete db;
    isDBOpen = false;
    free(dbPath);
    dbPath = NULL;
}

int JNI_OnLoad(JavaVM* vm, void *reserved) {
    JNIEnv* env = NULL;
    LOGI("jni_load");
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    releaseDB();

    register_im_amomo_leveldb_LevelDB(env);

    return JNI_VERSION_1_6;
}

void JNI_OnUnload(JavaVM* vm, void* reserved) {

    delete db;
    isDBOpen = false;
    free(dbPath);
    dbPath = NULL;
}

void nativeOpen(JNIEnv* env, jclass clazz, jstring dbpath) {
    const char *path = env->GetStringUTFChars(dbpath, 0);
    LOGI("Opening database %s", path);

    if (isDBOpen) {
        if (NULL != dbPath && 0 != strcmp(dbPath, path)) {
            throwException(env, "Your database is still open, please close it before");
        } else {
            LOGI("database was already open %s", path);
        }

        env->ReleaseStringUTFChars(dbpath, path);
    }
    leveldb::Options options;
    options.create_if_missing = true;
    options.compression = leveldb::kSnappyCompression;
    leveldb::Status status = leveldb::DB::Open(options, path, &db);

    if (!status.ok()) {
        LOGE("Failed to open database");
        releaseDB();
        std::string err("Failed to open/create database:" + status.ToString());
        env->ReleaseStringUTFChars(dbpath, path);
        throwException(env, err.c_str());
    } else {
        isDBOpen = true;
        if ((dbPath = strdup(path)) != NULL) {
            env->ReleaseStringUTFChars(dbpath, path);
            LOGI("Opened databse");
        } else {
            env->ReleaseStringUTFChars(dbpath, path);
            throwException(env, "OutOfMemory when saving the database name");
        }
    }
}

void nativeClose(JNIEnv* env, jclass clazz) {
    LOGI("Closing database %s", dbPath);
    if (isDBOpen) {
        releaseDB();
    } else {
        throwException(env, "Database was already closed");
    }
}

static void nativeDestroy(JNIEnv* env,
                          jclass clazz,
                          jstring dbpath) {
    const char* path = env->GetStringUTFChars(dbpath, 0);

    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = DestroyDB(path, options);

    env->ReleaseStringUTFChars(dbpath, path);

    if (!status.ok()) {
        std::string err("Failed to destroy database: " + status.ToString());
        throwException(env, err.c_str());
    }
}

void nativeDelete(JNIEnv* env, jclass clazz, jstring jkey) {

    if (!isDBOpen) {
        throwException(env, "Database is not open");
        return;
    }

    const char* key = env->GetStringUTFChars(jkey, 0);

    leveldb::Status status = db->Delete(leveldb::WriteOptions(), key);

    env->ReleaseStringUTFChars(jkey, key);

    if (!status.ok()) {
        std::string err("Failed to delete: " + status.ToString());
        throwException(env, err.c_str());
    }
}


void nativePutString(JNIEnv* env, jclass clazz, jstring jkey, jstring jval) {

    LOGI("Putting a string");

    if (!isDBOpen) {
        throwException(env, "Database is not open");
        return;
    }

    const char* key = env->GetStringUTFChars(jkey, 0);
    const char* value = env->GetStringUTFChars(jval, 0);

    leveldb::Status status = db->Put(leveldb::WriteOptions(), key, value);

    env->ReleaseStringUTFChars(jval, value);
    env->ReleaseStringUTFChars(jkey, key);

    if (!status.ok()) {
        std::string err("Failed to put a String: " + status.ToString());
        throwException(env, err.c_str());
    }
}

void nativePut(JNIEnv* env, jclass clazz, jstring jkey, jbyteArray arr) {
    LOGI("Putting a Serializable");
    if (!isDBOpen) {
        throwException(env, "Database is not open");
        return;
    }

    int len = env->GetArrayLength(arr);
    jbyte* data = (jbyte*)env->GetPrimitiveArrayCritical(arr, 0);

    if (data == NULL) {
        throwException(env, "OutOfMemory when trying to get bytes array for Serializable");
        return;
    }

    const char* key = env->GetStringUTFChars(jkey, 0);
    leveldb::Slice value = leveldb::Slice(reinterpret_cast<char*>(data), len);

    leveldb::Status status = db->Put(leveldb::WriteOptions(), key, value);

    env->ReleasePrimitiveArrayCritical(arr, data, 0);
    env->ReleaseStringUTFChars(jkey, key);

    if (!status.ok()) {
        std::string err("Failed to put a Serializable: " + status.ToString());
        throwException(env, err.c_str());
    }

}

void nativePutLong(JNIEnv* env, jclass clazz, jstring jkey, jlong jval) {
    LOGI("Putting a long ");

    if (!isDBOpen) {
        throwException(env, "Database is not open");
        return;
    }

    const char* key = env->GetStringUTFChars(jkey, 0);
    leveldb::Slice value = leveldb::Slice((char*) &jval, sizeof(jlong));

    leveldb::Status status = db->Put(leveldb::WriteOptions(), key, value);

    env->ReleaseStringUTFChars(jkey, key);

    if (!status.ok()) {
        std::string err("Failed to put a long: " + status.ToString());
        throwException(env, err.c_str());
    }
}

void nativePutInt(JNIEnv* env, jclass clazz, jstring jkey, jint jval) {
    LOGI("Putting an int");

    if (!isDBOpen) {
        throwException(env, "Database is not open");
        return;
    }

    const char* key = env->GetStringUTFChars(jkey, 0);
    leveldb::Slice value = leveldb::Slice ((char*)& jval, sizeof(jint));

    leveldb::Status status = db->Put(leveldb::WriteOptions(), key, value);

    env->ReleaseStringUTFChars(jkey, key);

    if (!status.ok()) {
        std::string err("Failed to put an int: " + status.ToString());
        throwException(env, err.c_str());
    }
}

void nativePutShort(JNIEnv* env, jclass clazz, jstring jkey, jshort jval) {
    LOGI("Putting an short");

    if (!isDBOpen) {
        throwException(env, "Database is not open");
        return;
    }

    const char* key = env->GetStringUTFChars(jkey, 0);
    leveldb::Slice value = leveldb::Slice ((char*)& jval, sizeof(jshort));

    leveldb::Status status = db->Put(leveldb::WriteOptions(), key, value);

    env->ReleaseStringUTFChars(jkey, key);

    if (!status.ok()) {
        std::string err("Failed to put an short: " + status.ToString());
        throwException(env, err.c_str());
    }
}

void nativePutBoolean(JNIEnv* env, jclass clazz, jstring jkey, jboolean jval) {
    LOGI("Putting an boolean");

    if (!isDBOpen) {
        throwException(env, "Database is not open");
        return;
    }

    const char* key = env->GetStringUTFChars(jkey, 0);
    leveldb::Slice value = leveldb::Slice ((char*)& jval, sizeof(jboolean));

    leveldb::Status status = db->Put(leveldb::WriteOptions(), key, value);

    env->ReleaseStringUTFChars(jkey, key);

    if (!status.ok()) {
        std::string err("Failed to put an boolean: " + status.ToString());
        throwException(env, err.c_str());
    }
}

void nativePutDouble(JNIEnv* env, jclass clazz, jstring jkey, jdouble jval) {
    LOGI("Putting an double");

    if (!isDBOpen) {
        throwException(env, "Database is not open");
        return;
    }

    const char* key = env->GetStringUTFChars(jkey, 0);
    leveldb::Slice value = leveldb::Slice ((char*)& jval, sizeof(jdouble));

    leveldb::Status status = db->Put(leveldb::WriteOptions(), key, value);

    env->ReleaseStringUTFChars(jkey, key);

    if (!status.ok()) {
        std::string err("Failed to put an double: " + status.ToString());
        throwException(env, err.c_str());
    }
}

void nativePutFloat(JNIEnv* env, jclass clazz, jstring jkey, jfloat jval) {
    LOGI("Putting an float");

    if (!isDBOpen) {
        throwException(env, "Database is not open");
        return;
    }

    const char* key = env->GetStringUTFChars(jkey, 0);
    leveldb::Slice value = leveldb::Slice ((char*)& jval, sizeof(jfloat));

    leveldb::Status status = db->Put(leveldb::WriteOptions(), key, value);

    env->ReleaseStringUTFChars(jkey, key);

    if (!status.ok()) {
        std::string err("Failed to put an float: " + status.ToString());
        throwException(env, err.c_str());
    }
}

jstring nativeGetString(JNIEnv* env, jclass clazz, jstring jkey) {
    LOGI("Getting a String");

    if (!isDBOpen) {
        throwException(env, "Database is not open");
        return NULL;
    }

    const char* key = env->GetStringUTFChars(jkey, 0);
    std::string value;
    leveldb::Status status = db->Get(leveldb::ReadOptions(), key, &value);

    env->ReleaseStringUTFChars(jkey, key);

    if (status.ok()) {
        return env->NewStringUTF(value.c_str());
    } else {
        std::string err("Failed to get a String: " + status.ToString());
        throwException(env, err.c_str());
        return NULL;
    }
}

jlong nativeGetLong(JNIEnv* env, jclass clazz, jstring jkey) {
    LOGI("Getting a Long");

    if (!isDBOpen) {
        throwException(env, "Database is not open");
    }

    const char* key = env->GetStringUTFChars(jkey, 0);
    std::string data;
    leveldb::Status status = db->Get(leveldb::ReadOptions(), key, &data);

    env->ReleaseStringUTFChars(jkey, key);

    if (status.ok()) {
        if (8 == data.length()) {
            const char* bytes = data.data();
            long ret = 0;

            ret = bytes[7];
            ret = (ret << 8) + (unsigned char)bytes[6];
            ret = (ret << 8) + (unsigned char)bytes[5];
            ret = (ret << 8) + (unsigned char)bytes[4];
            ret = (ret << 8) + (unsigned char)bytes[3];
            ret = (ret << 8) + (unsigned char)bytes[2];
            ret = (ret << 8) + (unsigned char)bytes[1];
            ret = (ret << 8) + (unsigned char)bytes[0];
            return ret;
        } else {
            throwException(env, "Failed to get a Long");
        }
    } else {
        std::string err("Failed to get a Long: " + status.ToString());
        throwException(env, err.c_str());
    }
}

jint nativeGetInt(JNIEnv* env, jclass clazz, jstring jkey) {
    LOGI("Getting a Int");

    if (!isDBOpen) {
        throwException(env, "Database is not open");
    }

    const char* key = env->GetStringUTFChars(jkey, 0);
    std::string data;
    leveldb::Status status = db->Get(leveldb::ReadOptions(), key, &data);

    env->ReleaseStringUTFChars(jkey, key);

    if (status.ok()) {
        if (4 == data.length()) {
            const char* bytes = data.data();
            long ret = 0;

            ret = bytes[3];
            ret = (ret << 8) + (unsigned char)bytes[2];
            ret = (ret << 8) + (unsigned char)bytes[1];
            ret = (ret << 8) + (unsigned char)bytes[0];
            return ret;
        } else {
            throwException(env, "Failed to get a Int");
        }
    } else {
        std::string err("Failed to get a Int: " + status.ToString());
        throwException(env, err.c_str());
    }
}

jdouble nativeGetDouble(JNIEnv* env, jclass clazz, jstring jkey) {
    LOGI("Getting a Double");

    if (!isDBOpen) {
        throwException(env, "Database is not open");
    }

    const char* key = env->GetStringUTFChars(jkey, 0);
    std::string data;
    leveldb::Status status = db->Get(leveldb::ReadOptions(), key, &data);

    env->ReleaseStringUTFChars(jkey, key);

    if (status.ok()) {
        double d = atof(data.c_str());
        return d;
    } else {
        std::string err("Failed to get a Double: " + status.ToString());
        throwException(env, err.c_str());
    }
}

jfloat nativeGetFloat(JNIEnv* env, jclass clazz, jstring jkey) {
    LOGI("Getting a Float");

    if (!isDBOpen) {
        throwException(env, "Database is not open");
    }

    const char* key = env->GetStringUTFChars(jkey, 0);
    std::string data;
    leveldb::Status status = db->Get(leveldb::ReadOptions(), key, &data);

    env->ReleaseStringUTFChars(jkey, key);

    if (status.ok()) {
        float f = atof(data.c_str());
        return f;
    } else {
        std::string err("Failed to get a Short: " + status.ToString());
        throwException(env, err.c_str());
    }
}

jshort nativeGetShort(JNIEnv* env, jclass clazz, jstring jkey) {
    LOGI("Getting a Short");

    if (!isDBOpen) {
        throwException(env, "Database is not open");
    }

    const char* key = env->GetStringUTFChars(jkey, 0);
    std::string data;
    leveldb::Status status = db->Get(leveldb::ReadOptions(), key, &data);

    env->ReleaseStringUTFChars(jkey, key);

    if (status.ok()) {
        if (2 == data.length()) {
            const char* bytes = data.data();
            short ret = 0;
            ret = bytes[1];
            ret = (ret << 8) + bytes[0];
            return ret;
        } else {
            throwException(env, "Failed to get a Short");
        }
    } else {
        std::string err("Failed to get a Short: " + status.ToString());
        throwException(env, err.c_str());
    }
}

jboolean nativeGetBoolean(JNIEnv* env, jclass clazz, jstring jkey) {
    LOGI("Getting a Boolean");

    if (!isDBOpen) {
        throwException(env, "Database is not open");
    }

    const char* key = env->GetStringUTFChars(jkey, 0);
    std::string data;
    leveldb::Status status = db->Get(leveldb::ReadOptions(), key, &data);

    env->ReleaseStringUTFChars(jkey, key);

    if (status.ok()) {
        if (1 == data.length()) {
            return data.data()[0];
        } else {
            throwException(env, "Failed to get a Boolean");
        }
    } else {
        std::string err("Failed to get a Boolean: " + status.ToString());
        throwException(env, err.c_str());
    }
}

jbyteArray nativeGet(JNIEnv* env, jclass clazz, jstring jkey) {
    LOGI("Getting a ByteArray");

    if (!isDBOpen) {
        throwException(env, "Database is not open");
    }

    const char* key = env->GetStringUTFChars(jkey, 0);
    std::string data;
    leveldb::Status status = db->Get(leveldb::ReadOptions(), key, &data);

    env->ReleaseStringUTFChars(jkey, key);

    if (status.ok()) {
        int size = data.size();
        char* elems = const_cast<char*>(data.data());
        jbyteArray array = env->NewByteArray(size * sizeof(jbyte));
        env->SetByteArrayRegion(array, 0, size, reinterpret_cast<jbyte*>(elems));
        return array;
    } else {
        std::string err("Failed to get a ByteArray: " + status.ToString());
        throwException(env, err.c_str());
    }
}

jboolean nativeExists(JNIEnv* env, jclass clazz, jstring jkey) {
    if (!isDBOpen) {
        throwException(env, "Database is not open");
    }

    const char* key = env->GetStringUTFChars(jkey, 0);
    std::string value;
    leveldb::Status status = db->Get(leveldb::ReadOptions(), key, &value);

    env->ReleaseStringUTFChars(jkey, key);

    if (status.ok()) {
        return JNI_TRUE;
    } else if (status.IsNotFound()) {
        return JNI_FALSE;
    } else {
        std::string err("Failed to check if a key exists: " + status.ToString());
        throwException(env, err.c_str());
    }
}

static JNINativeMethod sMethods[] = {
    { "nativeOpen", "(Ljava/lang/String;)V", (void*) nativeOpen},
    { "nativeClose", "()V", (void*) nativeClose},
    { "nativeGet", "(Ljava/lang/String;)[B", (void*) nativeGet},
    { "nativeGetString", "(Ljava/lang/String;)Ljava/lang/String;", (void*) nativeGetString},
    { "nativeGetLong", "(Ljava/lang/String;)J", (void*) nativeGetLong},
    { "nativeGetInt", "(Ljava/lang/String;)I", (void*) nativeGetInt},
    { "nativeGetShort", "(Ljava/lang/String;)S", (void*) nativeGetShort},
    { "nativeGetDouble", "(Ljava/lang/String;)D", (void*) nativeGetDouble},
    { "nativeGetFloat", "(Ljava/lang/String;)F", (void*) nativeGetFloat},
    { "nativeGetBoolean", "(Ljava/lang/String;)Z", (void*) nativeGetBoolean},
    { "nativePut", "(Ljava/lang/String;[B)V", (void*) nativePut},
    { "nativePutString", "(Ljava/lang/String;Ljava/lang/String;)V", (void*) nativePutString},
    { "nativePutLong", "(Ljava/lang/String;J)V", (void*) nativePutLong},
    { "nativePutInt", "(Ljava/lang/String;I)V", (void*) nativePutInt},
    { "nativePutShort", "(Ljava/lang/String;S)V", (void*) nativePutShort},
    { "nativePutDouble", "(Ljava/lang/String;D)V", (void*) nativePutDouble},
    { "nativePutFloat", "(Ljava/lang/String;F)V", (void*) nativePutFloat},
    { "nativePutBoolean", "(Ljava/lang/String;Z)V", (void*) nativePutBoolean},
    { "nativeDelete", "(Ljava/lang/String;)V", (void*) nativeDelete },
    { "nativeExists", "(Ljava/lang/String;)Z", (void*) nativeExists },
    { "nativeDestroy", "(Ljava/lang/String;)V", (void*) nativeDestroy }
};

int register_im_amomo_leveldb_LevelDB(JNIEnv *env) {
    jclass clazz = env->FindClass("im/amomo/leveldb/LevelDB");
    LOGI("register_im_amomo_leveldb_LevelDB");
    if (!clazz) {
        LOGE("Can't find class im.amomo.leveldb.LevelDB");
        return 0;
    }
    return env->RegisterNatives(clazz, sMethods, NELEM(sMethods));
}


