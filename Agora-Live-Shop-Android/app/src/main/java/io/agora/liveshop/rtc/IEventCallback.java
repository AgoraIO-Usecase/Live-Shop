package io.agora.liveshop.rtc;

import io.agora.rtc.IRtcEngineEventHandler;

/**
 * A list of event handler callbacks which are needed for this
 * particular application.
 * This callback class is useful when not all RtcEngine
 * EventHandler callbacks are used (which happens very often).
 * If more callbacks are used, do not forget to call them
 * in the global EventHandler.
 * Typically, an event callback is registered to the EventHandler
 * when entering a new Activity, handling specific business if
 * any callback is invoked, and then they are unregistered
 * when leaving that Activity
 */
public interface IEventCallback {
    void onJoinChannelSuccess(String channel, int uid, int elapsed);
    void onUserJoined(int uid, int elapsed);
    void onUserOffline(int uid, int reason);
    void onError(int err);
    void onLeaveChannel(IRtcEngineEventHandler.RtcStats stats);
    void onAudioVolumeIndication(IRtcEngineEventHandler.AudioVolumeInfo[] speakers, int totalVolume);
    void onFirstRemoteVideoFrame(int uid, int width, int height, int elapsed);
    void onFirstRemoteVideoDecoded(int uid, int width, int height, int elapsed);
    void onReceiveSEI(String info);
    byte[] onSendSEI();
}
