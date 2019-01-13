//
//  AmountViewController.swift
//  AgoraLiveShop
//
//  Created by CavanSu on 2019/1/13.
//  Copyright © 2019 Agora.io. All rights reserved.
//

import UIKit

protocol AmountVCDelegate: NSObjectProtocol  {
    func amountVCViewWillMiss(_ vc: AmountViewController)
    func amountVC(_ vc: AmountViewController, didInput amount: Int, team: Team)
}

class AmountViewController: UIViewController {

    @IBOutlet weak var inputTextField: UITextField!
    
    var team: Team?
    
    static func create(for team: Team) -> AmountViewController {
        let story = UIStoryboard(name: "Main", bundle: nil)
        let vc = story.instantiateViewController(withIdentifier: "AmountViewController") as! AmountViewController
        vc.team = team
        vc.view.frame = CGRect(x: 0, y: 0, width: ScreenWidth, height: ScreenHeight)
        return vc
    }
    
    weak var delegate: AmountVCDelegate?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        preferredContentSize = CGSize(width: ScreenWidth, height: ScreenHeight)
    }
    
    @IBAction func doGoPressed(_ sender: UIButton) {
        if let text = inputTextField.text,
            let amount = Int(text.suffix(text.count - 1)),
            let team = self.team {
            delegate?.amountVC(self, didInput: amount, team: team)
        }
        delegate?.amountVCViewWillMiss(self)
    }
    
    @IBAction func doCancelPressed(_ sender: UIButton) {
        delegate?.amountVCViewWillMiss(self)
    }
}

extension AmountViewController: UITextFieldDelegate {
    func textField(_ textField: UITextField, shouldChangeCharactersIn range: NSRange, replacementString string: String) -> Bool {
        if let text = textField.text, text.count + string.count == 1 {
            return false
        }
        return true
    }
}
