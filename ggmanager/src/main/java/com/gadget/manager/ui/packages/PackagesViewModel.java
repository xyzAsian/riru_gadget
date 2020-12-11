package com.gadget.manager.ui.packages;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.ListAdapter;
import android.widget.TextView;

import androidx.lifecycle.ViewModel;

import com.gadget.manager.R;
import com.gadget.manager.callback.ToastCallback;
import com.gadget.manager.ui.PackagesActivity;
import com.gadget.manager.utils.NativeLib;

import java.util.ArrayList;
import java.util.List;

public class PackagesViewModel extends ViewModel implements AdapterView.OnItemClickListener {
    // TODO: Implement the ViewModel

    private PackagesFragment pf;
    private PackageAdapter mAdapter;
    private List<PackageInfo> mPackages;
    private List<PackageInfo> mNoSysPackages;
    private ToastCallback suToastCallback;
    private ToastCallback successToastCallback;
    private ToastCallback failedToastCallback;

    public void init(PackagesFragment pf) {
        Context context = pf.getActivity();
        initData(context);
        suToastCallback = new ToastCallback(context, "请给与GGManager root权限!", R.drawable.ic_failed);
        successToastCallback = new ToastCallback(context, "", R.drawable.ic_success);
        failedToastCallback = new ToastCallback(context, "", R.drawable.ic_failed);
        mAdapter = new PackageAdapter(context, pf.isSystemAppEnable() ? mPackages : mNoSysPackages);
        pf.getLvPackages().setAdapter(mAdapter);
        pf.getLvPackages().setOnItemClickListener(this);
    }

    public void notifyPkgs(boolean isSysAppEnable) {
        mAdapter.setPackages(isSysAppEnable ? mPackages : mNoSysPackages);
        mAdapter.notifyDataSetChanged();
    }

    // 通过packName得到PackageInfo，作为参数传入即可
    private boolean isSystemApp(PackageInfo pi) {
        boolean isSysApp = (pi.applicationInfo.flags & ApplicationInfo.FLAG_SYSTEM) == 1;
        boolean isSysUpd = (pi.applicationInfo.flags & ApplicationInfo.FLAG_UPDATED_SYSTEM_APP) == 1;
        return isSysApp || isSysUpd;
    }

    private void initData(Context context) {
        List<PackageInfo> list = context.getPackageManager().getInstalledPackages(PackageManager.GET_ACTIVITIES | PackageManager.GET_SERVICES);
        mNoSysPackages = new ArrayList<>();
        mPackages = new ArrayList<>();
        for (PackageInfo info : list) {
            if(!isSystemApp(info)) {
                mNoSysPackages.add(info);
            } else {
                mPackages.add(info);
            }
        }
    }

    @Override
    public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
        PackageInfo item = mAdapter.getItem(i);
        successToastCallback.setMsg("设置HOOK应用成功:"+item.applicationInfo.loadLabel(adapterView.getContext().getPackageManager()));
        failedToastCallback.setMsg("设置HOOK应用出错，请联系Developer!");
        NativeLib.execMustSu("su -c \"echo \\\""+item.packageName+"\\\" > /data/local/tmp/xyzpkgname\"", suToastCallback, successToastCallback, failedToastCallback);
    }


    static class PackageAdapter extends BaseAdapter {

        private Context context;
        private List<PackageInfo> mPackages;

        public void setPackages(List<PackageInfo> mPackages) {
            this.mPackages = mPackages;
        }

        public PackageAdapter(Context context, List<PackageInfo> mPackages) {
            this.context = context;
            this.mPackages = mPackages;
        }

        @Override
        public int getCount() {
            return mPackages.size();
        }

        @Override
        public PackageInfo getItem(int i) {
            return mPackages.get(i);
        }

        @Override
        public long getItemId(int i) {
            return i;
        }

        @Override
        public View getView(int i, View view, ViewGroup viewGroup) {
            ViewHolder holder = null;
            if(view == null) {
                holder = new ViewHolder();
                view = LayoutInflater.from(context).inflate(R.layout.adapter_packages, null);
                holder.ivIcon = view.findViewById(R.id.adapter_iv_icon);
                holder.tvLabel = view.findViewById(R.id.adapter_tv_label);
                holder.tvPkgname = view.findViewById(R.id.adapter_tv_pkgname);
                view.setTag(holder);
            } else {
                holder = (ViewHolder) view.getTag();
            }
            PackageInfo info = getItem(i);
            holder.ivIcon.setImageDrawable(info.applicationInfo.loadIcon(context.getPackageManager()));
            holder.tvLabel.setText(info.applicationInfo.loadLabel(context.getPackageManager()));
            holder.tvPkgname.setText(info.packageName);
            return view;
        }

        class ViewHolder {
            ImageView ivIcon;
            TextView tvLabel;
            TextView tvPkgname;
        }
    }
}