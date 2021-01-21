package com.gadget.manager.ui.home;

import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.Switch;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.gadget.manager.R;
import com.gadget.manager.callback.ToastCallback;
import com.gadget.manager.ui.packages.PackagesFragment;
import com.gadget.manager.utils.NativeLib;


public class HomeFragment extends Fragment {

    public ImageView ivEditGGVer, ivEditGGMode;
    public TextView tvGGVer, tvGGMode;
    public Switch ggSwich;

    private HomeViewModel homeViewModel;

    private ToastCallback suToastCallback;

    private ImageView ivIcon;
    public TextView tvLabel,tvPkgname, tvVersion, tvDescribe, tvBaseInfo, tvOthers;

    public static HomeFragment newInstance() {
        return new HomeFragment();
    }


    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        homeViewModel = new ViewModelProvider(this).get(HomeViewModel.class);
        View root = inflater.inflate(R.layout.fragment_home, container, false);

        ivEditGGVer = root.findViewById(R.id.iv_edit_ggversion);
        ivEditGGMode  = root.findViewById(R.id.iv_edit_gg_mode);
        tvGGVer = root.findViewById(R.id.tv_ggversion);
        tvGGMode = root.findViewById(R.id.tv_gg_mode);

        ggSwich = root.findViewById(R.id.swich_gg_inject);

        ivIcon = root.findViewById(R.id.home_iv_icon);

        tvLabel = root.findViewById(R.id.home_tv_label);
        tvPkgname = root.findViewById(R.id.home_tv_pkgname);
        tvVersion = root.findViewById(R.id.home_tv_version);
        tvDescribe = root.findViewById(R.id.home_tv_describe);

        tvBaseInfo = root.findViewById(R.id.home_tv_baseinfo);
        tvOthers = root.findViewById(R.id.home_tv_others);

        suToastCallback = new ToastCallback(getActivity(), "请给与GGManager root权限!", R.drawable.ic_failed);

        homeViewModel.init(this);

        return root;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
    }

    @Override
    public void onStart() {
        super.onStart();
        if(NativeLib.checkSu(null, suToastCallback)) {
            String status = NativeLib.exec("ls /data/local/tmp/xyzpkgname", true);
            if(NativeLib.isStatusSuccess(status)) {
                String result = NativeLib.exec("cat /data/local/tmp/xyzpkgname", false);
                homeViewModel.initData(result);
            } else {
                Log.e("ggmanager","/data/local/tmp/xyzpkgname not exist!");
            }
        }
    }

    @Override
    public void onResume() {
        super.onResume();
    }

    public ImageView getIvIcon() {
        return ivIcon;
    }

    public TextView getTvLabel() {
        return tvLabel;
    }

    public TextView getTvPkgname() {
        return tvPkgname;
    }

    public TextView getTvDescribe() {
        return tvDescribe;
    }
}