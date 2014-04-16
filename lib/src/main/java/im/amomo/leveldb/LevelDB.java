package im.amomo.leveldb;

import android.text.TextUtils;
import android.util.Log;

import java.io.File;

/**
 * Created by GoogolMo on 3/13/14.
 */
public class LevelDB {


    private final String mPath;

    public LevelDB(File path) throws LevelDBException {
        if (path == null) {
            throw new NullPointerException();
        }
        mPath = path.getAbsolutePath();
        nativeOpen(mPath);
    }

    public LevelDB(String path) throws LevelDBException {
        if (TextUtils.isEmpty(path)) {
            throw new NullPointerException();
        }
        mPath = path;
        nativeOpen(mPath);
    }


    public void close() {
        nativeClose();
    }

    public void put(String key, byte[] value) throws LevelDBException {
        if (TextUtils.isEmpty(key)) {
            throw new NullPointerException("key can not be null");
        }
        if (value == null) {
            throw new NullPointerException("value can not be null");
        }

        nativePut(key, value);

    }

    public void put(String key, String value) throws LevelDBException {
        if (TextUtils.isEmpty(key)) {
            throw new NullPointerException("key can not be null");
        }

        if (value == null) {
            throw new NullPointerException("value can not be null");
        }

        nativePutString(key, value);
    }

    public void put(String key, long value) throws LevelDBException {
        if (TextUtils.isEmpty(key)) {
            throw new NullPointerException("key can not be null");
        }

        nativePutLong(key, value);
    }

    public void put(String key, int value) throws LevelDBException {
        if (TextUtils.isEmpty(key)) {
            throw new NullPointerException("key can not be null");
        }

        nativePutInt(key, value);
    }

    public void put(String key, short value) throws LevelDBException {
        if (TextUtils.isEmpty(key)) {
            throw new NullPointerException("key can not be null");
        }

        nativePutShort(key, value);
    }

    public void put(String key, double value) throws LevelDBException {
        if (TextUtils.isEmpty(key)) {
            throw new NullPointerException("key can not be null");
        }

        nativePutDouble(key, value);
    }

    public void put(String key, float value) throws LevelDBException {
        if (TextUtils.isEmpty(key)) {
            throw new NullPointerException("key can not be null");
        }

        nativePutFloat(key, value);
    }

    public void put(String key, boolean value) throws LevelDBException {
        if (TextUtils.isEmpty(key)) {
            throw new NullPointerException("key can not be null");
        }
        nativePutBoolean(key, value);
    }

    public byte[] get(String key) throws LevelDBException {
        if (TextUtils.isEmpty(key)) {
            throw new NullPointerException("key can not be null");
        }
        if (nativeExists(key)) {
            return nativeGet(key);
        }
        return null;
    }

    public String getString(String key) throws LevelDBException {
        if (TextUtils.isEmpty(key)) {
            throw new NullPointerException("key can not be null");
        }
        if (nativeExists(key)) {
            return nativeGetString(key);
        }
        return null;
    }

    public long getLong(String key) throws LevelDBException {
        if (TextUtils.isEmpty(key)) {
            throw new NullPointerException("key can not be null");
        }
        return nativeGetLong(key);
    }

    public int getInt(String key) throws LevelDBException {
        if (TextUtils.isEmpty(key)) {
            throw new NullPointerException("key can not be null");
        }
        return nativeGetInt(key);
    }

    public short getShort(String key) throws LevelDBException {
        if (TextUtils.isEmpty(key)) {
            throw new NullPointerException("key can not be null");
        }
        return nativeGetShort(key);
    }

    public double getDouble(String key) throws LevelDBException {
        if (TextUtils.isEmpty(key)) {
            throw new NullPointerException("key can not be null");
        }
        return nativeGetDouble(key);
    }

    public float getFloat(String key) throws LevelDBException {
        if (TextUtils.isEmpty(key)) {
            throw new NullPointerException("key can not be null");
        }
        return nativeGetFloat(key);
    }

    public boolean getBoolean(String key) throws LevelDBException {
        if (TextUtils.isEmpty(key)) {
            throw new NullPointerException("key can not be null");
        }
        return nativeGetBoolean(key);
    }

    public void delete(String key) throws LevelDBException {
        if (TextUtils.isEmpty(key)) {
            throw new NullPointerException("key can not be null");
        }

        nativeDelete(key);
    }

    public void destroy() throws LevelDBException{
        destroy(mPath);
    }

    public boolean exists(String key) throws LevelDBException {
        if (TextUtils.isEmpty(key)) {
            throw new NullPointerException("key can not be null");
        }
        return nativeExists(key);
    }

    public static void destroy(String path) throws LevelDBException{
        if (TextUtils.isEmpty(path)) {
           throw new NullPointerException("path can not be null");
        }
        nativeDestroy(path);
    }

    private native void nativeOpen(String dbpath);

    private native void nativeClose();

    private native void nativePut(String key, byte[] value);

    private native void nativePutString(String key, String value);

    private native void nativePutLong(String key, long value);

    private native void nativePutInt(String key, int value);

    private native void nativePutShort(String key, short value);

    private native void nativePutDouble(String key, double value);

    private native void nativePutFloat(String key, float value);

    private native void nativePutBoolean(String key, boolean value);

    private native byte[] nativeGet(String key);

    private native String nativeGetString(String key);

    private native long nativeGetLong(String key);

    private native int nativeGetInt(String key);

    private native short nativeGetShort(String key);

    private native double nativeGetDouble(String key);

    private native float nativeGetFloat(String key);

    private native boolean nativeGetBoolean(String key);

    private native void nativeDelete(String key);

    private native boolean nativeExists(String key);

    private static native void nativeDestroy(String dbpath);

        static {
            Log.d(LevelDB.class.getSimpleName(), "loadjni");
            System.loadLibrary("leveldbjni");
        }

}
