//
//  DMSDFGen.m
//  DMBulletSDK
//
//  Created by liguoqing on 2022/8/11.
//

#import <math.h>
#import <stdlib.h>

#import "DMSDFGen.h"

@implementation DMSDFGen


#define SDF_MAX_PASSES 10        // Maximum number of distance transform passes
#define SDF_SLACK 0.001f        // Controls how much smaller the neighbour value must be to cosnider, too small slack increse iteration count.
#define SDF_SQRT2 1.4142136f    // sqrt(2)
#define SDF_BIG 1e+37f            // Big value used to initialize the distance field.

struct SDFpoint {
    float x,y;
};

static float sdf__clamp01(float x)
{
    return x < 0.0f ? 0.0f : (x > 1.0f ? 1.0f : x);
}

static float sdf__edgedf(float gx, float gy, float a)
{
    float df, a1;
    if ((gx == 0) || (gy == 0)) {
        // Either A) gu or gv are zero, or B) both
        // Linear approximation is A) correct or B) a fair guess
        df = 0.5f - a;
    } else {
        // Everything is symmetric wrt sign and transposition,
        // so move to first octant (gx>=0, gy>=0, gx>=gy) to
        // avoid handling all possible edge directions.
        gx = fabsf(gx);
        gy = fabsf(gy);
        if (gx < gy) {
            float temp = gx;
            gx = gy;
            gy = temp;
        }
        a1 = 0.5f*gy/gx;
        if (a < a1) { // 0 <= a < a1
            df = 0.5f*(gx + gy) - sqrtf(2.0f*gx*gy*a);
        } else if (a < (1.0-a1)) { // a1 <= a <= 1-a1
            df = (0.5f-a)*gx;
        } else { // 1-a1 < a <= 1
            df = -0.5f*(gx + gy) + sqrt(2.0f*gx*gy*(1.0f-a));
        }
    }
    return df;
}

static float sdf__distsqr(struct SDFpoint* a, struct SDFpoint* b)
{
    float dx = b->x - a->x, dy = b->y - a->y;
    return dx*dx + dy*dy;
}

+ (void)sdfCoverageToDistanceFieldWith:(unsigned char*)outData
//                             outstride:(int)outstride
                                srcImg:(const unsigned char *)img
                                 width:(int)width
                                height:(int)height
//                                stride:(int)stride
{
    int outstride = width;
    int stride = width;
    int x, y;

    // Zero out borders
    for (x = 0; x < width; x++)
        outData[x] = 0;
    for (y = 1; y < height; y++) {
        outData[y*outstride] = 0;
        outData[width-1+y*outstride] = 0;
    }
    for (x = 0; x < width; x++)
        outData[x+(height-1)*outstride] = 0;

    for (y = 1; y < height-1; y++) {
        for (x = 1; x < width-1; x++) {
            int k = x + y * stride;
            float d, gx, gy, glen, a, a1;

            // Skip flat areas.
            if (img[k] == 255) {
                outData[x+y*outstride] = 255;
                continue;
            }
            if (img[k] == 0) {
                // Special handling for cases where full opaque pixels are next to full transparent pixels.
                // See: https://github.com/memononen/SDF/issues/2
                int he = img[k-1] == 255 || img[k+1] == 255;
                int ve = img[k-stride] == 255 || img[k+stride] == 255;
                if (!he && !ve) {
                    outData[x+y*outstride] = 0;
                    continue;
                }
            }

            gx = -(float)img[k-stride-1] - SDF_SQRT2*(float)img[k-1] - (float)img[k+stride-1] + (float)img[k-stride+1] + SDF_SQRT2*(float)img[k+1] + (float)img[k+stride+1];
            gy = -(float)img[k-stride-1] - SDF_SQRT2*(float)img[k-stride] - (float)img[k-stride+1] + (float)img[k+stride-1] + SDF_SQRT2*(float)img[k+stride] + (float)img[k+stride+1];
            a = (float)img[k]/255.0f;
            gx = fabsf(gx);
            gy = fabsf(gy);
            if (gx < 0.0001f || gy < 0.000f) {
                d = (0.5f - a) * SDF_SQRT2;
            } else {
                glen = gx*gx + gy*gy;
                glen = 1.0f / sqrtf(glen);
                gx *= glen;
                gy *= glen;
                if (gx < gy) {
                    float temp = gx;
                    gx = gy;
                    gy = temp;
                }
                a1 = 0.5f*gy/gx;
                if (a < a1) { // 0 <= a < a1
                    d = 0.5f*(gx + gy) - sqrtf(2.0f*gx*gy*a);
                } else if (a < (1.0-a1)) { // a1 <= a <= 1-a1
                    d = (0.5f-a)*gx;
                } else { // 1-a1 < a <= 1
                    d = -0.5f*(gx + gy) + sqrt(2.0f*gx*gy*(1.0f-a));
                }
            }
            d *= 1.0f / SDF_SQRT2;
            outData[x+y*outstride] = (unsigned char)(sdf__clamp01(0.5f - d) * 255.0f);
        }
    }
}

+ (void)sdfBuildDistanceFieldWith:(unsigned char*)outData
//                        outstride:(int)outstride
                           radius:(float)radius
                           srcImg:(const unsigned char *)img
                            width:(int)width
                           height:(int)height
//                           stride:(int)stride
{
    int outstride = width;
    int stride = width;
    unsigned char* temp = (unsigned char*)malloc(width*height*sizeof(float)*3);
    if (temp == NULL) return;

    int i, x, y, pass;
    float scale;
    float* tdist = (float*)&temp[0];
    struct SDFpoint* tpt = (struct SDFpoint*)&temp[width * height * sizeof(float)];

    // Initialize buffers
    for (i = 0; i < width*height; i++) {
        tpt[i].x = 0;
        tpt[i].y = 0;
        tdist[i] = SDF_BIG;
    }

    // Calculate position of the anti-aliased pixels and distance to the boundary of the shape.
    for (y = 1; y < height-1; y++) {
        for (x = 1; x < width-1; x++) {
            int tk, k = x + y * stride;
            struct SDFpoint c = { (float)x, (float)y };
            float d, gx, gy, glen;

            // Skip flat areas.
            if (img[k] == 255) continue;
            if (img[k] == 0) {
                // Special handling for cases where full opaque pixels are next to full transparent pixels.
                // See: https://github.com/memononen/SDF/issues/2
                int he = img[k-1] == 255 || img[k+1] == 255;
                int ve = img[k-stride] == 255 || img[k+stride] == 255;
                if (!he && !ve) continue;
            }

            // Calculate gradient direction
            gx = -(float)img[k-stride-1] - SDF_SQRT2*(float)img[k-1] - (float)img[k+stride-1] + (float)img[k-stride+1] + SDF_SQRT2*(float)img[k+1] + (float)img[k+stride+1];
            gy = -(float)img[k-stride-1] - SDF_SQRT2*(float)img[k-stride] - (float)img[k-stride+1] + (float)img[k+stride-1] + SDF_SQRT2*(float)img[k+stride] + (float)img[k+stride+1];
            if (fabsf(gx) < 0.001f && fabsf(gy) < 0.001f) continue;
            glen = gx*gx + gy*gy;
            if (glen > 0.0001f) {
                glen = 1.0f / sqrtf(glen);
                gx *= glen;
                gy *= glen;
            }

            // Find nearest point on contour.
            tk = x + y * width;
            d = sdf__edgedf(gx, gy, (float)img[k]/255.0f);
            tpt[tk].x = x + gx*d;
            tpt[tk].y = y + gy*d;
            tdist[tk] = sdf__distsqr(&c, &tpt[tk]);
        }
    }

    // Calculate distance transform using sweep-and-update.
    for (pass = 0; pass < SDF_MAX_PASSES; pass++){
        int changed = 0;

        // Bottom-left to top-right.
        for (y = 1; y < height-1; y++) {
            for (x = 1; x < width-1; x++) {
                int k = x+y*width, kn, ch = 0;
                struct SDFpoint c = { (float)x, (float)y }, pt = {0, 0};
                float pd = tdist[k], d;
                // (-1,-1)
                kn = k - 1 - width;
                if (tdist[kn] < pd) {
                    d = sdf__distsqr(&c, &tpt[kn]);
                    if (d + SDF_SLACK < pd) {
                        pt = tpt[kn];
                        pd = d;
                        ch = 1;
                    }
                }
                // (0,-1)
                kn = k - width;
                if (tdist[kn] < pd) {
                    d = sdf__distsqr(&c, &tpt[kn]);
                    if (d + SDF_SLACK < pd) {
                        pt = tpt[kn];
                        pd = d;
                        ch = 1;
                    }
                }
                // (1,-1)
                kn = k + 1 - width;
                if (tdist[kn] < pd) {
                    d = sdf__distsqr(&c, &tpt[kn]);
                    if (d + SDF_SLACK < pd) {
                        pt = tpt[kn];
                        pd = d;
                        ch = 1;
                    }
                }
                // (-1,0)
                kn = k - 1;
                if (tdist[kn] < tdist[k]) {
                    d = sdf__distsqr(&c, &tpt[kn]);
                    if (d + SDF_SLACK < pd) {
                        pt = tpt[kn];
                        pd = d;
                        ch = 1;
                    }
                }
                if (ch) {
                    tpt[k] = pt;
                    tdist[k] = pd;
                    changed++;
                }
            }
        }

        // Top-right to bottom-left.
        for (y = height-2; y > 0 ; y--) {
            for (x = width-2; x > 0; x--) {
                int k = x+y*width, kn, ch = 0;
                struct SDFpoint c = { (float)x, (float)y }, pt = {0, 0};
                float pd = tdist[k], d;
                // (1,0)
                kn = k + 1;
                if (tdist[kn] < pd) {
                    d = sdf__distsqr(&c, &tpt[kn]);
                    if (d + SDF_SLACK < pd) {
                        pt = tpt[kn];
                        pd = d;
                        ch = 1;
                    }
                }
                // (-1,1)
                kn = k - 1 + width;
                if (tdist[kn] < pd) {
                    d = sdf__distsqr(&c, &tpt[kn]);
                    if (d + SDF_SLACK < pd) {
                        pt = tpt[kn];
                        pd = d;
                        ch = 1;
                    }
                }
                // (0,1)
                kn = k + width;
                if (tdist[kn] < pd) {
                    d = sdf__distsqr(&c, &tpt[kn]);
                    if (d + SDF_SLACK < pd) {
                        pt = tpt[kn];
                        pd = d;
                        ch = 1;
                    }
                }
                // (1,1)
                kn = k + 1 + width;
                if (tdist[kn] < pd) {
                    d = sdf__distsqr(&c, &tpt[kn]);
                    if (d + SDF_SLACK < pd) {
                        pt = tpt[kn];
                        pd = d;
                        ch = 1;
                    }
                }
                if (ch) {
                    tpt[k] = pt;
                    tdist[k] = pd;
                    changed++;
                }
            }
        }

        if (changed == 0) break;
    }

    // Map to good range.
    scale = 1.0f / radius;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            float d = sqrtf(tdist[x+y*width]) * scale;
            if (img[x+y*stride] > 127) d = -d;
            outData[x+y*outstride] = (unsigned char)(sdf__clamp01(0.5f - d*0.5f) * 255.0f);
        }
    }

    free(temp);
}

@end
