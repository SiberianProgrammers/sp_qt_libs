package com.sp.statusBar;

import org.qtproject.qt5.android.bindings.QtActivity;
import android.app.Activity;
import android.os.Bundle;
import android.os.Build;
import android.view.Window;
import android.view.WindowManager;
import android.view.View;
import android.graphics.Color;
import android.content.Context;

public class StatusBarActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                getWindow().addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
                getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
                getWindow().setStatusBarColor(Color.TRANSPARENT);
            } else {
                getWindow().setFlags( WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS
                                    , WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
            }
        }
    }

    //--------------------------------------------------------------------------
    // @brief Возвращает высоту status bar
    //--------------------------------------------------------------------------
    public int statusBarHeight() {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.KITKAT) {
            return 0;
        }

        int result = 0;
        int resourceId = getResources().getIdentifier("status_bar_height", "dimen", "android");
        if (resourceId > 0) {
            result = getResources().getDimensionPixelSize(resourceId);
        }

        return result;
    }
}
