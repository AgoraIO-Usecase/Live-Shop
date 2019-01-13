//
//  QuestionViewController.swift
//  AgoraLiveShop
//
//  Created by CavanSu on 2019/1/13.
//  Copyright © 2019 Agora.io. All rights reserved.
//

import UIKit

enum Result {
    case right, wrong
    
    var content: String {
        switch self {
        case .right: return "You are right!"
        case .wrong: return "Your are wrong!"
        }
    }
}

protocol QuestionVCDelegate: NSObjectProtocol {
    func questionVC(_ vc: QuestionViewController, didSelect result: Result)
    func questionVCViewWiilDismiss(_ vc: QuestionViewController)
}

class QuestionViewController: UIViewController {
    
    @IBOutlet weak var contentLabel: UILabel!
    
    weak var delegate: QuestionVCDelegate?
    
    var question: Question?
    
    static func create(with question: Question) -> QuestionViewController {
        let story = UIStoryboard(name: "Main", bundle: nil)
        let vc = story.instantiateViewController(withIdentifier: "QuestionViewController") as! QuestionViewController
        vc.question = question
        vc.view.frame = CGRect(x: 0, y: 0, width: ScreenWidth, height: ScreenHeight)
        return vc
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        initQuestionContent()
    }
    
    @IBAction func doYesPressed(_ sender: UIButton) {
        checkResult(select: true)
    }
    
    @IBAction func doNoPressed(_ sender: UIButton) {
        checkResult(select: false)
    }
}

private extension QuestionViewController {
    func initQuestionContent() {
        guard let question = question else {
            return
        }
        self.contentLabel.text = question.content
    }
    
    func checkResult(select: Bool) {
        guard let question = question else {
            return
        }
        
        let userSelect = select ? 2 : 1
        
        var result: Result
        
        if question.result == userSelect {
            result = .right
        } else {
            result = .wrong
        }
        
        delegate?.questionVC(self, didSelect: result)
        delegate?.questionVCViewWiilDismiss(self)
    }
}
