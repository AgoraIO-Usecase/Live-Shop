//
//  MediaManager.swift
//  AgoraLiveShop
//
//  Created by ZhangJi on 2018/10/9.
//  Copyright © 2018 ZhangJi. All rights reserved.
//


//
import UIKit
import AgoraRtcKit

@objc enum SeiType: NSInteger {
    case prod, quiz
}

@objc protocol MediaManagerDelegate: NSObjectProtocol {
    @objc optional func mediaMangaer(_ manager: MediaManager, didJoinChannel channel: String, withUid uid: UInt)
    @objc optional func mediaManagerDidLeaveChannel(_ manager: MediaManager)
    @objc optional func mediaManager(_ manager: MediaManager, didJoinedOfUid uid: UInt)
    
    @objc optional func mediaManager(_ manager: MediaManager, didReceiveSEI sei: NSDictionary, type: SeiType)
    
    @objc optional func mediaManager(_ manager: MediaManager, didOfflineOfUid uid: UInt)
}

class MediaManager: NSObject {
    public weak var delegate: MediaManagerDelegate?
    
    public var channelProfile = AgoraChannelProfile.liveBroadcasting
    
    public var clientRole = AgoraClientRole.audience {
        didSet {
            agoraKit.setClientRole(clientRole)
        }
    }
    
    private static let manager = MediaManager()
    
    private var agoraKit: AgoraRtcEngineKit!
    
    // video metadata to be sent later
    var metadata: Data?
    // metadata lenght limitation
    let MAX_META_LENGTH = 1024
    
}

extension MediaManager {
    public static func shared() -> MediaManager {
        return manager
    }
    
    public func joinMediaChannel(_ channel: String) -> Int32{
        agoraKit = AgoraRtcEngineKit.sharedEngine(withAppId: KeyCenter.AppId, delegate: self)
        agoraKit.enableVideo()
        agoraKit.setChannelProfile(channelProfile)
        agoraKit.setClientRole(clientRole)
        agoraKit.setMediaMetadataDelegate(self, with: .video)
        let code = agoraKit.joinChannel(byToken: nil, channelId: channel, info: nil, uid: 0, joinSuccess: nil)
        if code == 0 {
            setIdleTimerActive(false)
            agoraKit.setEnableSpeakerphone(true)
        }
        
        return code
    }
    
    public func leaveMediaChannel() {
        agoraKit.leaveChannel(nil)
    }
    
    public func setupRemoteVideo(withUid uid: UInt, toView view: UIView, withStreamType type: AgoraVideoStreamType) {
        let canvas = AgoraRtcVideoCanvas()
        canvas.view = view
        canvas.uid = uid
        canvas.renderMode = .hidden
        
        agoraKit.setupRemoteVideo(canvas)
        agoraKit.setRemoteVideoStream(uid, type: type)
    }
    
    public func switchCamera() {
        agoraKit.switchCamera()
    }
    
    public func muteAllRemoteAudio(_ mute: Bool) {
        agoraKit.muteAllRemoteAudioStreams(mute)
    }
    
    public func muteAllRemoteVideo(_ mute: Bool) {
        agoraKit.muteAllRemoteVideoStreams(mute)
    }
    
    public func muteRemoteAudioStream(_ uid: UInt, mute: Bool) {
        agoraKit.muteRemoteAudioStream(uid, mute: mute)
    }
    
    public func muteRemoteVideoStream(_ uid: UInt, mute: Bool) {
        agoraKit.muteRemoteVideoStream(uid, mute: mute)
    }
}

extension MediaManager: AgoraRtcEngineDelegate {
    func rtcEngine(_ engine: AgoraRtcEngineKit, didJoinChannel channel: String, withUid uid: UInt, elapsed: Int) {
        self.delegate?.mediaMangaer?(self, didJoinChannel: channel, withUid: uid)
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, didLeaveChannelWith stats: AgoraChannelStats) {
        self.delegate?.mediaManagerDidLeaveChannel?(self)
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, didJoinedOfUid uid: UInt, elapsed: Int) {
        self.delegate?.mediaManager?(self, didJoinedOfUid: uid)
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, didOfflineOfUid uid: UInt, reason: AgoraUserOfflineReason) {
        delegate?.mediaManager?(self, didOfflineOfUid: uid)
    }
}

/// AgoraMediaMetadataDelegate and AgoraMediaMetadataDataSource
extension MediaManager : AgoraMediaMetadataDelegate, AgoraMediaMetadataDataSource {
    func metadataMaxSize() -> Int {
        // the data to send should not exceed this size
        return MAX_META_LENGTH
    }
    
    /// Callback when the SDK is ready to send metadata.
    /// You need to specify the metadata in the return value of this method.
    /// Ensure that the size of the metadata that you specify in this callback does not exceed the value set in the metadataMaxSize callback.
    /// @param timestamp The timestamp (ms) of the current metadata.
    /// @return The metadata that you want to send in the format of Data
    func readyToSendMetadata(atTimestamp timestamp: TimeInterval) -> Data? {
        guard let metadata = self.metadata else {return nil}
        
        // clear self.metadata to nil after any success send to avoid redundancy
        self.metadata = nil
        
        if(metadata.count > MAX_META_LENGTH) {
            //if data exceeding limit, return nil to not send anything
            return nil
        }
        self.metadata = nil
        return metadata
    }
    
    /// Callback when the local user receives the metadata.
    /// @param data The received metadata.
    /// @param uid The ID of the user who sends the metadata.
    /// @param timestamp The timestamp (ms) of the received metadata.
    func receiveMetadata(_ d: Data, fromUser uid: Int, atTimestamp timestamp: TimeInterval) {
        do {
            let jsonData: NSDictionary = try JSONSerialization.jsonObject(with: d, options: .mutableContainers) as! NSDictionary
            print(jsonData)
            if jsonData["type"] as! String == "prod" {
                let prodData = jsonData["product"] as! NSDictionary
                delegate?.mediaManager?(self, didReceiveSEI: prodData, type: .prod)
                
            } else if jsonData["type"] as! String == "quiz" {
                let quizData = jsonData
                delegate?.mediaManager?(self, didReceiveSEI: quizData, type: .quiz)
            }
        } catch {
            print(error)
        }
    }
    
}

private extension MediaManager {
    func setIdleTimerActive(_ active: Bool) {
        UIApplication.shared.isIdleTimerDisabled = !active
    }
}
