package io.agora.liveshop.activities;

import android.content.pm.PackageManager;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.view.Window;

import io.agora.liveshop.AgoraApplication;
import io.agora.liveshop.rtc.RtcEngineManager;
import io.agora.liveshop.utils.PermissionUtil;

public abstract class BaseActivity extends AppCompatActivity {
    protected static final String EXTRA_KEY_CHANNEL = "channel";

    @Override
    public void onCreate(Bundle bundle) {
        supportRequestWindowFeature(Window.FEATURE_NO_TITLE);

        super.onCreate(bundle);

        // Necessary permissions must be granted before
        // any agora RTC function performed.
        PermissionUtil.checkPermissionsIfNeeded(this,
                new PermissionUtil.OnCheckPermissionListener() {
            @Override
            public void onPermissionAlreadyGranted() {
                onAllPermissionsGranted();
            }
        });
    }

    /**
     * Called when all necessary permissions are acquired.
     * RTC calls are valid during and after onAllPermissionsGranted()
     * is called.
     */
    public abstract void onAllPermissionsGranted();

    public RtcEngineManager getRtcEngineManager() {
        return ((AgoraApplication) getApplication()).getRtcEngineManager();
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if (requestCode != PermissionUtil.REQ_LIVE_SHOP_PERMISSION) {
            return;
        }

        boolean granted = true;
        for (int result : grantResults) {
            if (result != PackageManager.PERMISSION_GRANTED) {
                granted = false;
                break;
            }
        }

        if (granted) {
            onAllPermissionsGranted();
        }
    }
}
