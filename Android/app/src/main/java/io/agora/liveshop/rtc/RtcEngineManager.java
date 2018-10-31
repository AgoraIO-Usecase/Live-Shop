package io.agora.liveshop.rtc;

import android.content.Context;

import io.agora.liveshop.R;
import io.agora.rtc.Constants;
import io.agora.rtc.RtcEngine;

public class RtcEngineManager {
    private Context mContext;
    private RtcEngine mRtcEngine;
    private EventHandler mEventHandler;

    private boolean mStarted = false;

    public RtcEngineManager(Context context) {
        mContext = context;
    }

    public void init() {
        if (mStarted) {
            return;
        }

        mEventHandler = new EventHandler();

        String appId = mContext.getResources().getString(R.string.agora_app_id);
        try {
            mRtcEngine = RtcEngine.create(mContext, appId, mEventHandler);
        } catch (Exception e) {
            e.printStackTrace();
        }

        // Uncomment this to acquire full sdk log
        // mRtcEngine.setParameters("{\"rtc.log_filter\":65535}");

        // Set your log file path
        // mRtcEngine.setLogFile("path");

        // Use this option to monitor the volumes of
        // users in the same channel
        // mRtcEngine.enableAudioVolumeIndication(300, 3);

        // The channel is set in live broadcasting mode. In this
        // mode, only broadcasters are eligible to send audio and
        // video streams, while audience can only receive streams.
        // In this application all users are audience. The audio
        // and video streams are received from another broadcasting
        // client.
        mRtcEngine.setChannelProfile(Constants.CHANNEL_PROFILE_LIVE_BROADCASTING);
        mRtcEngine.setClientRole(Constants.CLIENT_ROLE_AUDIENCE);

        mRtcEngine.enableVideo();
    }

    public RtcEngine getRtcEngine() {
        return mRtcEngine;
    }

    public void joinChannel(String token, String channel, String data, int uid) {
        if (mRtcEngine != null) {
            mRtcEngine.joinChannel(token, channel, data, uid);
        }
    }

    public void leaveChannel() {
        if (mRtcEngine != null) {
            mRtcEngine.leaveChannel();
        }
    }

    public void destroy() {
        removeAllCallbacks();
        RtcEngine.destroy();
        mStarted = false;
    }

    public void addEventCallback(IEventCallback callback) {
        mEventHandler.addCallback(callback);
    }

    public void removeEventCallback(IEventCallback callback) {
        mEventHandler.removeCallback(callback);
    }

    public void removeAllCallbacks() {
        mEventHandler.removeAllCallbacks();
    }
}
