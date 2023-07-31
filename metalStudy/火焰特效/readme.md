// 噪声设置：火焰效果

const float Power = 5.059;
const float MaxLength = 0.9904;
const float Dumping = 10.0;

vec3 hash3(vec3 p) {
    p = vec3(dot(p, vec3(127.1, 311.7, 74.7)),
            dot(p, vec3(269.5, 183.3, 246.1)),
            dot(p, vec3(113.5, 271.9, 124.6)));

    return -1.0 + 2.0 * fract(sin(p) * 43758.5453123);
}

float noise(vec3 p) {
    vec3 i = floor(p);
    vec3 f = fract(p);

    vec3 u = f * f * (3.0 - 2.0 * f);

    float n0 = dot(hash3(i + vec3(0.0, 0.0, 0.0)), f - vec3(0.0, 0.0, 0.0));
    float n1 = dot(hash3(i + vec3(1.0, 0.0, 0.0)), f - vec3(1.0, 0.0, 0.0));
    float n2 = dot(hash3(i + vec3(0.0, 1.0, 0.0)), f - vec3(0.0, 1.0, 0.0));
    float n3 = dot(hash3(i + vec3(1.0, 1.0, 0.0)), f - vec3(1.0, 1.0, 0.0));
    float n4 = dot(hash3(i + vec3(0.0, 0.0, 1.0)), f - vec3(0.0, 0.0, 1.0));
    float n5 = dot(hash3(i + vec3(1.0, 0.0, 1.0)), f - vec3(1.0, 0.0, 1.0));
    float n6 = dot(hash3(i + vec3(0.0, 1.0, 1.0)), f - vec3(0.0, 1.0, 1.0));
    float n7 = dot(hash3(i + vec3(1.0, 1.0, 1.0)), f - vec3(1.0, 1.0, 1.0));

    float ix0 = mix(n0, n1, u.x);
    float ix1 = mix(n2, n3, u.x);
    float ix2 = mix(n4, n5, u.x);
    float ix3 = mix(n6, n7, u.x);

    float ret = mix(mix(ix0, ix1, u.y), mix(ix2, ix3, u.y), u.z) * 0.5 + 0.5;
    return ret * 2.0 - 1.0;
}

float udSegment(vec2 p, vec2 start, vec2 end) {
    vec2 dir = start - end;
    float len = length(dir);
    dir /= len;

    vec2 proj = clamp(dot(p - end, dir), 0.0, len) * dir + end;
    return distance(p, proj);
}

/**
 * Rune function by Otavio Good.
 * https://www.shadertoy.com/view/MsXSRn
 */
float rune(vec2 uv) {
    float ret = 100.0;
    vec2 newSeed = vec2(0.0);
    for (int i = 0; i < 4; i++) {
        // generate seeded random line endPoints - just about any texture_ should work.
        // Hopefully this randomness will work the same on all GPUs (had some trouble with that)
        vec2 posA = vec2(0.0);
        vec2 posB = vec2(0.0);

        // expand the range and mod it to get a nicely distributed random number - hopefully. :)    
        // each rune touches the edge of its box on all 4 sides
        if (i == 0) {
            posA.y = 0.0;
        }
        if (i == 1) {
            posA.x = 0.999;
        }
        if (i == 2) {
            posA.x = 0.0;
        }
        if (i == 3) {
            posA.y = 0.999;
        }

        // snap the random line endpoints to a grid 2x3
        vec2 snaps = vec2(2.0, 3.0);
    
        if (distance(posA, posB) < 0.0001) {
            continue; // eliminate dots.
        }

        // Dots (degenerate lines) are not cross-GPU safe without adding 0.001 - divide by 0 error.
        float d = udSegment(uv, posA, posB + 0.001);
        ret = min(ret, d);
    }
    return ret;
}

float normalizeScalar(float value, float max) {
    return clamp(value, 0.0, max) / max;
}

vec3 color(vec2 p) {
    vec3 coord = vec3(p, iTime * 0.25);
    float n = abs(noise(coord));
    n += 0.5 * abs(noise(coord * 2.0));
    n += 0.25 * abs(noise(coord * 4.0));
    n += 0.125 * abs(noise(coord * 8.0));
    
    n *= (100.001 - Power);
    float dist = rune(p * 0.15);
    float k = normalizeScalar(dist, MaxLength);
    n *= dist / pow(1.001 - k, Dumping);
    
    vec3 col = vec3(1.0, 0.25, 0.08) / n;
    return pow(col, vec3(2.0));
}

vec3 render(vec2 coord) {
    vec3 col = color(coord * 6.5);
    return vec3(clamp(col, 0.0, 1.0));
}

void mainImage(out vec4 fragColor, in vec2 fragCoord) {
    vec2 q = fragCoord.xy / iResolution.xy;
    vec2 coord = 2.0 * q - 1.0;
    coord.x *= iResolution.x / iResolution.y;
    
    vec3 col = render(coord);
    col = pow(col, vec3(0.4545));
    fragColor = vec4(col, 1.0);
}
