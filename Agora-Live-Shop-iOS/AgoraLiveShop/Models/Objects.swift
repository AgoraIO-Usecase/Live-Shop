//
//  bjects.swift
//  AgoraLiveShop
//
//  Created by Zhang Ji on 2018/10/9.
//  Copyright © 2018 Agora.io. All rights reserved.
//

import Foundation

struct Question{
    // qusetion
    var id: UInt  //id of question
    var content: String  //question body content
    var options: [String]  //options array, total count matches with total property
    var timeout: UInt  //timeout of this question
    var total: UInt  //total question number
    // result
    var result: UInt  //result of current quiz
    var spread: Dictionary<String, UInt>? //spread of answer, -1 means number of users whose answer are not received
    
    init(question: NSDictionary) {
 
        let quiz = question["question"] as! NSDictionary
        self.id = quiz["id"] as! UInt
        self.content = quiz["content"] as! String
        self.options = quiz["options"] as! [String]
        self.timeout = quiz["timeout"] as! UInt
        self.total = quiz["total"] as! UInt
        
        let result = question["result"] as! NSDictionary
        
        self.result = result["result"] as! UInt
        self.spread = (result["spread"] as! Dictionary<String, UInt>)
    }
}
