package im.amomo.leveldb;

import android.content.Context;
import android.test.AndroidTestCase;

/**
 * Created by GoogolMo on 3/26/14.
 */
public class TestDB extends AndroidTestCase {
    private static final String DB_NAME = "test_db";

    protected LevelDB db;

    public void testOpenDB() throws Exception {
        db = DBFactory.open(getContext(), DB_NAME);
        assertNotNull(db);
        db.close();
    }

    public void testByteArray() throws Exception {
        db = DBFactory.open(getContext(), DB_NAME);
        db.put("test_bytes", "test_byte".getBytes());
        assertEquals(new String(db.get("test_bytes")), "test_byte");
        db.close();
    }

    public void testString() throws Exception {
        db = DBFactory.open(getContext(), DB_NAME);
        db.put("test_string", "test_string");
        assertEquals(db.getString("test_string"), "test_string");
        db.close();
    }

    public void testLong() throws Exception {
        db = DBFactory.open(getContext(), DB_NAME);
        db.put("test_long", 123456l);
        assertEquals(db.getLong("test_long"), 123456l);
        db.close();
    }

    public void testShort() throws Exception {
        db = DBFactory.open(getContext(), DB_NAME);
        db.put("test_short", Short.MIN_VALUE);
        assertEquals(db.getShort("test_short"), Short.MIN_VALUE);
        db.close();
    }

    public void testInt() throws Exception {
        db = DBFactory.open(getContext(), DB_NAME);
        db.put("test_int", Integer.MAX_VALUE);
        assertEquals(db.getInt("test_int"), Integer.MAX_VALUE);
        db.close();
    }

    public void testDouble() throws Exception {
        db = DBFactory.open(getContext(), DB_NAME);
        db.put("test_double", 1.12345d);
        System.out.println(db.getDouble("test_double"));
        assertEquals(db.getDouble("test_double"), 1.12345d);
        db.close();
    }

    public void testFloat() throws Exception {
        db = DBFactory.open(getContext(), DB_NAME);
        db.put("test_float", 1.12345f);
        assertEquals(db.getFloat("test_float"), 1.12345f);
        db.close();
    }

    public void testBoolean() throws Exception {
        db = DBFactory.open(getContext(), DB_NAME);
        db.put("test_boolean", true);
        assertEquals(db.getBoolean("test_boolean"), true);
        db.close();
    }

    public void test1000() throws Exception {
        db = DBFactory.open(getContext(), DB_NAME);
        for (int i = 0; i < 100; i ++) {
            db.put("test_" + i, i * 100);
        }
        assertNotNull(db);
        db.close();
    }

    public void testExists() throws Exception {
        db = DBFactory.open(getContext(), DB_NAME);
        db.put("key_exists", "<(=￣▽￣=)>");
        assertTrue(db.exists("key_exists"));
        db.close();
    }

    public void testDelete() throws Exception {
        db = DBFactory.open(getContext(), DB_NAME);
        db.put("key_delete", "<(=￣▽￣=)>");
        db.delete("key_delete");
        assertFalse(db.exists("key_delete"));
        db.close();
    }

    public void testDestroy() throws Exception {
        db = DBFactory.open(getContext(), DB_NAME);
        db.close();
        db.destroy();
    }


    @Override
    protected void setUp() throws Exception {
        super.setUp();
    }
}
