package im.amomo.leveldb.sample;

import android.app.Activity;
import android.app.ActionBar;
import android.app.Fragment;
import android.content.Context;
import android.os.Bundle;
import android.os.Environment;
import android.os.SystemClock;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.os.Build;
import android.widget.TextView;
import im.amomo.leveldb.DBFactory;
import im.amomo.leveldb.LevelDB;

import java.io.File;
import java.io.IOException;


public class MainActivity extends Activity {

    protected static final String DB_KEY = "hello";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        if (savedInstanceState == null) {
            getFragmentManager().beginTransaction()
                    .add(R.id.container, new PlaceholderFragment())
                    .commit();
        }
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {

        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    /**
     * A placeholder fragment containing a simple view.
     */
    public static class PlaceholderFragment extends Fragment {
        private TextView mTv;

        public PlaceholderFragment() {
        }

        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                                 Bundle savedInstanceState) {
            View rootView = inflater.inflate(R.layout.fragment_main, container, false);
            return rootView;
        }

        @Override
        public void onViewCreated(View view, Bundle savedInstanceState) {
            super.onViewCreated(view, savedInstanceState);
            mTv = (TextView) view.findViewById(R.id.hello);
            LevelDB db = DBFactory.open(getActivity(), "hobbits1");
            long start = SystemClock.elapsedRealtime();
            String value = null;
            if (db.exists(DB_KEY)) {
                value = db.getString(DB_KEY);
            }
            long end = SystemClock.elapsedRealtime();
            if (value == null) {
                db.put(DB_KEY, "test_123");
            } else {
                mTv.setText(value);
            }
            db.close();
            Log.d(MainActivity.class.getName(), "耗时" + (end - start));
        }


    }
}
