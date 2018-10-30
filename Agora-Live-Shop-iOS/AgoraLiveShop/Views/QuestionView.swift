//
//  QuestionView.swift
//  AgoraHQ
//
//  Created by ZhangJi on 08/01/2018.
//  Copyright © 2018 ZhangJi. All rights reserved.
//

import UIKit

struct Answer {
    var type: Bool!
    var ratio: Double!
}

protocol QuestionViewDeleagte: NSObjectProtocol {
    func questionViewDidClosed(_ questionView: QuestionView)
}

class QuestionView: UIView {

    @IBOutlet weak var questionLable: UILabel!
    @IBOutlet weak var circleView: CircleProgressView!
    
    weak var deleagte: QuestionViewDeleagte?
    
    var width: CGFloat = ScreenWidth - 40
    var height: CGFloat = ScreenHeight
    
    var channelName: String!
    var buttons = [UIButton]()
    var answerViews = [UIView]()
    var myAnswer: Int = -1
    
    var timer: Timer!
    var answerTime: UInt = 15
    var pastTime: UInt = 15
    
    var question: Question!
    
    static func newQuestionView(with question: Question) -> QuestionView? {
        let nibView = Bundle.main.loadNibNamed("QuestionView", owner: nil, options: nil)
        if let view = nibView?.first as? QuestionView {
            let id = UserDefaults.standard.integer(forKey: "sid") + 1
            
            view.question = question
            view.questionLable.text = "\(id) . " + question.content
            view.answerTime = question.timeout
            view.pastTime = question.timeout
            view.setViewWith(qusetin: question.content, answers: question.options)
    
            return view
        }
        return nil
    }
    
    override init(frame: CGRect) {
        super.init(frame: frame)
    }
    
    required init?(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)
    }
    
    @IBAction func doCancelPressed(_ sender: UIButton) {
        deleagte?.questionViewDidClosed(self)
    }
    
    // Start counting down
    func startCounting() {
        timer = Timer.scheduledTimer(timeInterval: 1, target: self, selector: #selector(changeTime), userInfo: nil, repeats: true)
    }
    
    @objc func changeTime() {
        self.circleView.percent = 1 - Float(pastTime - 1) / Float(answerTime)
        if pastTime > 1 {
            pastTime -= 1
            self.circleView.centerLabel.text = String(pastTime)
        } else {
            pastTime = answerTime
            timer?.invalidate()
        }
    }
    
    // Stop counting down
    func stopCounting() {
        timer.invalidate()
    }
}

private extension QuestionView {
    // This func used to init question card with the question
    func setViewWith(qusetin: String, answers: Array<String>) {
        var answerHeights = [CGFloat]()
        var allHeight: CGFloat = 0
        let height = getLabHeigh(labelStr: qusetin, width: width - 100) + CGFloat(170)
        answerHeights.append(height)
        allHeight += height
        for answer in answers {
            let height = getLabHeigh(labelStr: answer, width: width - 100) + CGFloat(20)
            answerHeights.append(height)
            allHeight += height
        }
        self.height = CGFloat(10 * answers.count + 20) + allHeight
        self.frame.size = CGSize(width: width, height: self.height)
        
        self.setAnswers(answers, answerHeights: answerHeights, enable: true)
    }
    
    // This func used to calculate the height of the UILabel
    func getLabHeigh(labelStr:String, width:CGFloat) -> CGFloat {
        let size = labelStr.boundingRect(with: CGSize(width: width, height: 8000), options: .usesLineFragmentOrigin, attributes: [NSAttributedString.Key.font: UIFont.systemFont(ofSize: 17)], context: nil)
        
        return size.height
    }
    
    // This func used to init the answer list with the options
    func setAnswers(_ answers: Array<String>, answerHeights: Array<CGFloat>, enable: Bool) {
        var height: CGFloat = answerHeights[0]
        for (index, answer) in answers.enumerated() {
            let answerView = UIView(frame: CGRect(x: 30.0, y: height, width: CGFloat(self.frame.width - 60), height: answerHeights[index + 1]))
            let answerButton = UIButton(frame: CGRect(x: 20, y: 0, width: answerView.frame.width - 40, height: answerView.frame.height))
            answerView.layer.cornerRadius = 20
            answerView.layer.masksToBounds = true
            answerView.layer.borderWidth = 1
            answerView.layer.borderColor = buttonColor.cgColor
            answerButton.setTitle(answer, for: .normal)
            answerButton.contentHorizontalAlignment = .left
            answerButton.setTitleColor(UIColor.black, for: .normal)
            answerButton.titleLabel?.numberOfLines = 0
            answerButton.isEnabled = enable
            answerButton.tag = index
            answerButton.addTarget(self, action: #selector(sendAnswer(_:)), for: .touchUpInside)
            answerView.addSubview(answerButton)
            buttons.append(answerButton)
            answerViews.append(answerView)
            self.addSubview(answerView)
            height += answerHeights[index + 1] + 10
        }
        
        circleView.backgroundColor = UIColor.clear
        circleView.progressColor = UIColor.init(hex: 0x302593)
        circleView.progressBackgroundColor = buttonColor
        circleView.progressWidth = 10
        circleView.centerLabel.text = String(answerTime)
        circleView.percent = 0
        circleView.clockwise = false
    }
    
    // When a answer be chose, this func wil be called
    @objc func sendAnswer(_ button: UIButton) {
        self.resetViewWithRightAnswers2()

        self.stopCounting()
        for button in buttons {
            button.isEnabled = false
        }
    }
    
    // This func used to show the result with ratio styel
    func resetViewWithRightAnswers2() {
        guard let spreads = question.spread else { return }
        
        var totalAnswers: UInt = 0
        for item in spreads {
            totalAnswers += item.value
        }
        
        for i in 0..<spreads.count - 1 {
            let ratio = totalAnswers == 0 ? 0.0 : Double(spreads["\(i)"]!) / Double(totalAnswers)
            if i != question.result {
                setBackground(color: wrongColor, of: answerViews[i], with: ratio, count: spreads["\(i)"]!)
            } else {
                setBackground(color: rightColor, of: answerViews[i], with: ratio, count: spreads["\(i)"]!)
            }
        }
    }
    
    // This func used to set the background of option list when the result is displayed
    func setBackground(color: UIColor, of view: UIView, with ratio: Double, count: UInt) {
        let backView = UIView(frame: CGRect(x: 0, y: 0, width: Double(view.frame.width) * ratio, height: Double(view.frame.height)))
        backView.layer.cornerRadius = 20
        backView.layer.masksToBounds = true
        backView.backgroundColor = color
        backView.transform = CGAffineTransform(translationX: -(backView.frame.width), y: 0)
        let label = UILabel(frame: CGRect(x: view.frame.width - 60, y: (view.frame.height - 20) / 2, width: 40, height: 20))
        label.font = UIFont.systemFont(ofSize: 12)
        label.textAlignment = .right
        label.textColor = UIColor.lightGray
        label.text = String(count)
        
        view.addSubview(backView)
        view.insertSubview(backView, at: 0)
        view.addSubview(label)
        
        UIView.animate(withDuration: 0.3) {
            backView.transform = .identity
        }
        
        view.backgroundColor = UIColor.clear
    }
}
