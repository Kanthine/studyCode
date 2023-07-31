#import "AAPLConfig.h"

@import MetalKit;

@interface AAPLRenderer : NSObject

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView;

- (void)drawSceneToView:(nonnull MTKView *)view;

- (void)drawableSizeWillChange:(CGSize)size withGBufferStorageMode:(MTLStorageMode)storageMode;

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size;

@end
