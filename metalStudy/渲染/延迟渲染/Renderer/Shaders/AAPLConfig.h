#ifndef __METAL_VERSION__
#include <TargetConditionals.h>
#endif

// Must account for simulator target in both application ObjC code and Metal
// shader code so use __APPLE_EMBEDDED_SIMULATOR__ to check if building for
// simulator target in Metal shader code
#if TARGET_OS_SIMULATOR || defined(__APPLE_EMBEDDED_SIMULATOR__)
#define TARGET_OS_SIMULATOR 1
#endif

/// 将眼睛的深度值写入 G-Buffer 的深度组件；
// 这允许延迟传递计算眼睛空间碎片的位置更容易，以便应用照明。
// 当禁用时，屏幕深度被写入g-buffer深度组件，并且需要一个额外的从屏幕空间到眼睛空间的反变换来计算延迟传递中的照明贡献。
// When enabled, writes depth values in eye space to the g-buffer depth component.
// This allows the deferred pass to calculate the eye space fragment position more easily in order to apply lighting.
// When disabled, the screen depth is written to the g-buffer depth component and an extra inverse transform from screen space to eye space is necessary to calculate lighting contributions in the deferred pass.
#define USE_EYE_DEPTH              1

// When enabled, uses the stencil buffer to avoid execution of lighting
// calculations on fragments that do not intersect with a 3D light volume.
// When disabled, all fragments covered by a light in screen space will have
// lighting calculations executed. This means that considerably more fragments
// will have expensive lighting calculations executed than is actually
// necessary.
#define LIGHT_STENCIL_CULLING      1

// Enables toggling of buffer examination mode at runtime. Code protected by
// this definition is only useful to examine or debug parts of the underlying
// implementation.
#define SUPPORT_BUFFER_EXAMINATION 1
