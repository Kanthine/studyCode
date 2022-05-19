//
//  GLResourceManager.m
//  Mist
//
//  Created by 苏莫离 on 2019/10/17.
//

#import "GLResourceManager.h"
#include <iostream>
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>



GLuint GLResourceManager::initTexture(const string& name) {
    GLuint m_gridTexture;//定义纹理ID
    // Load the texture.
    glGenTextures(1, &m_gridTexture);//产生纹理对象索引
    glBindTexture(GL_TEXTURE_2D, m_gridTexture);
    //设置纹理对象的缩放过滤
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    
    @autoreleasepool {
        //将c++字符串转换为Objective-C字符串对象
        NSString* basePath = [[NSString alloc] initWithUTF8String:name.c_str()];
//        NSBundle* mainBundle = [NSBundle mainBundle];
//        NSString* fullPath = [mainBundle pathForResource:basePath ofType:@"png"];//获取PNG文件的全路径符
//        UIImage* uiImage = [[UIImage alloc] initWithContentsOfFile:fullPath];//创建UIImage对象
        UIImage *uiImage = [UIImage imageNamed:basePath];
        CGImageRef cgImage = uiImage.CGImage;//从UIImage中获取内部CGImage对象
        float x = CGImageGetWidth(cgImage);//从内部CGImage对象中获取图像尺寸
        float y = CGImageGetHeight(cgImage);
        //从CGImage中创建CFData对象
        CFDataRef m_imageData = CGDataProviderCopyData(CGImageGetDataProvider(cgImage));
        void* pixels = (void*) CFDataGetBytePtr(m_imageData);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        
        CFRelease(m_imageData);//释放CFData对象
    }
    
    return m_gridTexture;//返回纹理ID
}

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


string GLResourceManager::loadObjScript(const string& name) {
    NSString* basePath = [[NSString alloc] initWithUTF8String:name.c_str()];
    NSBundle* mainBundle = [NSBundle mainBundle];
    //获取txt文件路径
    NSString *txtPath = [mainBundle pathForResource:basePath ofType:@"obj"];
    //将txt到string对象中，编码类型为NSUTF8StringEncoding
    NSError *error;
    
    unsigned long encode = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_18030_2000);


    NSString *nr = [[NSString alloc] initWithContentsOfFile:txtPath encoding:encode error:&error];
    if (error) {
        NSLog(@"error ===== %@",error);
    }
    const char* scriptNr=[nr UTF8String];
    return string(scriptNr);
}


vector<vector<float>> GLResourceManager::loadLandforms(const string& name) {
    vector<vector<float>> yArray;
    @autoreleasepool {
        NSString* basePath = [[NSString alloc] initWithUTF8String:name.c_str()];
        UIImage *uiImage = [UIImage imageNamed:basePath];
        CGImageRef cgImage = uiImage.CGImage;
        float width = CGImageGetWidth(cgImage);
        float height = CGImageGetHeight(cgImage);
        size_t bitsPerComponent = CGImageGetBitsPerComponent(cgImage);
        int bytePerPixel = 4;
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        int pixelCount = width * height;

        uint8_t *rgba = (uint8_t *)malloc(pixelCount * bytePerPixel);
        CGContextRef context = CGBitmapContextCreate(rgba,width,height,
                                                     bitsPerComponent,
                                                     bytePerPixel * width,
                                                     colorSpace,
                                                     kCGImageAlphaNoneSkipLast);
        CGContextDrawImage(context, CGRectMake(0, 0, width, height), cgImage);
        CGContextRelease(context);
        
        for(int i = 0; i < width; i++) {
            vector<float> y_1Array;
            for(int j = 0; j < height; j++) {
                int index = (width * i + j) * 4;
                int r = rgba[index], g = rgba[index + 1], b = rgba[index + 2];
                float h = (r + g + b) / 3.0;
                float result = h * LAND_HIGHEST / 255.0 - LAND_HIGH_ADJUST;
                y_1Array.push_back(result);
            }
            yArray.push_back(y_1Array);
        }
        free(rgba);
    }
    
    return yArray;//返回纹理ID
}
