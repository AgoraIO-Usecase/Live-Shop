//
//  AlertUtil.swift
//  AgoraHQ
//
//  Created by Zhang Ji on 09/01/2018.
//  Copyright © 2018 Agora.io. All rights reserved.
//

import UIKit

/*-----------------------------------------------------------------------------

   Show Alert Info

 -----------------------------------------------------------------------------*/

class AlertUtil: NSObject {
    static func showAlert(message: String) {
        let alert = UIAlertController(title: nil, message: message, preferredStyle: .alert)
        alert.addAction(UIAlertAction(title: "OK", style: .cancel, handler: nil))
        
        let rootVC = UIApplication.shared.keyWindow?.rootViewController
        let topVC = rootVC?.presentedViewController != nil ? rootVC?.presentedViewController : rootVC
        
        DispatchQueue.main.async {
            topVC?.present(alert, animated: true, completion: nil)
        }
    }
}
