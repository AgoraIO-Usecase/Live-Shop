//
//  MediaManager.swift
//  AgoraLiveShop
//
//  Created by Zhang Ji on 2018/10/9.
//  Copyright © 2018 Agora.io. All rights reserved.
//


//
import UIKit
import AgoraRtcEngineKit

@objc enum SeiType: NSInteger {
    case prod, quiz
}

@objc protocol MediaManagerDelegate: NSObjectProtocol {
    @objc optional func mediaMangaer(_ manager: MediaManager, didJoinChannel channel: String, withUid uid: UInt)
    @objc optional func mediaManagerDidLeaveChannel(_ manager: MediaManager)
    @objc optional func mediaManager(_ manager: MediaManager, didReceiveSEI sei: NSDictionary, type: SeiType)
   
    @objc optional func mediaManager(_ manager: MediaManager, didJoinedOfUid uid: UInt)
    @objc optional func mediaManager(_ manager: MediaManager, didOfflineOfUid uid: UInt)
    @objc optional func mediaManager(_ manager: MediaManager, firstRemoteVideoDecodedOfUid uid: UInt, size: CGSize, elapsed: Int)
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
}

extension MediaManager {
    public static func shared() -> MediaManager {
        return manager
    }
    
    @discardableResult public func joinMediaChannel(_ channel: String) -> Int32{
        agoraKit = AgoraRtcEngineKit.sharedEngine(withAppId: KeyCenter.AppId, delegate: self)
        agoraKit.enableVideo()
        agoraKit.setChannelProfile(channelProfile)
        agoraKit.setClientRole(clientRole)
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
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, firstRemoteVideoDecodedOfUid uid: UInt, size: CGSize, elapsed: Int) {
        self.delegate?.mediaManager?(self, firstRemoteVideoDecodedOfUid: uid, size: size, elapsed: elapsed)
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, didReceiveSEI sei: String) {
        let data = sei.data(using: String.Encoding.utf8)
        do {
            let jsonData: NSDictionary = try JSONSerialization.jsonObject(with: data!, options: .mutableContainers) as! NSDictionary
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
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, didOfflineOfUid uid: UInt, reason: AgoraUserOfflineReason) {
        delegate?.mediaManager?(self, didOfflineOfUid: uid)
    }
}

private extension MediaManager {
    func setIdleTimerActive(_ active: Bool) {
        UIApplication.shared.isIdleTimerDisabled = !active
    }
}
