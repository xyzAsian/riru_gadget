package com.gadget.manager.ui.home;

import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PermissionInfo;
import android.os.Build;
import android.util.Log;
import android.view.View;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.gadget.manager.R;
import com.gadget.manager.ui.EditGadgetModeActivity;
import com.gadget.manager.ui.PackagesActivity;
import com.gadget.manager.utils.NativeLib;
import com.gadget.manager.utils.SPUtils;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

public class HomeViewModel extends ViewModel implements View.OnClickListener{

    private SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");

    private MutableLiveData<String> mText;

    public HomeViewModel() {
        mText = new MutableLiveData<>();
        mText.setValue("This is home fragment");
    }

    public LiveData<String> getText() {
        return mText;
    }

    private HomeFragment hf;
    private Context mContext;

    public void init(HomeFragment hf) {
        this.hf = hf;
        this.mContext = hf.getActivity();
        hf.getIvIcon().setOnClickListener(this);
        hf.ivEditGGMode.setOnClickListener(this);
    }

    public void initData(String pkgname) {
        try {
            pkgname = pkgname.trim();
            PackageInfo packageInfo = mContext.getPackageManager().getPackageInfo(pkgname, PackageManager.GET_ACTIVITIES | PackageManager.GET_SERVICES);
            hf.getIvIcon().setImageDrawable(packageInfo.applicationInfo.loadIcon(mContext.getPackageManager()));
            hf.getTvLabel().setText(packageInfo.applicationInfo.loadLabel(mContext.getPackageManager()));
            hf.getTvPkgname().setText(packageInfo.packageName);
            hf.tvVersion.setText(packageInfo.versionName+"    "+packageInfo.versionCode);
            hf.getTvDescribe().setText(packageInfo.applicationInfo.loadDescription(mContext.getPackageManager()));

            StringBuilder baseInfo = new StringBuilder();
            baseInfo.append("minSdkVersion\t\t\t\t:\t\t").append( (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) ? packageInfo.applicationInfo.minSdkVersion : "Unknow").append("\n");
            baseInfo.append("targetSdkVersion\t\t:\t\t").append(packageInfo.applicationInfo.targetSdkVersion).append("\n");
            baseInfo.append("firstInstallTime\t\t\t\t:\t\t").append(sdf.format(new Date(packageInfo.firstInstallTime))).append("\n");
            baseInfo.append("lastUpdateTime\t\t\t:\t\t").append(sdf.format(new Date(packageInfo.lastUpdateTime)));
            hf.tvBaseInfo.setText(baseInfo);

        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }

        try {
            hf.tvGGVer.setText("Frida libgadget.so version is : "+SPUtils.getGGVersion());
            String mode = NativeLib.exec("su -c \"cat /data/adb/modules/riru-RiruGadget/system/lib/libxyzgg.config.so\"", false);
            hf.tvGGMode.setText(mode);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.home_iv_icon:
                this.hf.startActivity(new Intent(this.hf.getActivity(), PackagesActivity.class));
                break;
            case R.id.iv_edit_gg_mode:
                this.hf.startActivity(new Intent(this.hf.getActivity(), EditGadgetModeActivity.class));
                break;
        }
    }
}