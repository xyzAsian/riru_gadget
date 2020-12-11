package com.gadget.manager.callback;

import android.content.Context;
import android.widget.Toast;

import com.gadget.manager.utils.ToastImg;

public class ToastCallback implements Callback {

    private Context mContext;
    private String mMsg;
    int resId;

    public ToastCallback(Context mContext, String msg, int resId) {
        this.mContext = mContext;
        this.mMsg = msg;
        this.resId = resId;
    }

    @Override
    public void run() {
        ToastImg.toast(mContext, mMsg, resId);
//        Toast.makeText(mContext, mMsg, Toast.LENGTH_SHORT).show();
    }

    public void setMsg(String msg) {
        this.mMsg = msg;
    }

    public void setResId(int resId) {
        this.resId = resId;
    }
}
