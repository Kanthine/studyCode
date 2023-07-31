#import "DMMaskManager.h"
#import "zlib.h"

@interface NSData (DMTool)
- (NSInteger) dm_read:(NSRange *) range;
+ (NSData *)gzipData:(NSData *)pUncompressedData;
+ (NSData *)uncompressZippedData:(NSData *)compressedData;
@end
@implementation NSData (DMTool)

- (NSInteger) dm_read:(NSRange *) range {
    NSInteger length = (*range).length;
    NSInteger location = (*range).location;
    
    if ((location+length) < self.length) {
        
        Byte buf[(length)];
        [self getBytes:buf range:*range];
        NSInteger value = 0;
        for (NSInteger i = 0; i < length; i++) {
            value += buf[i] << i*8;
        }
        *range = NSMakeRange((*range).location+(*range).length, (*range).length);
        return value;
    } else {
        return 0;
    }
}

+ (NSData *)gzipData:(NSData *)pUncompressedData {
    if (!pUncompressedData || [pUncompressedData length] == 0) {
        NSLog(@"%s: Error: Can't compress an empty or nil NSData object",__func__);
        return nil;
    }
    
    z_stream zlibStreamStruct;
    zlibStreamStruct.zalloc = Z_NULL;
    zlibStreamStruct.zfree = Z_NULL;
    zlibStreamStruct.opaque = Z_NULL;
    zlibStreamStruct.total_out = 0;
    zlibStreamStruct.next_in = (Bytef *)[pUncompressedData bytes];
    zlibStreamStruct.avail_in = [pUncompressedData length];
    
    int initError = deflateInit2(&zlibStreamStruct, Z_DEFAULT_COMPRESSION, Z_DEFLATED, (15+16), 8, Z_DEFAULT_STRATEGY);
    if (initError != Z_OK) {
        NSString *errorMsg = nil;
        switch (initError) {
            case Z_STREAM_ERROR:
                errorMsg = @"Invalid parameter passed in to function.";
                break;
            case Z_MEM_ERROR:
                errorMsg = @"Insufficient memory.";
                break;
            case Z_VERSION_ERROR:
                errorMsg = @"The version of zlib.h and the version of the library linked do not match.";
                break;
            default:
                errorMsg = @"Unknown error code.";
                break;
        }
        NSLog(@"%s:deflateInit2() Error: \"%@\" Message: \"%s\"",__func__,errorMsg,zlibStreamStruct.msg);
        return nil;
    }
    
    NSMutableData *compressedData = [NSMutableData dataWithLength:[pUncompressedData length] * 1.01 + 21];
    
    int deflateStatus;
    do {
        zlibStreamStruct.next_out = [compressedData mutableBytes] + zlibStreamStruct.total_out;
        zlibStreamStruct.avail_out = [compressedData length] - zlibStreamStruct.total_out;
        deflateStatus = deflate(&zlibStreamStruct, Z_FINISH);
                
    } while (deflateStatus == Z_OK);
    
    if (deflateStatus != Z_STREAM_END)
    {
      NSString *errorMsg = nil;
      switch (deflateStatus) {
          case Z_ERRNO:
              errorMsg = @"Error occured while reading file.";
              break;
          case Z_STREAM_ERROR:
              errorMsg = @"The stream state was inconsistent (e.g., next_in or next_out was NULL).";
                break;
          case Z_DATA_ERROR:
              errorMsg = @"The deflate data was invalid or incomplete.";
              break;
          case Z_MEM_ERROR:
              errorMsg = @"Memory could not be allocated for processing.";
              break;
          case Z_BUF_ERROR:
              errorMsg = @"Ran out of output buffer for writing compressed bytes.";
              break;
          case Z_VERSION_ERROR:
              errorMsg = @"The version of zlib.h and the version of the library linked do not match.";
              break;
          default:
              errorMsg = @"Unknown error code.";
              break;
      }
      NSLog(@"%s:zlib error while attempting compression: \"%@\" Message: \"%s\"", __func__, errorMsg, zlibStreamStruct.msg);
      deflateEnd(&zlibStreamStruct);
      return nil;
    }
    
    deflateEnd(&zlibStreamStruct);
    [compressedData setLength:zlibStreamStruct.total_out];
    NSLog(@"%s: Compressed file from %d B to %d B", __func__, [pUncompressedData length], [compressedData length]);
    return compressedData;
}


+ (NSData *)uncompressZippedData:(NSData *)compressedData  {
    if ([compressedData length] == 0) return compressedData;
    unsigned full_length = [compressedData length];
    unsigned half_length = [compressedData length] / 2;
    NSMutableData *decompressed = [NSMutableData dataWithLength: full_length + half_length];
    BOOL done = NO;
    int status;
    z_stream strm;
    strm.next_in = (Bytef *)[compressedData bytes];
    strm.avail_in = [compressedData length];
    strm.total_out = 0;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    if (inflateInit2(&strm, (15+32)) != Z_OK) return nil;
    while (!done) {
        // Make sure we have enough room and reset the lengths.
        if (strm.total_out >= [decompressed length]) {
            [decompressed increaseLengthBy: half_length];
        }
        strm.next_out = [decompressed mutableBytes] + strm.total_out;
        strm.avail_out = [decompressed length] - strm.total_out;
        // Inflate another chunk.
        status = inflate (&strm, Z_SYNC_FLUSH);
        if (status == Z_STREAM_END) {
            done = YES;
        } else if (status != Z_OK) {
            break;
        }
    }
    if (inflateEnd (&strm) != Z_OK) return nil;
    // Set real length.
    if (done) {
        [decompressed setLength: strm.total_out];
        return [NSData dataWithData: decompressed];
    } else {
        return nil;
    }
}

@end









#pragma mark - DMMaskSection

@interface DMSingleMask : NSObject
@property (nonatomic ,assign) NSUInteger position;
@property (nonatomic ,assign) NSUInteger startFrame;
@property (nonatomic ,assign) NSUInteger endFrame;
@end
@implementation DMSingleMask
@end


@interface DMMaskSection : NSObject

/**********************************压缩包里解析出来的数据********************************************/
//视频描述信息
@property (nonatomic ,assign) NSUInteger desc;

//蒙版图片
@property (nonatomic ,assign) NSUInteger maskImage;

//蒙版位置信息起始字节
@property (nonatomic ,assign) NSUInteger startMsg;

//视频ID
@property (nonatomic ,assign)  NSUInteger videoID;

//段落总数
@property (nonatomic ,assign)  NSUInteger sectionSum;

//当前段号
@property (nonatomic ,assign)  NSUInteger index;

//FPS
@property (nonatomic ,assign)  NSUInteger fps;

//视频宽
@property (nonatomic ,assign)  NSUInteger videoWidth;

//视频高
@property (nonatomic ,assign)  NSUInteger videoHeight;

//蒙版宽
@property (nonatomic ,assign)  NSUInteger maskWidth;

//蒙版高
@property (nonatomic ,assign)  NSUInteger maskHeight;

//存放单个mask的数据信息
@property (nonatomic ,copy) NSArray <DMSingleMask *> *maskList;

/******************************************************************************/

@property (nonatomic ,strong) NSData *data;

+ (instancetype) sectionWithData:(NSData *) data;

- (uint32_t *)getBitmapData:(int *)width height:(int *)height;

@end


static CGFloat const kBaseDataLength = 4;



@interface DMMaskSection ()

@property (nonatomic ,assign) NSInteger pointer;
@property (nonatomic ,strong) DMSingleMask *currentMask;
@property (nonatomic ,strong) NSMutableArray *zipColors;

@end

@implementation DMMaskSection

+ (instancetype)sectionWithData:(NSData *) data {
    DMMaskSection *section = [DMMaskSection new];
    if (![data isKindOfClass:[NSData class]]) {
        return section;
    }
    section.data = data;
    
    NSRange range = NSMakeRange(0, kBaseDataLength);
    section.desc = [data dm_read:&range];
    section.maskImage = [data dm_read:&range];
    section.startMsg = [data dm_read:&range];
    section.videoID = [data dm_read:&range];
    section.sectionSum = [data dm_read:&range];
    section.index = [data dm_read:&range];
    section.fps = [data dm_read:&range];
    section.videoWidth = [data dm_read:&range];
    section.videoHeight = [data dm_read:&range];
    section.maskWidth = [data dm_read:&range];
    section.maskHeight = [data dm_read:&range];
    
    NSMutableArray *list = [NSMutableArray array];
    NSRange msgRange = NSMakeRange(section.startMsg, kBaseDataLength);
    while (true) {
        //取三个数据，起始，结束帧，mask数据在data中的位置
        NSInteger dataCount = 3;
        if (msgRange.location+msgRange.length*dataCount >= data.length) {
            break;
        }
        DMSingleMask *mask = [DMSingleMask new];
        mask.startFrame = [data dm_read:&msgRange];
        mask.endFrame = [data dm_read:&msgRange];
        mask.position = [data dm_read:&msgRange];
        [list addObject:mask];
    }
    section.maskList = [self sortWithList:list];
    
    return section;
}

+ (NSArray *)sortWithList:(NSArray *) list {
    NSArray *comparatorSortedArray = [list sortedArrayUsingComparator:^NSComparisonResult(id  _Nonnull obj1, id  _Nonnull obj2) {
        DMSingleMask *mask1 = obj1;
        DMSingleMask *mask2 = obj2;
        return [@(mask1.startFrame) compare:@(mask2.startFrame)];
    }];
    return comparatorSortedArray;
}

#pragma  mark - Public Method

- (UInt32 *)getMaskImageWidth:(size_t *)width height:(size_t*)height {
    [self p_maskWithFrame];
    UInt32 *image = [self p_imageFromMask:self.currentMask width:width height:height];
    [self.zipColors removeAllObjects];
    return image;
}

#pragma  mark - Private Method

- (void) p_maskWithFrame {
    NSInteger frame = 2548; /// 双人
//    frame = 2456; /// 单人
    for (NSInteger i = self.pointer; i < self.maskList.count; i++) {
        DMSingleMask *mask = self.maskList[i];
        if (frame < mask.startFrame) {
            break;
        }
        if (frame <= mask.endFrame) {
            self.pointer = i++;
            self.currentMask = mask;
            break;
        }
    }
}

- (UInt32 *)p_imageFromMask:(DMSingleMask *)mask width:(size_t *)width height:(size_t*)height {
    NSInteger pixNum = self.maskWidth * self.maskHeight;
    NSInteger index = 0;
    NSRange range = NSMakeRange(mask.position, kBaseDataLength);
    while (true) {
        if (range.location+range.length >= self.data.length) {
            break;
        }
        
        NSInteger zipColor = [self.data dm_read:&range];
        [self.zipColors addObject:@(zipColor)];
        index += labs(zipColor);
        if (index == pixNum) {
            break;
        }
    }
    return [self p_createBitmapWithWidth:width height:height];
}

- (UInt32 *)p_createBitmapWithWidth:(size_t *)width height:(size_t *)height {
    NSInteger maskWidth = self.maskWidth;
    NSInteger maskHeight = self.maskHeight;
    NSInteger videoWidth = self.videoWidth;
    NSInteger videoHeight = self.videoHeight;
    
    
    CGFloat screenWidth = 852, screenHeight = 393;
    
    //bitmap其实就是视频内容等比例缩小，scale就是这个比例的值
    CGFloat scale;
    
    if (videoHeight/screenHeight > videoWidth/screenWidth) {
        scale = maskWidth/screenWidth;      //上下两边超出屏幕外
    } else {
        scale = maskHeight/screenHeight;    //左右两边超出屏幕外
    }
    
    //按照bitmap缩小的比例，计算出bitmap相对在弹幕上的位置
    NSInteger x = ceil(scale*0);
    NSInteger y = ceil(scale*0);
    NSInteger bitmapW = maskWidth;
    NSInteger bitmapH = maskHeight;
    
    NSInteger pixNum = bitmapW * bitmapH;
    UInt32 * pixels;
    pixels = malloc(pixNum*sizeof(UInt32));
    memset(pixels, 0xffffffff, pixNum*sizeof(UInt32));
    
    NSInteger count = self.zipColors.count;
    NSInteger playerPixNum = maskWidth * maskHeight;
    NSInteger index = 0;
    
    //根据行程编码生成bitmap，这部分参考遮罩数据生产方给的算法文档
    for (NSInteger i = 0; i < count; i++) {
        NSInteger sameBit = [self.zipColors[i] integerValue];
        NSInteger bitCount = labs(sameBit);
        
        for (NSInteger j = 0; j < bitCount; j++) {
            NSInteger maskX = index % maskWidth;
            NSInteger maskY = index / maskWidth;
            NSInteger loc = (maskY+y) * bitmapW + maskX+x;
            if (index++ >= playerPixNum || loc >= pixNum) {
                break;
            }
            if (sameBit > 0 ) {
                pixels[loc] = 0x00000000; /// 被遮挡的部分
            }
        }
        
        if (index >= playerPixNum) {
            break;
        }
    }
    
    *width = bitmapW;
    *height = bitmapH;
    return pixels;
}

#pragma  mark - getter / setter

- (NSMutableArray *) zipColors {
    if (!_zipColors) {
        _zipColors = [NSMutableArray array];
    }
    return _zipColors;
}

@end


#pragma mark - DMMaskManager

@implementation DMMaskManager

+ (uint32_t *)getBitdataWithData:(size_t *)widthP height:(size_t *)heightP {
    NSString *path = [NSBundle.mainBundle pathForResource:@"mask_1" ofType:@"z"];
    NSData *data = [NSData dataWithContentsOfFile:path];
    data = [NSData uncompressZippedData:data];
    DMMaskSection *section = [DMMaskSection sectionWithData:data];
    return  [section getMaskImageWidth:widthP height:heightP];
//    UIImage *maskImage = [section getMaskImageWidth:widthP height:heightP];
//    CGImageRef imageRef = maskImage.CGImage;
//    size_t width = CGImageGetWidth(imageRef);
//    size_t height = CGImageGetHeight(imageRef);
//    size_t bitsPerPixel = 32;
//    size_t bitsPerComponent = 8;
//    size_t bytesPerPixel = bitsPerPixel / bitsPerComponent;
//    size_t bytesPerRow = width * bytesPerPixel;
//    size_t bufferLength = bytesPerRow * height;
//    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
//    if(!colorSpace) {
//        return nil;
//    }
//
//    uint32_t *bitmapData = (uint32_t *)malloc(bufferLength);
//    if(!bitmapData) {
//        CGColorSpaceRelease(colorSpace);
//        return nil;
//    }
//
//    CGContextRef context = CGBitmapContextCreate(bitmapData,width,height,bitsPerComponent,bytesPerRow,colorSpace, kCGImageAlphaPremultipliedLast);    // RGBA
//    if(!context) {
//        CGColorSpaceRelease(colorSpace);
//        free(bitmapData);
//        return nil;
//    }
//    CGContextDrawImage(context, CGRectMake(0, 0, width, height), imageRef);
//    CGContextRelease(context);
//    CGColorSpaceRelease(colorSpace);
//
//    *widthP = width;
//    *heightP = height;
//    return bitmapData;
}

@end
