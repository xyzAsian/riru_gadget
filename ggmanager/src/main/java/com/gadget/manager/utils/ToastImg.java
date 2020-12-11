package com.gadget.manager.utils;

import android.content.Context;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.gadget.manager.R;

public class ToastImg {

    public static void toast(Context context, String msg, int imgId) {
        Toast toast=new Toast(context.getApplicationContext());

//创建一个填充物,用于填充Toast
        LayoutInflater inflater = LayoutInflater.from(context);

        toast.setGravity(Gravity.CENTER,0,0);

//填充物来自的xml文件,在这个改成一个view
//实现xml到view的转变哦
        View view =inflater.inflate(R.layout.toast,null);

//不一定需要，找到xml里面的组件，设置组件里面的具体内容
        ImageView imageView1=view.findViewById(R.id.iv_toast);
        TextView textView1=view.findViewById(R.id.tv_toast);
        imageView1.setImageResource(imgId);
        textView1.setText(msg);

//把填充物放进toast
        toast.setView(view);
        toast.setDuration(Toast.LENGTH_SHORT);

//展示toast
        toast.show();
    }
}
