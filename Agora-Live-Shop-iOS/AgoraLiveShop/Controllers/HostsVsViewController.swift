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
    
    private var leftTaped: Bool = false {
        didSet {
            if leftTaped {
                leftAmount += 1
            } else {
                leftAmount -= 1
            }
        }
    }
    
    private var rightTaped: Bool = false {
        didSet {
            if rightTaped {
                rightAmount += 1
            } else {
                rightAmount -= 1
            }
        }
    }
    
    private var leftAmount: Int = 16 {
        didSet {
            updateWidth()
        }
    }
    
    private var rightAmount: Int = 13 {
        didSet {
           updateWidth()
        }
    }
    
    private var isFirstLayout: Bool = false
    
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
            rightTaped = !rightTaped
        } else {
            leftTaped = !leftTaped
        }
    }
}

private extension HostsVsViewController {
    func updateWidth() {
        leftLabel.text = "\(leftAmount)"
        rightLabel.text = "\(rightAmount)"
        
        let containerWidth = self.containerView.frame.width
        var coefficient = CGFloat(leftAmount) / CGFloat(rightAmount + leftAmount)
        
        if coefficient > 1 {
            coefficient = 1
        }
        
        let width = coefficient * containerWidth
        redWidth.constant = width
    }
}
