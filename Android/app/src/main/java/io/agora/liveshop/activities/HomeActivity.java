package io.agora.liveshop.activities;

import android.content.ComponentName;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import io.agora.liveshop.R;

public class HomeActivity extends BaseActivity implements View.OnClickListener {
    private static final String DEFAULT_ROOM = "hh12345";

    @Override
    public void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        setContentView(R.layout.home_activity_layout);
        initUI();
    }

    private void initUI() {
        // Sample channel by clicking the first product
        View boseLayout = findViewById(R.id.product_bose_layout);
        boseLayout.setOnClickListener(this);
    }

    private void toLiveRoom() {
        Intent intent = new Intent();
        intent.setComponent(new ComponentName(this, LiveRoomActivity.class.getName()));
        intent.putExtra(EXTRA_KEY_CHANNEL, DEFAULT_ROOM);
        startActivity(intent);
    }

    @Override
    public void onAllPermissionsGranted() {
        // No RTC communication for home activity
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }

    @Override
    public void onClick(View v) {
        int id = v.getId();
        switch (id) {
            case R.id.product_bose_layout:
                toLiveRoom();
                break;
        }
    }
}
