package com.gadget.manager.ui.packages;

import androidx.lifecycle.ViewModelProvider;

import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;

import com.gadget.manager.R;


public class PackagesFragment extends Fragment {

    private PackagesViewModel mViewModel;

    public static PackagesFragment newInstance() {
        return new PackagesFragment();
    }

    private ListView mLvPackages;

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setHasOptionsMenu(true);
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container,
                             @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_packages, container, false);
        mLvPackages = view.findViewById(R.id.lv_pkgs);
        return view;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        mViewModel = new ViewModelProvider(this).get(PackagesViewModel.class);
        // TODO: Use the ViewModel
    }

    @Override
    public void onResume() {
        super.onResume();
        mViewModel.init(this);
    }

    public ListView getLvPackages() {
        return mLvPackages;
    }

    public void notifyPackagesList(boolean isSystemAppEnable) {
        mViewModel.notifyPkgs(isSystemAppEnable);
    }

    @Override
    public void onCreateOptionsMenu(@NonNull Menu menu, @NonNull MenuInflater inflater) {
        inflater.inflate(R.menu.menu_packages, menu);
        super.onCreateOptionsMenu(menu, inflater);
    }

    private boolean isSystemAppEnable = false;
    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        if(item.getItemId() == R.id.menu_system_app) {
            isSystemAppEnable = !isSystemAppEnable;
            notifyPackagesList(isSystemAppEnable);
            item.setIcon(isSystemAppEnable ? R.drawable.ic_icon_system_app_enable : R.drawable.ic_icon_ssytem_app_disable);
        }
        return super.onOptionsItemSelected(item);
    }

    public boolean isSystemAppEnable() {
        return isSystemAppEnable;
    }
}