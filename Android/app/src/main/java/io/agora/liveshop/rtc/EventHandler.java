package io.agora.liveshop.rtc;

import java.util.Iterator;
import java.util.concurrent.ConcurrentHashMap;

import io.agora.rtc.IRtcEngineEventHandler;

/**
 * Default global event handler for RTC engine, managing a
 * map of IEventCallback instances.
 */
public class EventHandler extends IRtcEngineEventHandler {
    private ConcurrentHashMap<Object, IEventCallback> mCallbacks = new ConcurrentHashMap<>();

    public void addCallback(IEventCallback callback) {
        if (!mCallbacks.contains(callback)) {
            mCallbacks.put(callback, callback);
        }
    }

    public void removeCallback(IEventCallback callback) {
        if (!mCallbacks.contains(callback)) {
            mCallbacks.remove(callback);
        }
    }

    public void removeAllCallbacks() {
        if (!mCallbacks.isEmpty()) {
            mCallbacks.clear();
        }
    }

    @Override
    public void onJoinChannelSuccess(String channel, int uid, int elapsed) {
        for (IEventCallback callback : mCallbacks.values()) {
            callback.onJoinChannelSuccess(channel, uid, elapsed);
        }
    }

    @Override
    public void onRejoinChannelSuccess(String channel, int uid, int elapsed) {
        super.onRejoinChannelSuccess(channel, uid, elapsed);
    }

    @Override
    public void onWarning(int warn) {
        super.onWarning(warn);
    }

    @Override
    public void onError(int err) {
        for (IEventCallback callback : mCallbacks.values()) {
            callback.onError(err);
        }
    }

    @Override
    public void onCameraReady() {
        super.onCameraReady();
    }

    @Override
    public void onVideoStopped() {
        super.onVideoStopped();
    }

    @Override
    public void onAudioQuality(int uid, int quality, short delay, short lost) {
        super.onAudioQuality(uid, quality, delay, lost);
    }

    @Override
    public void onLeaveChannel(RtcStats stats) {
        for (IEventCallback callback : mCallbacks.values()) {
            callback.onLeaveChannel(stats);
        }
    }

    @Override
    public void onRtcStats(RtcStats stats) {
        super.onRtcStats(stats);
    }

    @Override
    public void onAudioVolumeIndication(AudioVolumeInfo[] speakers, int totalVolume) {
        for (IEventCallback callback : mCallbacks.values()) {
            callback.onAudioVolumeIndication(speakers, totalVolume);
        }
    }

    @Override
    public void onNetworkQuality(int uid, int txQuality, int rxQuality) {
        super.onNetworkQuality(uid, txQuality, rxQuality);
    }

    @Override
    public void onLastmileQuality(int quality) {
        super.onLastmileQuality(quality);
    }

    @Override
    public void onUserJoined(int uid, int elapsed) {
        for (IEventCallback callback : mCallbacks.values()) {
            callback.onUserJoined(uid, elapsed);
        }
    }

    @Override
    public void onUserOffline(int uid, int reason) {
        for (IEventCallback callback : mCallbacks.values()) {
            callback.onUserOffline(uid, reason);
        }
    }

    @Override
    public void onUserMuteAudio(int uid, boolean muted) {
        super.onUserMuteAudio(uid, muted);
    }

    @Override
    public void onUserMuteVideo(int uid, boolean muted) {
        super.onUserMuteVideo(uid, muted);
    }

    @Override
    public void onUserEnableVideo(int uid, boolean enabled) {
        super.onUserEnableVideo(uid, enabled);
    }

    @Override
    public void onUserEnableLocalVideo(int uid, boolean enabled) {
        super.onUserEnableLocalVideo(uid, enabled);
    }

    @Override
    public void onLocalVideoStat(int sentBitrate, int sentFrameRate) {
        super.onLocalVideoStat(sentBitrate, sentFrameRate);
    }

    @Override
    public void onRemoteVideoStat(int uid, int delay, int receivedBitrate, int receivedFrameRate) {
        super.onRemoteVideoStat(uid, delay, receivedBitrate, receivedFrameRate);
    }

    @Override
    public void onRemoteVideoStats(RemoteVideoStats stats) {
        super.onRemoteVideoStats(stats);
    }

    @Override
    public void onLocalVideoStats(LocalVideoStats stats) {
        super.onLocalVideoStats(stats);
    }

    @Override
    public void onFirstRemoteVideoFrame(int uid, int width, int height, int elapsed) {
        super.onFirstRemoteVideoFrame(uid, width, height, elapsed);
        Iterator<IEventCallback> iterable = mCallbacks.values().iterator();
        if (iterable.hasNext()) {
            iterable.next().onFirstRemoteVideoFrame(uid, width, height, elapsed);
        }
    }

    @Override
    public void onFirstLocalVideoFrame(int width, int height, int elapsed) {
        super.onFirstLocalVideoFrame(width, height, elapsed);
    }

    @Override
    public void onFirstRemoteVideoDecoded(int uid, int width, int height, int elapsed) {
        super.onFirstRemoteVideoDecoded(uid, width, height, elapsed);
        Iterator<IEventCallback> iterable = mCallbacks.values().iterator();
        if (iterable.hasNext()) {
            iterable.next().onFirstRemoteVideoDecoded(uid, width, height, elapsed);
        }
    }

    @Override
    public void onVideoSizeChanged(int uid, int width, int height, int rotation) {
        super.onVideoSizeChanged(uid, width, height, rotation);
    }

    @Override
    public void onConnectionLost() {
        super.onConnectionLost();
    }

    @Override
    public void onConnectionInterrupted() {
        super.onConnectionInterrupted();
    }

    @Override
    public void onConnectionBanned() {
        super.onConnectionBanned();
    }

    @Override
    public void onStreamMessage(int uid, int streamId, byte[] data) {
        super.onStreamMessage(uid, streamId, data);
    }

    @Override
    public void onStreamMessageError(int uid, int streamId, int error, int missed, int cached) {
        super.onStreamMessageError(uid, streamId, error, missed, cached);
    }

    @Override
    public void onMediaEngineLoadSuccess() {
        super.onMediaEngineLoadSuccess();
    }

    @Override
    public void onMediaEngineStartCallSuccess() {
        super.onMediaEngineStartCallSuccess();
    }

    @Override
    public void onAudioMixingFinished() {
        super.onAudioMixingFinished();
    }

    @Override
    public void onAudioRouteChanged(int routing) {
        super.onAudioRouteChanged(routing);
    }

    @Override
    public void onFirstLocalAudioFrame(int elapsed) {
        super.onFirstLocalAudioFrame(elapsed);
    }

    @Override
    public void onFirstRemoteAudioFrame(int uid, int elapsed) {
        super.onFirstRemoteAudioFrame(uid, elapsed);
    }

    @Override
    public void onActiveSpeaker(int uid) {
        super.onActiveSpeaker(uid);
    }

    @Override
    public void onAudioEffectFinished(int soundId) {
        super.onAudioEffectFinished(soundId);
    }

    @Override
    public void onClientRoleChanged(int oldRole, int newRole) {
        super.onClientRoleChanged(oldRole, newRole);
    }

    @Override
    public void onReceiveSEI(String info) {
        for (IEventCallback callback : mCallbacks.values()) {
            callback.onReceiveSEI(info);
        }
    }

    @Override
    public byte[] onSendSEI() {
        byte[] result = null;
        Iterator<IEventCallback> iterable = mCallbacks.values().iterator();
        if (iterable.hasNext()) {
            result = iterable.next().onSendSEI();
        }

        return result;
    }
}
