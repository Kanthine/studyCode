//
//  ViewController.swift
//  ffApp
//
//  Created by 苏莫离 on 2018/3/24.
//

import Cocoa

class ViewController: NSViewController {
    var button : NSButton?
    override func viewDidLoad() {
        super.viewDidLoad()
        button = NSButton.init(title: "开始录制", target: self, action: #selector(buttonClick(sender:)))
        button!.tag = 1
        button!.frame = NSRect(x: 100, y: 100, width: 100, height: 50)
        self.view.addSubview(button!)
                
        let thread = Thread.init {
            /// /Users/i7y/Desktop/活死人军团1080.mp4
//            cut_video(10, 100, "\(NSHomeDirectory())/Desktop/音视频编辑/Demo.mov", "\(NSHomeDirectory())/Desktop/音视频编辑/10_100.mov")
            cut_video(10, 20, "\(NSHomeDirectory())/Desktop/2.mov", "\(NSHomeDirectory())/Desktop/10_100.mov")
//            mp4_convert_flv("\(NSHomeDirectory())/Desktop/音视频编辑/活死人军团1080.mp4", "\(NSHomeDirectory())/Desktop/1.mp4")
        }
        thread.start();
    }
    
    
    @objc func buttonClick(sender : NSButton) {
        if sender.tag == 1 {
            sender.tag = 2
            sender.title = "录制中"
                       
            let thread = Thread.init {
                print("开始录制 \(Thread.current)")
//                start_audioRecord("\(NSHomeDirectory())/Desktop/audio.pcm")
//                start_resample("\(NSHomeDirectory())/Desktop/audio.pcm");
//                start_audioCoder("\(NSHomeDirectory())/Desktop/audio.aac")
                
//                start_videoRecord("\(NSHomeDirectory())/Desktop/7.yuv")
                start_videoCoder("\(NSHomeDirectory())/Desktop/0.yuv", "\(NSHomeDirectory())/Desktop/0.h264")
            }
            thread.start();
            
        } else {
            sender.tag = 1
            sender.title = "开始录制"
//            stop_audioRecord()
//            stop_resample();
//            stop_audioCoder()
//            stop_videoRecord()
            stop_videoCoder()
        }
    }
}

