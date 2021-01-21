package com.gadget.manager.ui;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.gadget.manager.R;
import com.gadget.manager.utils.NativeLib;

public class EditGadgetModeActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_edit_gadget_mode);
    }

    private void initData() {
        String mode = NativeLib.exec("su -c \"cat /data/adb/modules/riru-RiruGadget/system/lib/libxyzgg.config.so\"", false);
    }
}