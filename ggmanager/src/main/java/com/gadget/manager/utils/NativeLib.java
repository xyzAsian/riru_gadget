package com.gadget.manager.utils;

import android.util.Log;

import com.gadget.manager.callback.Callback;
import com.gadget.manager.ui.signkiller.SignkillerViewModel;

public class NativeLib {

    static {
        System.loadLibrary("ggmanager");
    }

    public static boolean isStatusSuccess(String status) {
//        Log.e("ggmanager","status : "+status);
        if("0".equals(status.trim())) {
            return true;
        } else {
            return false;
        }
    }

    public static boolean checkSu(Callback ... callback) {
        String status = exec("su").trim();
//        Log.e("ggmanager","status : "+status);
        boolean hasSu = "0".equals(status);
        if(!Utils.isEmpty(callback)) {
            if(callback[0] != null && hasSu) {
                callback[0].run();
            }
            if(callback[1] != null && !hasSu) {
                callback[1].run();
            }
        }
        return hasSu;
    }

    public static String execMustSu(String cmd, Callback ... callback) {
        if(checkSu()) {
            String status = exec(cmd);
            if(isStatusSuccess(status)) {
                if(!Utils.isEmpty(callback) && callback.length >= 2 && callback[1] != null) {
                    callback[1].run();
                }
            } else {
                if(!Utils.isEmpty(callback) && callback.length >= 3 && callback[2] != null) {
                    callback[2].run();
                }
            }
            return status;
        }
        if(!Utils.isEmpty(callback) && callback.length >= 1 && callback[0] != null) {
            callback[0].run();
        }
        return null;
    }

    public static String exec(String cmd) {
        return exec(cmd, true);
    }

    public static native String exec(String cmd, boolean isRetStatus);

    public static native void startSignKillerServer();
    public static native void stopSignKillerServer();
    public static native boolean isSignKillerProcessAlive();
}