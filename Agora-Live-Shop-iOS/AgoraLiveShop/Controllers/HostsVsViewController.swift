//
//  HostsVsViewController.swift
//  AgoraLiveShop
//
//  Created by CavanSu on 2019/1/13.
//  Copyright © 2019 Agora.io. All rights reserved.
//

import UIKit

class HostsVsViewController: UIViewController {
    @IBOutlet weak var redView: UIView!
    @IBOutlet weak var blueView: UIView!
    @IBOutlet weak var containerView: UIView!
    @IBOutlet weak var redWidth: NSLayoutConstraint!
    
    @IBOutlet weak var leftLabel: UILabel!
    @IBOutlet weak var rightLabel: UILabel!
    
    
    
    var leftAmount: Int = 1647 {
        didSet {
            updateWidth()
        }
    }
    
    var rightAmount: Int = 1309 {
        didSet {
           updateWidth()
        }
    }
    
    var isFirstLayout: Bool = false
    
    override func viewDidLoad() {
        super.viewDidLoad()
    }
    
    override func viewDidLayoutSubviews() {
        super.viewDidLayoutSubviews()
        
        if isFirstLayout == false {
            updateWidth()
            isFirstLayout = true
        }
    }
    
    @IBAction func doTapPressed(_ sender: UITapGestureRecognizer) {
        let point = sender.location(in: sender.view)
        
        guard let senderView = sender.view else {
            return
        }
        
        if point.x > CGFloat(senderView.bounds.width * 0.5) {
            rightAmount += 1
        } else {
            leftAmount += 1
        }
    }
}

private extension HostsVsViewController {
    func updateWidth() {
        leftLabel.text = "\(leftAmount)"
        rightLabel.text = "\(rightAmount)"
        
        let containerWidth = self.containerView.frame.width
        let width = CGFloat(leftAmount) / CGFloat(rightAmount + leftAmount) * containerWidth
        redWidth.constant = width
    }
}
