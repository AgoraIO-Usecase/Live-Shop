package io.agora.liveshop.utils;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;

import java.util.ArrayList;

public class PermissionUtil {
    public static final int REQ_LIVE_SHOP_PERMISSION = 0;

    public static void checkPermissionsIfNeeded(Activity activity,
                                                @NonNull OnCheckPermissionListener listener) {
        ArrayList<String> requests = new ArrayList<>();

        if (permissionNotGranted(activity, Manifest.permission.RECORD_AUDIO)) {
            requests.add(Manifest.permission.RECORD_AUDIO);
        }

        if (permissionNotGranted(activity, Manifest.permission.CAMERA)) {
            requests.add(Manifest.permission.CAMERA);
        }

        if (permissionNotGranted(activity, Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
            requests.add(Manifest.permission.WRITE_EXTERNAL_STORAGE);
        }

        if (!requests.isEmpty()) {
            String[] permissions = new String[requests.size()];
            requests.toArray(permissions);
            ActivityCompat.requestPermissions(activity, permissions, REQ_LIVE_SHOP_PERMISSION);
        } else {
            listener.onPermissionAlreadyGranted();
        }
    }

    private static boolean permissionNotGranted(Context context, String permission) {
        return ContextCompat.checkSelfPermission(context, permission)
                != PackageManager.PERMISSION_GRANTED;
    }

    public interface OnCheckPermissionListener {
        void onPermissionAlreadyGranted();
    }
}
