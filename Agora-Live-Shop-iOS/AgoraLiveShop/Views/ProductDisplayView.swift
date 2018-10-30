//
//  ProductDisplayView.swift
//  AgoraLiveShop
//
//  Created by ZhangJi on 2018/10/5.
//  Copyright © 2018 ZhangJi. All rights reserved.
//

import UIKit

class ProductDisplayView: UIView {

    @IBOutlet weak var prodLiveView: UIView!
    @IBOutlet weak var titleLabel: UILabel!
    @IBOutlet weak var variantsLabel: UILabel!
    @IBOutlet weak var detaileLabel: UILabel!
    @IBOutlet weak var originalLabel: UILabel!
    @IBOutlet weak var groupLabel: UILabel!
    
    static func newProductDisplayView(with product: Product) -> ProductDisplayView? {
        let nibView = Bundle.main.loadNibNamed("ProductDisplayView", owner: nil, options: nil)
        if let view = nibView?.first as? ProductDisplayView {
            view.titleLabel.text = product.title
            var variants = product.variants[0]["name"] as! String
            let values = product.variants[0]["values"] as! [String]
            variants += ": "
            for (index, value) in values.enumerated() {
                if index != values.count - 1 {
                    variants += value + ","
                } else {
                     variants += value
                }
            }
//            for（index, value) in values do {
//                variants += value + ","
//            }
            view.variantsLabel.text = variants
            view.detaileLabel.text = "Product Detaile  ·  \(product.sold) sold"
            view.originalLabel.text = String(product.price)
            view.groupLabel.text = String(product.groupDeal)
            return view
        }
        return nil
    }

}
