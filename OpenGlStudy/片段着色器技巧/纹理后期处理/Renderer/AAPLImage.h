#import <Foundation/Foundation.h>

@interface AAPLImage : NSObject

-(nullable instancetype) initWithTGAFileAtLocation:(nonnull NSURL *)location;

@property (nonatomic, readonly) NSUInteger      width;
@property (nonatomic, readonly) NSUInteger      height;
@property (nonatomic, readonly, nonnull) NSData *data;

@end
