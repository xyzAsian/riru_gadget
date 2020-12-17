package com.gadget.manager;

import android.app.Application;
import android.content.Context;

public class CustomApplication extends Application {

    static {
        System.loadLibrary("xyzggmanager");
    }
    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
    }

    @Override
    public void onCreate() {
        super.onCreate();
    }
}
