package io.agora.liveshop.rtc;

import android.util.Log;
import android.widget.Toast;

import java.nio.charset.Charset;
import java.util.Iterator;
import java.util.concurrent.ConcurrentHashMap;

import io.agora.rtc.IMetadataObserver;
import io.agora.rtc.IRtcEngineEventHandler;

/**
 * Default global event handler for RTC engine, managing a
 * map of IEventCallback instances.
 */
public class EventHandler extends IRtcEngineEventHandler {

    public static final String TAG = EventHandler.class.getSimpleName();

    /**
     * Maximum length of meta data
     */
    private int MAX_META_SIZE = 1024;
    /**
     * Meta data to be sent
     */
    private byte[] metadata;
    private ConcurrentHashMap<Object, IEventCallback> mCallbacks = new ConcurrentHashMap<>();

    /**
     * By implementing this interface, metadata can be sent and received with video frames.
     */
    private final IMetadataObserver iMetadataObserver = new IMetadataObserver()
    {
        /**Returns the maximum data size of Metadata*/
        @Override
        public int getMaxMetadataSize()
        {
            return MAX_META_SIZE;
        }

        /**Occurs when the SDK is ready to receive and send metadata.
         * You need to specify the metadata in the return value of this callback.
         * @param timeStampMs The timestamp (ms) of the current metadata.
         * @return The metadata that you want to send in the format of byte[]. Ensure that you set the return value.
         * PS: Ensure that the size of the metadata does not exceed the value set in the getMaxMetadataSize callback.*/
        @Override
        public byte[] onReadyToSendMetadata(long timeStampMs)
        {
            /**Check if the metadata is empty.*/
            if (metadata == null)
            {
                return null;
            }
            Log.i(TAG, "There is metadata to send!");
            /**Recycle metadata objects.*/
            byte[] toBeSend = metadata;
            metadata = null;
            if (toBeSend.length > MAX_META_SIZE)
            {
                Log.e(TAG, String.format("Metadata exceeding max length %d!", MAX_META_SIZE));
                return null;
            }
            String data = new String(toBeSend, Charset.forName("UTF-8"));
            Iterator<IEventCallback> iterable = mCallbacks.values().iterator();
            if (iterable.hasNext()) {
                toBeSend = iterable.next().onSendSEI();
            }
            Log.i(TAG, String.format("Metadata sent successfully! The content is %s", data));
            return toBeSend;
        }

        /**Occurs when the local user receives the metadata.
         * @param buffer The received metadata.
         * @param uid The ID of the user who sent the metadata.
         * @param timeStampMs The timestamp (ms) of the received metadata.*/
        @Override
        public void onMetadataReceived(byte[] buffer, int uid, long timeStampMs)
        {
            String data = new String(buffer, Charset.forName("UTF-8"));
            for (IEventCallback callback : mCallbacks.values()) {
                callback.onReceiveSEI(data);
            }
            Log.i(TAG, "onMetadataReceived:" + data);
        }
    };

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
}
