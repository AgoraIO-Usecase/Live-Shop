//
//  BarrageViewController.swift
//  AgoraLiveShop
//
//  Created by CavanSu on 2019/1/13.
//  Copyright © 2019 Agora.io. All rights reserved.
//

import UIKit

// MARK: Barrage
protocol BarrageProtocol: NSObjectProtocol {
    func barrageDidLeaveScreen(_ barrage: Barrage)
}

class Barrage: NSObject {
    
    weak var delegate: BarrageProtocol?
    
    var id: Int = 0
    
    private var view: BarrageView?
    private lazy var font: UIFont = {
        let size = arc4random() % 8 + 13
        return UIFont.systemFont(ofSize: CGFloat(size))
    }()
    
    private var xpp: CGFloat =  CGFloat(arc4random() % 3 + 1)
    
    private var w: CGFloat = 0
    private var h: CGFloat = 0
    private var y: CGFloat = 0
    
    private var x: CGFloat = 0 {
        didSet {
            guard let view = view else {
                return
            }
            
            if x > ScreenWidth {
                view.removeFromSuperview()
                delegate?.barrageDidLeaveScreen(self)
                return
            }
            
            view.frame = self.frame
        }
    }
    
    private var frame: CGRect {
        let frame = CGRect(x: self.x, y: self.y, width: self.w, height: self.h)
        return frame
    }
    
    init(text: String, containView: UIView) {
        super.init()
        
        let nsText = text as NSString

        let rect = nsText.boundingRect(with: CGSize(width: 0, height: 0),
                                       options: [.usesLineFragmentOrigin],
                                       attributes: [NSAttributedString.Key.font: font], context: nil)
        self.w = rect.width
        self.h = rect.height
        self.x -= self.w
        self.y = CGFloat(arc4random() % UInt32((containView.bounds.height - self.h)))
        self.id = self.hashValue
        
        let view = BarrageView(frame: self.frame, text: text, font: self.font)
        self.view = view
        containView.addSubview(view)
    }
    
    func lauching() {
        self.x += xpp
    }
}

// MARK: BarrageView
class BarrageView: UILabel {
    init(frame: CGRect, text: String, font: UIFont) {
        super.init(frame: frame)
        self.frame = frame
        self.text = text
        self.font = font
        self.textColor = UIColor.white
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
}

// MARK: BarrageViewController
protocol BarrageVCDelegate: NSObjectProtocol {
    func barrageVCDidTapView(_ vc: BarrageViewController)
}

enum TimerStatus {
    case running, stop
}

class BarrageViewController: UIViewController {

    private lazy var timer: CADisplayLink = {
        let timer = CADisplayLink(target: self, selector: #selector(barrageLauching))
        timer.add(to: RunLoop.main, forMode: .common)
        return timer
    }();
    
    weak var deleage: BarrageVCDelegate?

    var timerStatus: TimerStatus = .stop {
        didSet {
            guard oldValue != timerStatus else {
                return
            }
            switch timerStatus {
            case .running: startBarrageLauch()
            case .stop:    stopBarrageLauch()
            }
        }
    }
    
    private var barrageDic = [Int: Barrage]() {
        didSet {
            self.timerStatus = barrageDic.count == 0 ? .stop : .running
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
    }
    
    func wantToLauchABarrage(text: String) {
        let barrage = Barrage(text: text, containView: self.view)
        barrage.delegate = self
        barrageDic[barrage.id] = barrage
    }
    
    @IBAction func doTapPressed(_ sender: UITapGestureRecognizer) {
        deleage?.barrageVCDidTapView(self)
    }
}

private extension BarrageViewController {
    @objc func barrageLauching() {
        for (_, value) in barrageDic {
            value.lauching()
        }
    }
    
    func startBarrageLauch() {
        timer.isPaused = false
    }
    
    func stopBarrageLauch() {
        timer.isPaused = true
    }
}

extension BarrageViewController: BarrageProtocol {
    func barrageDidLeaveScreen(_ barrage: Barrage) {
        barrageDic.removeValue(forKey: barrage.id)
    }
}
