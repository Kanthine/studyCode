//
//  ViewController.swift
//  ffPushStream
//
//  Created by i7y on 2022/4/13.
//

import Cocoa

class ViewController: NSViewController {
    
    var button : NSButton?
    override func viewDidLoad() {
        super.viewDidLoad()
        button = NSButton.init(title: "开始推流", target: self, action: #selector(buttonClick(sender:)))
        button!.tag = 1
        button!.frame = NSRect(x: 100, y: 100, width: 100, height: 50)
        self.view.addSubview(button!)
    }
    
    
    @objc func buttonClick(sender : NSButton) {
        if sender.tag == 1 {
            sender.tag = 2
            sender.title = "推流中"
                       
            let thread = Thread.init {
                print("开始推流 \(Thread.current)")
                push_stream("\(NSHomeDirectory())/Desktop/Demo.flv", "rtmp://localhost/Desktop/FLV")
            }
            thread.start();
            
        } else {
            sender.tag = 1
            sender.title = "开始推流"

        }
    }
}

