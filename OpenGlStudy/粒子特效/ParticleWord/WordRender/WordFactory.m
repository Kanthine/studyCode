//
//  WordFactory.m
//  ParticleWord
//
//  Created by i7y on 2022/2/10.
//

#import "WordFactory.h"

vector_float2 *getPointsByCGPath(CGPathRef cgPath, int *length) {
    __block int capacity = 10, index = 0;
    __block vector_float2 *pointArray = calloc(capacity, sizeof(vector_float2));
    CGPathApplyWithBlock(cgPath, ^(const CGPathElement * _Nonnull element) {
        if (index + 3 >= capacity) {
            capacity += 3;
            pointArray = realloc(pointArray, capacity * sizeof(vector_float2));
        }
        
        CGPathElementType type = element -> type;
        CGPoint *points = element->points;
        
        if (type != kCGPathElementCloseSubpath) {
            pointArray[index++] = (vector_float2){points[0].x, points[0].y};
            if ((type != kCGPathElementAddLineToPoint) && (type != kCGPathElementMoveToPoint)) {
                pointArray[index++] = (vector_float2){points[1].x, points[1].y};
            }
        }
        if (type == kCGPathElementAddCurveToPoint) {
            pointArray[index++] = (vector_float2){points[2].x, points[2].y};
        }
    });
    *length = index;
    return pointArray;
}

CGMutablePathRef getPathWithAttributedString(CFAttributedStringRef string) {
    
    CGMutablePathRef mutablePath = CGPathCreateMutable();
    CGFloat x_offet = 0;
    
    CTLineRef lineRef = CTLineCreateWithAttributedString(string);
    CFArrayRef glyphRuns = CTLineGetGlyphRuns(lineRef);
    int runCount = (int)CFArrayGetCount(glyphRuns);
    
    for (int j = 0; j < runCount ; j ++) {
        CTRunRef run = CFArrayGetValueAtIndex(glyphRuns, j);
        int glyphCount = (int)CTRunGetGlyphCount(run);
        CGGlyph *glyphBuffer = calloc(glyphCount, sizeof(CGGlyph));
        CTRunGetGlyphs(run, CFRangeMake(0, glyphCount), glyphBuffer);
        CFDictionaryRef attributes = CTRunGetAttributes(run);
        if (attributes) {
            if (CFDictionaryContainsKey(attributes, kCTFontAttributeName)) {
                CTFontRef fontRef = CFDictionaryGetValue(attributes, kCTFontAttributeName);
                
                /// 整体边界 = (指定字体, 绘制方向, glyphs, 接收字形边界, 字形总数)
                CGRect *rects = calloc(glyphCount, sizeof(CGRect));
                CGRect bigRect = CTFontGetBoundingRectsForGlyphs(fontRef, kCTFontOrientationDefault, glyphBuffer, rects, glyphCount);
                
                /// 字形宽度
                CGSize *advances = calloc(glyphCount, sizeof(CGSize));
                /// 整体宽度 = (指定字体, 绘制方向, glyphs, 接收字形宽度, 字形总数)
                double bigAdvances = CTFontGetAdvancesForGlyphs(fontRef, kCTFontOrientationDefault, glyphBuffer, advances, glyphCount);
                
                double height = CTFontGetAscent(fontRef) + abs(CTFontGetDescent(fontRef)) + CTFontGetLeading(fontRef);
                for (int i = 0; i < glyphCount; i++) {
                    CGGlyph glyph = glyphBuffer[i];
                    NSLog(@"(%f, %f)",rects[i].origin.x,rects[i].origin.y);
                    CGPathRef glyphPath = CTFontCreatePathForGlyph(fontRef, glyph, NULL);
                    CGAffineTransform matrix = CGAffineTransformTranslate(CGAffineTransformIdentity, x_offet, rects[i].origin.y);
                    CGPathAddPath(mutablePath, &matrix, glyphPath);
                    x_offet += CGRectGetMaxX(rects[i]);
                }
            }
        }
        
        CGFloat width = CTRunGetTypographicBounds(run, CFRangeMake(0, 0), NULL, NULL, NULL);//获取 CTRun 的宽度
    }
    
//    int pointCount = 0;
//    vector_float2 *pathPoints = getPointsByCGPath(mutablePath, &pointCount);
//    for (int i = 0; i < pointCount; i++) {
//        NSLog(@"====== (%f, %f)",pathPoints[i].x, pathPoints[i].y);
//    }
    return mutablePath;
}
