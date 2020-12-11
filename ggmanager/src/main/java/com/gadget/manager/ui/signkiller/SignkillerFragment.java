package com.gadget.manager.ui.signkiller;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CheckBox;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.gadget.manager.R;

public class SignkillerFragment extends Fragment {

    private SignkillerViewModel dashboardViewModel;

    public CheckBox mCbSwich;

    public View onCreateView(@NonNull LayoutInflater inflater,
            ViewGroup container, Bundle savedInstanceState) {
        dashboardViewModel =
                new ViewModelProvider(this).get(SignkillerViewModel.class);
        View root = inflater.inflate(R.layout.fragment_signkiller, container, false);
        mCbSwich = root.findViewById(R.id.cb_signkiller);
        dashboardViewModel.init(this);
        return root;
    }

    @Override
    public void onStart() {
        super.onStart();
        dashboardViewModel.initData();
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        this.dashboardViewModel = null;
        this.mCbSwich = null;
    }
}