package com.gadget.manager.utils;

import android.content.Context;
import android.content.SharedPreferences;

public class SPUtils  extends NoClassDefFoundError{

    private static final String SP_NAME = "gg_mm";
    private static SharedPreferences sSP;

    public static void init(Context context) {
        if(context != null) {
            sSP = context.getApplicationContext().getSharedPreferences(SP_NAME, Context.MODE_PRIVATE);
        }
    }

    private static final String KEY_GG_VERSION = "gg_version";

    public static String getGGVersion() {
        return getString(KEY_GG_VERSION);
    }

    public static boolean putGGVersion(String version) {
        return putString(KEY_GG_VERSION, version);
    }

    private static String getString(String key) {
        if(check()) {
            return sSP.getString(key, null);
        }
        return null;
    }

    private static boolean putString(String key, String value) {
        if(check()) {
            return sSP.edit().putString(key, value).commit();
        }
        return false;
    }

    private static boolean check() {
        if(sSP == null) {
            throw new InitErrorException();
//            return false;
        }
        return true;
    }

    static class InitErrorException extends RuntimeException {
        public InitErrorException() {
            super("Please call SPUtils.init before use get or set function.");
        }
    }
}
