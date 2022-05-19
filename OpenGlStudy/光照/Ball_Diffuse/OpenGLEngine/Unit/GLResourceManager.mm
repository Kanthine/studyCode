//
//  GLResourceManager.m
//  Ball
//
//  Created by 苏莫离 on 2019/10/17.
//

#import "GLResourceManager.h"
#include <iostream>
#import <UIKit/UIKit.h>

string GLResourceManager::loadShaderScript(const string& name) {
    NSString* basePath = [[NSString alloc] initWithUTF8String:name.c_str()];
    NSBundle* mainBundle = [NSBundle mainBundle];
    //    获取txt文件路径
    NSString *txtPath = [mainBundle pathForResource:basePath ofType:@"sh"];
    //    将txt到string对象中，编码类型为NSUTF8StringEncoding
    NSString *nr = [[NSString  alloc] initWithContentsOfFile:txtPath encoding:NSUTF8StringEncoding error:nil];
    
    const char* scriptNr=[nr UTF8String];
        
    return string(scriptNr);
}
