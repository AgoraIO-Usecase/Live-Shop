//
//  StringExtension.swift
//  AgoraLiveShop
//
//  Created by CavanSu on 2019/1/13.
//  Copyright © 2019 Agora.io. All rights reserved.
//

import UIKit

extension String {
    func rect(font: UIFont, limitWidth: CGFloat? = nil, limitHeight: CGFloat? = nil) -> CGRect {
        let nsText = self as NSString
        
        let size = CGSize(width: limitWidth ?? 0, height: limitHeight ?? 0)
        
        let rect = nsText.boundingRect(with: size,
                                       options: [.usesLineFragmentOrigin],
                                       attributes: [NSAttributedString.Key.font: font], context: nil)
        
        return rect
    }
    
    static func intConvertedToDollarsString(value: Int) -> String {
        let string = "\(value)"
        var temp: String = ""
        var index = 0
        
        let length = string.count
        
        var tempArray = [String]()
        
        for char in string.reversed() {
            let start = temp.startIndex
            temp.insert(char, at: start)
            
            if (index + 1) % 3 == 0, index != 0 {
                tempArray.insert(temp, at: 0)
                temp = ""
            }
            
            index += 1
            
            if index == length {
                tempArray.insert(temp, at: 0)
            }
        }
        
        var final: String = "$"
        
        for (index, item) in tempArray.enumerated() {
            var t = item
            
            if index != tempArray.count - 1 {
                t.append(",")
            }
            
            final.append(t)
        }
        
        return final
    }
}
