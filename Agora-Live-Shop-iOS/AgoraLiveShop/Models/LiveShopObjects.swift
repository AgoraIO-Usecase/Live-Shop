//
//  LiveShopObjects.swift
//  AgoraLiveShop
//
//  Created by ZhangJi on 2018/10/9.
//  Copyright © 2018 ZhangJi. All rights reserved.
//

import Foundation

/*-----------------------------------------------------------------------------
 
 
 
 -----------------------------------------------------------------------------*/

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

struct Product {
    var id: String // id of product
    var title: String // product title
    var variants: [Dictionary<String, Any>] // product variants
    var sold: Int // total sold number
    var price: Int // original price, 2 decimal place
    var currency: String // currency symbol, applies to price and group deal
    var groupDeal: Int // group deal price
    var streamId: UInt // streamId of product camera
    
    init(product: NSDictionary) {
        self.id = product["id"] as! String
        self.title = product["title"] as! String
        self.variants = product["variants"] as! [Dictionary<String, Any>]
        self.sold = product["sold"] as! Int
        self.price = product["price"] as! Int
        self.currency = product["currency"] as! String
        self.groupDeal =  product["groupDeal"] as! Int
        self.streamId = product["streamId"] as! UInt
    }
}
