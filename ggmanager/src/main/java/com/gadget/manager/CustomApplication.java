package com.gadget.manager;

import android.app.Application;
import android.content.Context;
import android.text.TextUtils;

import com.gadget.manager.utils.SPUtils;
import com.gadget.manager.utils.Utils;

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
        SPUtils.init(this.getBaseContext());
        if(TextUtils.isEmpty(SPUtils.getGGVersion())) {
            SPUtils.putGGVersion(Utils.getGGVersionFromMetaDataByKey(getApplicationContext()));
        }
    }
}
