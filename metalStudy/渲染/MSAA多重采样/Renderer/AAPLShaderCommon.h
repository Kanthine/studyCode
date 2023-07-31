#pragma once

struct FragData {
    half4 resolvedColor [[color(0)]];
};

half3 tonemapByLuminance(half3 inColor);
