package sp;

import org.qtproject.qt5.android.QtNative;
import org.qtproject.qt5.android.bindings.QtActivity;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.MemoryInfo;
import android.app.Application;
import android.os.Bundle;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.os.IBinder;
import android.view.Window;
import android.view.WindowManager;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.content.Intent;
import android.telephony.SmsManager;
import android.util.TypedValue;
import android.text.TextUtils;
import android.graphics.Color;
import android.R.style;
import android.graphics.Rect;
import android.content.Context;
import android.provider.Settings.Secure;

public class SpActivity extends QtActivity
{
    public String mac_address = "";
    public int _statusBarHeight = 0; // Высота статус бара

    Context context;
 
    // hander основного потока
    private Handler handler = new Handler(Looper.getMainLooper());

    //--------------------------------------------------------------------------
    @Override
    public void onCreate(Bundle savedInstanceState) {
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
        _statusBarHeight = statusBarHeight();
        logInfo("SpActivity Create Succesefull!");
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
    public void sendSms (String text, String phone_number) {
        SmsManager sms = SmsManager.getDefault();
        sms.sendTextMessage(phone_number, null, text, null, null);
    }

    //--------------------------------------------------------------------------
    // @brief Возвращает размер statusBar'a
    //--------------------------------------------------------------------------
    public int statusBarHeight() {
        if (!availibleChangeStatusBar()) {
            return 0;
        }

        if (_statusBarHeight != 0) {
            return _statusBarHeight;
        }

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
    public int getHeight() {
        Window window = this.getWindow();
        View viewMain = window.getDecorView();

        Rect r = new Rect();
        viewMain.getWindowVisibleDisplayFrame(r);

        int screenHeight = viewMain.getRootView().getHeight();
        int heightDifference = r.bottom - r.top;

        return heightDifference;
    }

    //--------------------------------------------------------------------------
    public static String getDeviceID(Context context) {
        return Secure.getString(context.getContentResolver(), Secure.ANDROID_ID);
    }

    //--------------------------------------------------------------------------
    float pixelDencity() {
        return TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_MM, 1,
                        getResources().getDisplayMetrics());
    }

    //--------------------------------------------------------------------------
    public static long availableRAM(Context context) {
        MemoryInfo mi = new MemoryInfo();
        ActivityManager activityManager = (ActivityManager)context.getSystemService(Context.ACTIVITY_SERVICE);
        activityManager.getMemoryInfo(mi);
        long availableMegs = mi.availMem / 1048576L;
        return availableMegs;
    }

    //--------------------------------------------------------------------------
    public static int getVersion() {
        return Build.VERSION.SDK_INT;
    }

    //--------------------------------------------------------------------------
    public static long totalRam(Context context) {
        MemoryInfo mi = new MemoryInfo();
        ActivityManager activityManager = (ActivityManager)context.getSystemService(Context.ACTIVITY_SERVICE);
        activityManager.getMemoryInfo(mi);
        long totalMegs = mi.totalMem / 1048576L;
        return totalMegs;
    }

    //--------------------------------------------------------------------------
    public void hideKeyboard()
    {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                try {
                    final View v = getCurrentFocus();
                    if (v == null) {
                        return;
                    }

                    InputMethodManager imm = (InputMethodManager)v.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
                    if (imm == null)  {
                        logInfo("K12Activity.java::hideKeyboard: InputMethodManager is null");
                        return;
                    }

                    IBinder token = v.getWindowToken();
                    if( token != null ) {
                        imm.hideSoftInputFromWindow(token, 0);
                    }
                } catch (Exception e) {
                    logError("catch K12Activity.java::hideKeyboard: exception:" + e);
                }
            }
        });
    }

    //--------------------------------------------------------------------------
    public void showKeyboard() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                try {
                    final View v = getCurrentFocus();
                    if (v == null) {
                        logError(" K12Activity.java::showKeyboard: View is null");
                        return;
                    }

                    InputMethodManager imm = (InputMethodManager)v.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
                    if (imm == null) {
                        logInfo("K12Activity.java::showKeyboard: InputMethodManager is null");
                        return;
                    }

                    imm.showSoftInput(v, InputMethodManager.SHOW_IMPLICIT);
                } catch (Exception e) {
                    logError("catch  K12Activity.java::showKeyboard: exception:" + e);
                }
            }
        });
    }

    //--------------------------------------------------------------------------
    public static native void keyboardVisibleChanged(boolean visible, int height);

    public static native void logInfo(String text);
    public static native void logError(String text);
}

