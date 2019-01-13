//
//  LiveRoomViewController.swift
//  AgoraLiveShop
//
//  Created by Zhang Ji on 2018/9/30.
//  Copyright © 2018 Agora.io. All rights reserved.
//

import UIKit
import AgoraRtcEngineKit

enum Team {
    case warriors, memphis
}

class LiveRoomViewController: UIViewController {
    
    @IBOutlet weak var rtmpView: UIView!
    @IBOutlet weak var leftVideoView: UIView!
    @IBOutlet weak var rightVideoView: UIView!
    @IBOutlet weak var inputTextField: UITextField!
    @IBOutlet weak var warriorsAmountLabel: UILabel!
    @IBOutlet weak var memphisAmountLabel: UILabel!
    
    @IBOutlet weak var resultLabel: UILabel!
    @IBOutlet weak var resultLabelWidth: NSLayoutConstraint!
    
    weak var barrageVC: BarrageViewController?
    
    var amountVC: AmountViewController?
    var questionVC: QuestionViewController?
    
    var warriorsAmount: Int = 0 {
        didSet {
            let value = String.intConvertedToDollarsString(value: warriorsAmount)
            warriorsAmountLabel.text = value
        }
    }
    
    var memphisAmount: Int = 0 {
        didSet {
            let value = String.intConvertedToDollarsString(value: memphisAmount)
            memphisAmountLabel.text = value
        }
    }
    
    var mediaManager: MediaManager!
    let chatViewHight: CGFloat = 200

    override func viewDidLoad() {
        super.viewDidLoad()
        initViews()
        addKeyboradObserver()
        loadMedia()
    }
    
    override func viewDidDisappear(_ animated: Bool) {
        mediaManager.leaveMediaChannel()
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        guard let identifier = segue.identifier else {
            return
        }
        
        switch identifier {
        case "BarrageViewController":
            let barrageVC = segue.destination as! BarrageViewController
            barrageVC.deleage = self
            self.barrageVC = barrageVC
        default:
            break
        }
    }
    
    @IBAction func doWarriorsPressed(_ sender: UIButton) {
        amountViewShow(for: .warriors)
    }
    
    @IBAction func doMemphisPressed(_ sender: UIButton) {
       amountViewShow(for: .memphis)
    }
}

// MARK: Media
private extension LiveRoomViewController {
    func loadMedia() {
        mediaManager = MediaManager.shared()
        mediaManager.delegate = self
        mediaManager.joinMediaChannel("hh12345")
    }
}

extension LiveRoomViewController: MediaManagerDelegate {
    func mediaMangaer(_ manager: MediaManager, didJoinChannel channel: String, withUid uid: UInt) {
    }
    
    func mediaManager(_ manager: MediaManager, firstRemoteVideoDecodedOfUid uid: UInt, size: CGSize, elapsed: Int) {
        switch uid {
        case 666:
            rtmpView.isHidden = false
            mediaManager.setupRemoteVideo(withUid: uid, toView: rtmpView, withStreamType: .high)
        case 1:
            rightVideoView.isHidden = false
            mediaManager.setupRemoteVideo(withUid: uid, toView: rightVideoView, withStreamType: .high)
        case 2:
            leftVideoView.isHidden = false
            mediaManager.setupRemoteVideo(withUid: uid, toView: leftVideoView, withStreamType: .high)
        default:
            break
        }
    }
    
    func mediaManager(_ manager: MediaManager, didReceiveSEI sei: NSDictionary, type: SeiType) {
        guard type == .quiz else {
            return
        }
        questionViewShow(Question(question: sei))
    }
    
    func mediaManager(_ manager: MediaManager, didOfflineOfUid uid: UInt) {
        switch uid {
        case 666:
            rtmpView.isHidden = true
        case 1:
            rightVideoView.isHidden = true
        case 2:
            leftVideoView.isHidden = true
        default:
            break
        }
    }
}

private extension LiveRoomViewController {
    func initViews() {
        warriorsAmount = 100002
        memphisAmount = 95005
    }
    
    func amountViewShow(for team: Team) {
        let amountVC = AmountViewController.create(for: team)
        amountVC.delegate = self
        self.view.addSubview(amountVC.view)
        self.amountVC = amountVC
    }
    
    func amountViewHidden() {
        guard let amountVC = self.amountVC else {
            return
        }
        amountVC.view.removeFromSuperview()
        self.amountVC = nil
    }
    
    func questionViewShow(_ question: Question) {
        let questionVC = QuestionViewController.create(with: question)
        questionVC.delegate = self
        self.view.addSubview(questionVC.view)
        self.questionVC = questionVC
    }
    
    func questionViewHidden() {
        guard let questionVC = self.questionVC else {
            return
        }
        questionVC.view.removeFromSuperview()
        self.questionVC = nil
    }
    
    func resultLabelShow(result: Result) {
        let content = result.content
        let rect = content.rect(font: resultLabel.font)
        resultLabel.text = content
        
        resultLabelWidth.constant = rect.width + 37
        resultLabel.isHidden = false
        resultLabel.alpha = 0
        
        UIView.animate(withDuration: 0.5, animations: { [unowned self] in
            self.resultLabel.alpha = 1
        }) { (finish) in
            guard finish else {
                return
            }
            
            DispatchQueue.main.asyncAfter(deadline: DispatchTime.now() + 2, execute: {
                self.resultLabelHidden()
            })
        }
    }
    
    func resultLabelHidden() {
        UIView.animate(withDuration: 0.5, animations: { [unowned self] in
            self.resultLabel.alpha = 0
        }) { (finish) in
            guard finish else {
                return
            }
            self.resultLabel.isHidden = true
        }
    }
    
    func addKeyboradObserver() {
        NotificationCenter.default.addObserver(self,
                                               selector: #selector(keyboardFrameWillChange(notification:)),
                                               name: UIResponder.keyboardWillChangeFrameNotification, object: nil)
    }
    
    @objc func keyboardFrameWillChange(notification: NSNotification) {
        if let _ = self.amountVC {
            return
        }
        
        guard let userInfo = notification.userInfo,
        let endKeyboardFrameValue = userInfo[UIResponder.keyboardFrameEndUserInfoKey] as? NSValue,
        let durationValue = userInfo[UIResponder.keyboardAnimationDurationUserInfoKey] as? NSNumber else {
            return
        }
        
        let endKeyboardFrame = endKeyboardFrameValue.cgRectValue
        let duration = durationValue.doubleValue
        
        let isShowing: Bool = endKeyboardFrame.maxY > ScreenHeight ? false : true
        
        UIView.animate(withDuration: duration) { [weak self] in
            guard let strongSelf = self else {
                return
            }
            
            if isShowing {
                let offsetY = strongSelf.inputTextField.frame.maxY - endKeyboardFrame.minY
                strongSelf.inputTextField.transform = CGAffineTransform(translationX: 0, y: -offsetY)
            } else {
                strongSelf.inputTextField.transform = .identity
            }
        }
    }
}

extension LiveRoomViewController: BarrageVCDelegate {
    func barrageVCDidTapView(_ vc: BarrageViewController) {
        if inputTextField.isEditing {
            inputTextField.resignFirstResponder()
        } else {
            inputTextField.becomeFirstResponder()
        }
    }
}

extension LiveRoomViewController: UITextFieldDelegate {
    func textFieldShouldReturn(_ textField: UITextField) -> Bool {
        if let text = textField.text, text.count > 0 {
            textField.text = nil
            barrageVC?.wantToLauchABarrage(text: text)
        }
        
        textField.resignFirstResponder()
        return true
    }
}

extension LiveRoomViewController: AmountVCDelegate {
    func amountVCViewWillMiss(_ vc: AmountViewController) {
        amountViewHidden()
    }
    
    func amountVC(_ vc: AmountViewController, didInput amount: Int, team: Team) {
        switch team {
        case .warriors: warriorsAmount += amount
        case .memphis:  memphisAmount += amount
        }
    }
}

extension LiveRoomViewController: QuestionVCDelegate {
    func questionVC(_ vc: QuestionViewController, didSelect result: Result) {
        resultLabelShow(result: result)
    }
    
    func questionVCViewWiilDismiss(_ vc: QuestionViewController) {
        questionViewHidden()
    }
}
