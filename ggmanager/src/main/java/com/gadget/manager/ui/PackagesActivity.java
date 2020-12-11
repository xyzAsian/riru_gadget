package com.gadget.manager.ui;

import android.os.Bundle;

import com.gadget.manager.BaseActivity;
import com.gadget.manager.R;
import com.gadget.manager.ui.packages.PackagesFragment;

public class PackagesActivity extends BaseActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.packages_activity);
        if (savedInstanceState == null) {
            getSupportFragmentManager().beginTransaction()
                    .replace(R.id.container, PackagesFragment.newInstance())
                    .commitNow();
        }
        setTitle("Package Select");
    }

}