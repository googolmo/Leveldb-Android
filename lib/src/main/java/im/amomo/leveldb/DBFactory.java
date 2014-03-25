package im.amomo.leveldb;

import android.content.Context;
import android.os.Environment;
import android.util.Log;

import java.io.File;

/**
 * Created by GoogolMo on 3/25/14.
 */
public class DBFactory {

    private final static String DEFAULT_DATABASE_NAME = "leveldb";

    public static LevelDB open(File dbPath) throws LevelDBException {
        return new LevelDB(dbPath);
    }

    public static LevelDB open(Context context, String dbName) throws LevelDBException {
        File dbPath;
        try {
            dbPath = context.getExternalFilesDir(null);
        } catch (Exception e) {
            dbPath = context.getFilesDir();
        }
        return open(new File(dbPath, dbName));
    }

    public static LevelDB open(Context context) throws LevelDBException {
        return open(context, DEFAULT_DATABASE_NAME);
    }


}
