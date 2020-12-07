//
//  LiveRoomViewController.swift
//  AgoraLiveShop
//
//  Created by ZhangJi on 2018/9/30.
//  Copyright © 2018 ZhangJi. All rights reserved.
//

import UIKit
import AgoraRtcKit

class LiveRoomViewController: UIViewController {
    
    @IBOutlet weak var chatTextField: UITextField!
    @IBOutlet weak var hostContainView: UIView!  // view to show the host

    var hostView: UIView?
    var productView: ProductDisplayView?
    var questionView: QuestionView?
    
    var fullView: UIView?
    
    var isQuestionViewShow: Bool {
        return self.questionView != nil
    }
    
    var isProductViewShow: Bool {
        return self.productView != nil
    }
    
    var isFullViewMode: Bool {
        return fullView != nil
    }
    
    var mediaManager: MediaManager!
    
    var channelName: String!
    
    let chatViewHight: CGFloat = 200

    override func viewDidLoad() {
        super.viewDidLoad()
        
        initLiveView()
        loadMedia()
    }
    
    override func viewDidDisappear(_ animated: Bool) {
        mediaManager.leaveMediaChannel()
    }
    
    var question: Question? {
        didSet {
            self.showQuestionView()
        }
    }
    
    var product: Product? {
        didSet {
            showProductView()
        }
    }
    
    @IBAction func doBackPressed(_ sender: UIButton) {
        self.dismiss(animated: true, completion: nil)
    }
}

// MARK: Media
extension LiveRoomViewController {
    func loadMedia() {
        mediaManager = MediaManager.shared()
        mediaManager.delegate = self
        let code = mediaManager.joinMediaChannel("liveshopdemo")
        if code == 0 {
            print("Join media channel success")
        } else {
            print("Join media channel failed with code: \(code)")
        }
    }
}

extension LiveRoomViewController: MediaManagerDelegate {
    func mediaMangaer(_ manager: MediaManager, didJoinChannel channel: String, withUid uid: UInt) {

    }
    
    func mediaManager(_ manager: MediaManager, didJoinedOfUid uid: UInt) {
        if uid == winHostUid {
            let hostView = UIView(frame: hostContainView.frame)
            hostContainView.addSubview(hostView)
            mediaManager.setupRemoteVideo(withUid: uid, toView: hostView, withStreamType: .high)
            
            self.hostView = hostView
        } else {
            mediaManager.muteRemoteVideoStream(uid, mute: true)
            mediaManager.muteRemoteAudioStream(uid, mute: true)
        }
    }
    
    func mediaManagerDidLeaveChannel(_ manager: MediaManager) {
        
    }
    
    func mediaManager(_ manager: MediaManager, didReceiveSEI sei: NSDictionary, type: SeiType) {
        switch type {
        case .prod:
            self.product = Product(product: sei)
        case .quiz:
            self.question = Question(question: sei)
        }
    }
    
    func mediaManager(_ manager: MediaManager, didOfflineOfUid uid: UInt) {
        if uid == winHostUid {
            hostView?.removeFromSuperview()
        }
    }
}

extension LiveRoomViewController {
    func initLiveView() {
        chatTextField.layer.borderWidth = 1
        chatTextField.layer.borderColor = UIColor.white.cgColor
        
        addGestureRecognizer()
    }
    
    func addGestureRecognizer() {
        let doubleTapGestureRecognizer = UITapGestureRecognizer(target: self, action: #selector(handleDoubleTap(_:)))
        doubleTapGestureRecognizer.numberOfTapsRequired = 2
        view.addGestureRecognizer(doubleTapGestureRecognizer)
        view.isUserInteractionEnabled = true
    }
    
    // When double tap the product host view this method will be called
    @objc func handleDoubleTap(_ sender: UITapGestureRecognizer) {
        if !isProductViewShow {
            // If no product showing, do nothing
            return
        }
        if isFullViewMode {
            // If is in full view mode, transform the product host view to samll
            let location = sender.location(in: self.view)
            if (fullView?.frame.contains(location))! {
                let transformFrame = productView?.prodLiveView.convert((productView?.prodLiveView.bounds)!, to: self.view)
                
                UIView.animate(withDuration: 0.3, animations: {
                    self.fullView?.frame = transformFrame!
                }) { (_) in
                    self.mediaManager.setupRemoteVideo(withUid: self.product!.streamId, toView: (self.productView?.prodLiveView)!, withStreamType: .low)
                    self.fullView?.removeFromSuperview()
                    self.fullView = nil
                }
            }
        } else {
            // If is in small view mode, transform the product host view to full screen
            let location = sender.location(in: productView)
            if (productView?.prodLiveView.frame.contains(location))! {
                let transformFrame = productView?.prodLiveView.convert((productView?.prodLiveView.bounds)!, to: self.view)
                
                let fullView = UIView(frame: transformFrame!)
                self.mediaManager.setupRemoteVideo(withUid: self.product!.streamId, toView: fullView, withStreamType: .high)
                self.view.addSubview(fullView)

                UIView.animate(withDuration: 0.3) {
                    fullView.frame = CGRect(x: 20, y: 100, width: ScreenWidth - 40, height: ScreenHeight - 160)
                }

                self.fullView = fullView
            }
        }
    }
    
    // This method used to show question View, when the question info received
    func showQuestionView() {
        if isQuestionViewShow {
            self.questionView?.removeFromSuperview()
        }
        
        let questionView = QuestionView.newQuestionView(with: self.question!)
        questionView?.deleagte = self

        questionView?.frame.origin = CGPoint(x: (ScreenWidth - (questionView?.width)!) / 2, y: ScreenHeight)
        self.view.addSubview(questionView!)
        if isProductViewShow {
            if isFullViewMode {
                self.fullView?.removeFromSuperview()
                self.fullView = nil
            }
            UIView.animate(withDuration: 0.4, animations: {
                questionView?.transform = CGAffineTransform(translationX: 0, y: -((questionView?.height)! + 10))
                self.productView?.transform = CGAffineTransform(translationX: 0, y: -((questionView?.height)! + (self.productView?.frame.height)! + 12))
            }) { (_) in
                questionView?.startCounting()
            }
        } else {
            UIView.animate(withDuration: 0.4, animations: {
                questionView?.transform = CGAffineTransform(translationX: 0, y: -((questionView?.height)! + 10))
            }) { (_) in
                questionView?.startCounting()
            }
        }
        
        self.questionView = questionView
    }
    
    // This method used to show product View, when the product info received
    func showProductView() {
        if isProductViewShow {
            if isFullViewMode {
                self.fullView?.removeFromSuperview()
                self.fullView = nil
            }
            self.productView?.removeFromSuperview()
        }
        
        let productView = ProductDisplayView.newProductDisplayView(with: self.product!)
        let productWidth = max(ScreenWidth * 0.8, 300)
        productView?.frame = CGRect(x: (ScreenWidth - productWidth) / 2, y: ScreenHeight, width: productWidth, height: 120)
        
        if isQuestionViewShow {
            self.view.insertSubview(productView!, belowSubview: self.questionView!)
            UIView.animate(withDuration: 0.5) {
                productView?.transform = CGAffineTransform(translationX: 0, y: -((self.questionView?.height)! + (productView?.frame.height)! + 12))
            }
        } else {
            self.view.addSubview(productView!)
            UIView.animate(withDuration: 0.5) {
                productView?.transform = CGAffineTransform(translationX: 0, y: -((productView?.frame.height)! + self.chatViewHight))
            }
        }
        mediaManager.muteRemoteVideoStream(self.product!.streamId, mute: false)
        mediaManager.setupRemoteVideo(withUid: self.product!.streamId, toView: (productView?.prodLiveView)!, withStreamType: .low)
        
        self.productView = productView
    }
    
    // This method used to close question View
    func closeQuestionView() {
        if isProductViewShow {
            UIView.animate(withDuration: 0.5, animations: {
                self.questionView?.transform = .identity
                self.productView?.transform = CGAffineTransform(translationX: 0, y: -((self.productView?.frame.height)! + self.chatViewHight))
            }, completion: { (_) in
                self.questionView?.removeFromSuperview()
                self.questionView = nil
            })
        } else {
            UIView.animate(withDuration: 0.5, animations: {
                self.questionView?.transform = .identity
            }, completion: { (_) in
                self.questionView?.removeFromSuperview()
                self.questionView = nil
            })
        }
    }
    
    // This method used to close product View
    func closeProductView() {
        UIView.animate(withDuration: 0.5, animations: {
            self.productView?.transform = .identity
        }) { (_) in
            self.productView?.removeFromSuperview()
            self.productView = nil
        }
    }
}

extension LiveRoomViewController: QuestionViewDeleagte {
    func questionViewDidClosed(_ questionView: QuestionView) {
        self.closeQuestionView()
    }
}
