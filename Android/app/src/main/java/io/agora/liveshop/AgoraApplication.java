package io.agora.liveshop;

import android.app.Application;

import io.agora.liveshop.rtc.RtcEngineManager;

public class AgoraApplication extends Application {
    private RtcEngineManager mRtcEngineManager;

    @Override
    public void onCreate() {
        super.onCreate();
        mRtcEngineManager = new RtcEngineManager(this);
    }

    @Override
    public void onTerminate() {
        super.onTerminate();

        if (mRtcEngineManager != null) {
            mRtcEngineManager.destroy();
        }
    }

    public RtcEngineManager getRtcEngineManager() {
        return mRtcEngineManager;
    }
}
