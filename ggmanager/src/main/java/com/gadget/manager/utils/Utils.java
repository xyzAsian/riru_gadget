package com.gadget.manager.utils;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;

public class Utils {

    public static final <T> boolean isEmpty (T[] t) {
        if (t == null || t.length == 0) {
            return true;
        }
        return false;
    }

    private static final String KEY_META_DATA_GG_Version = "ggVersion";

    public static final String getGGVersionFromMetaDataByKey(Context context) {
        return getMetaDataByKey(context, KEY_META_DATA_GG_Version);
    }

    public static final String getMetaDataByKey(Context context, String key) {
        try {
            ApplicationInfo appInfo = context.getPackageManager().getApplicationInfo(context.getPackageName(), PackageManager.GET_META_DATA);
            String appMV = appInfo.metaData.getString(key);
            return appMV;
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return null;
    }
}
