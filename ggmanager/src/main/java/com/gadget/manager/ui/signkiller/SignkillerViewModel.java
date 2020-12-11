package com.gadget.manager.ui.signkiller;

import android.util.Log;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.gadget.manager.R;
import com.gadget.manager.callback.ToastCallback;
import com.gadget.manager.utils.NativeLib;

public class SignkillerViewModel extends ViewModel {

    private MutableLiveData<String> mText;

    public SignkillerViewModel() {
        mText = new MutableLiveData<>();
        mText.setValue("This is dashboard fragment");
    }

    public LiveData<String> getText() {
        return mText;
    }

    private SignkillerFragment mSf;
    private ToastCallback suToastCallback;
    public void init(SignkillerFragment sf) {
        this.mSf = sf;
        suToastCallback = new ToastCallback(mSf.getActivity(), "请给与GGManager root权限!", R.drawable.ic_failed);

        this.mSf.mCbSwich.setOnClickListener(view -> {
            if(NativeLib.checkSu(null, suToastCallback)) {
                Log.v("ggmanager", "cbbox : "+mSf.mCbSwich.isChecked());
                if(mSf.mCbSwich.isChecked()) {
                    NativeLib.startSignKillerServer();
                } else {
                    NativeLib.stopSignKillerServer();
                }
            }
        });
    }

    public void initData() {
        if(NativeLib.checkSu(null, suToastCallback)) {
            setSwitch(NativeLib.isSignKillerProcessAlive());
        }
        new Thread(() -> {
            while (true) {
                try {
                    Thread.sleep(5000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                mSf.getActivity().runOnUiThread(()-> {
                    setSwitch(NativeLib.isSignKillerProcessAlive());
                });
            }
        }).start();
    }

    public void setSwitch(boolean swich) {
        if(mSf.mCbSwich.isChecked() != swich) {
//            mSf.mCbSwich.setTag(Boolean.TRUE);
            mSf.mCbSwich.setChecked(swich);
        }
    }
}