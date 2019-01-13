//
//  ResultAlertViewController.swift
//  AgoraLiveShop
//
//  Created by CavanSu on 2019/1/12.
//  Copyright © 2019 Agora.io. All rights reserved.
//

import UIKit

enum ResultType {
    case success, fail
    
    var title: String {
        switch self {
        case .success: return "You won!"
        case .fail:    return "You lost!"
        }
    }
}

class ResultAlertViewController: UIViewController {

    @IBOutlet weak var imageView: UIImageView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
    }
    
    @IBAction func doOkPressed(_ sender: UIButton) {
        
    }
}
