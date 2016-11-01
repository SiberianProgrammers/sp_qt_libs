package dxs;

import org.qtproject.qt5.android.QtNative;
import org.qtproject.qt5.android.bindings.QtActivity;

import android.app.Activity;
import android.app.Application;
import android.os.Bundle;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.view.Window;
import android.view.WindowManager;
import android.view.View;
import android.content.Intent;
import android.telephony.SmsManager;
import android.util.TypedValue;
import android.text.TextUtils;
import android.graphics.Color;
import android.R.style;
import android.graphics.Rect;
import android.content.Context;

public class DxsActivity extends QtActivity
{
    public String mac_address = "";
    public int statusBarHeight = 0; // Высота статус бара

    Context context;
 
    // hander основного потока
    private Handler handler = new Handler(Looper.getMainLooper());

    //--------------------------------------------------------------------------
    @Override
    public void onCreate(Bundle savedInstanceState) {
        System.out.println("#### onCreate DxsActivity!");
        context = this.getApplicationContext();

        super.onCreate(savedInstanceState);

        if (availibleChangeStatusBar()) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                getWindow().addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
                getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
                getWindow().setStatusBarColor(Color.TRANSPARENT);
            } else {
                this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS,WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
            }
        }
        statusBarHeight = getStatusBarHeight();
    }

    //--------------------------------------------------------------------------
    @Override
    public void onDestroy() {
        super.onDestroy();
    }

    //--------------------------------------------------------------------------
    @Override
    public void onStop() {
        super.onStop();
    }

    //--------------------------------------------------------------------------
    @Override
    public void onStart() {
        super.onStart();
    }

    //--------------------------------------------------------------------------
    @Override
    protected void onResume() {
        super.onResume();
    }

    //--------------------------------------------------------------------------
    @Override
    protected void onPause() {
        super.onPause();
    }

    //--------------------------------------------------------------------------
    /** @brief Вызывает окно отсылки sms. */
    //--------------------------------------------------------------------------
    public void sendSms (String text, String phone_number)
    {
        SmsManager sms = SmsManager.getDefault();
        sms.sendTextMessage(phone_number, null, text, null, null);
    }

    //--------------------------------------------------------------------------
    // @brief Возвращает размер statusBar'a
    //--------------------------------------------------------------------------
    public int getStatusBarHeight() {
        if (!availibleChangeStatusBar()) { return 0; }
        if ( statusBarHeight != 0) { return statusBarHeight; }

        int result = 0;
        int resourceId = getResources().getIdentifier("status_bar_height", "dimen", "android");
        if (resourceId > 0) {
            result = getResources().getDimensionPixelSize(resourceId);
        }

        return result;
    }

    //--------------------------------------------------------------------------
    // @brief Можем ли мы изменять statusbar
    //--------------------------------------------------------------------------
    public boolean availibleChangeStatusBar() {
        return (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT);
    }

    //--------------------------------------------------------------------------
    // @brief Возвращает размер рабочей области (без клавиатуры)
    // Для расчета клавиатуры взять высоту окна и вычесть размер клавиатуры
    //--------------------------------------------------------------------------
    public int getHeight()
    {
        Window window = this.getWindow();
        View viewMain = window.getDecorView();

        Rect r = new Rect();
        viewMain.getWindowVisibleDisplayFrame(r);

        int screenHeight = viewMain.getRootView().getHeight();
        int heightDifference = r.bottom - r.top;

        return heightDifference;
    }

    //--------------------------------------------------------------------------
    float pixelDencity()
    {
        return TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_MM, 1,
                        getResources().getDisplayMetrics());
    }
}

