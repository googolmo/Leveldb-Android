package im.amomo.leveldb;

/**
 * Created by GoogolMo on 3/13/14.
 */
public class LevelDBException extends RuntimeException {

    public LevelDBException() {
    }

    public LevelDBException(String error) {
        super(error);
    }

    public LevelDBException(String error, Throwable cause) {
        super(error, cause);
    }
}
