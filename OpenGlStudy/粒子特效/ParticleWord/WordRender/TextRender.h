//
//  TextRender.h
//  Graphics
//
//  Created by 苏沫离 on 2020/9/25.
//

@import MetalKit;

NS_ASSUME_NONNULL_BEGIN

@interface TextRender : NSObject
<MTKViewDelegate>

@property (nonatomic, strong) NSString *word;

- (instancetype)initWithMTKView:(nonnull MTKView *)mtkView;

@end

NS_ASSUME_NONNULL_END



