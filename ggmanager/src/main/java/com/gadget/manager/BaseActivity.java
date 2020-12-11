package com.gadget.manager;

import android.os.Bundle;
import android.text.Spannable;
import android.text.SpannableString;
import android.text.style.ForegroundColorSpan;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;


public class BaseActivity extends AppCompatActivity {

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    protected void onStart() {
        super.onStart();
    }

    @Override
    public void setTitle(CharSequence title) {

        SpannableString spannableString = new SpannableString(title);
        spannableString.setSpan(new ForegroundColorSpan(getResources().getColor(R.color.black, getTheme())), 0, spannableString.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
        super.setTitle(spannableString);
    }
}
