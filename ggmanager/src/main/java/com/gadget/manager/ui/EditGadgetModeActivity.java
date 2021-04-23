package com.gadget.manager.ui;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.EditText;

import com.gadget.manager.R;
import com.gadget.manager.utils.NativeLib;

public class EditGadgetModeActivity extends AppCompatActivity {

    public static final int REQUEST_CODE = 1000;

    private EditText mETInput;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setTitle("Gadget Edit");
        setContentView(R.layout.activity_edit_gadget_mode);

        mETInput = findViewById(R.id.et_gg_mode);
        initData();
    }

    private void initData() {
        String mode = NativeLib.exec("su -c \"cat /data/adb/modules/riru-RiruGadget/system/lib/libxyzgg.config.so\"", false);
        mETInput.setText(mode);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_edit_gadget, menu);
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        switch (item.getItemId()) {
            case R.id.menu_edit_gadget_finish:
                String text = mETInput.getText().toString();
                text = text.replaceAll("\"","\\\\\"");
                NativeLib.exec("su -c \"echo '"+text+"' > /data/adb/modules/riru-RiruGadget/system/lib/libxyzgg.config.so\"");
                NativeLib.exec("su -c \"echo '"+text+"' > /data/adb/modules/riru-RiruGadget/system/lib64/libxyzgg.config.so\"");
                setResult(RESULT_OK);
                finish();
                break;
        }
        return super.onOptionsItemSelected(item);
    }
}