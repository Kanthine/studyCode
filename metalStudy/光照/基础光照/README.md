# 1、基本光照

现实世界的光照，可以大致认为由三种组成：环境光、散射光、镜面光！

## 1.1、环境光

环境光 (Ambient) 指的是从四面八方照射到物体上，全方位 360° 都均匀的光。其代表的是现实世界中从光源射出，经过多次反射后，各方向基本均匀的光。
环境光最大的特点是不依赖于光源的位置，而且没有方向性，下图简单地说明了这个问题。

![环境光的基本情况](asset/%E7%8E%AF%E5%A2%83%E5%85%89%E7%9A%84%E5%9F%BA%E6%9C%AC%E6%83%85%E5%86%B5.png)


环境光不但入射是均匀的，反射也是各向均匀的。用于计算环境光的数学模型非常简单，具体公式如下：

```
环境光照射结果 = 材质的反射系数 * 环境光强度
```

### Metal 示例

在程序中控制环境光强度！

```
typedef struct {  /// 常量数据
    matrix_float4x4 worldMatrix;      
    matrix_float4x4 cameraMatrix;     
    matrix_float4x4 projectionMatrix;
    
    vector_float3 ambient;/// 环境光
} Uniforms;
```

在顶点着色器中计算最终的光照强度：

```
vertex ShaderInOut vertexRender_Ambient(VertexDesc in [[ stage_in ]],
                                constant Uniforms &uniformData [[buffer(kAttributeUniforms)]]) {
    ShaderInOut out;
    out.position = uniformData.projectionMatrix * uniformData.cameraMatrix * uniformData.worldMatrix * in.position;
    out.normal = in.normal;
    out.textureCoordinate = in.textureCoordinate;
    
    out.ambient = float4(uniformData.ambient, 1.0); /// 环境光强度
    return out;
}
```

在片段着色器中为每一个片元着色：

```
fragment half4 fragmentShader_Ambient(ShaderInOut        in             [[ stage_in ]],
                                      texture2d<float> colorTexture [[ texture(kAttributeTexture) ]]) {
    constexpr sampler linearSampler(mip_filter::linear, mag_filter::linear, min_filter::linear);
    float4 color = colorTexture.sample(linearSampler, in.textureCoordinate);
    return half4(color * in.ambient); /// 环境光照射结果 = 材质的反射系数 * 环境光强度
}
```


## 1.2、散射光

上一小节中给出了仅仅使用环境光进行照射的案例，场景效果并不是太好，没有层次感。
本节将介绍另外一种真实感好很多的光照效果 — 散射光(Diffuse)，其指的是从物体表面向全方位 360°均匀反射的光，如下图所示。

![散射光的基本情况](asset/%E6%95%A3%E5%B0%84%E5%85%89%E7%9A%84%E5%9F%BA%E6%9C%AC%E6%83%85%E5%86%B5.png)

散射光具体代表的是现实世界中粗糙的物体表面被光照射时，反射光在各个方向基本均匀(也称为“漫反射”)的情况。

![光在粗糙的表面上发生漫反射](asset/%E6%BC%AB%E5%8F%8D%E5%B0%84%E7%9A%84%E5%9F%BA%E6%9C%AC%E6%83%85%E5%86%B5.png)


虽然反射后的散射光在各个方向是均匀的，但散射光反射的强度与入射光的强度以及入射的角度密切相关。因此，当光源的位置发生变化时，散射光的效果会发生明显变化。主要体现为 _当光垂直地照射到物体表面时比斜照时要亮_，其具体计算公式如下:

```
散射光照射结果 = 材质的反射系数 * 散射光强度 * max(cos(入射角), 0)
```

实际开发中往往分两步进行计算，此时公式被分解为如下情况；

```
散射光最终强度 = 散射光强度 * max(cos(入射角), 0)
散射光照射结果 = 材质的反射系数 * 散射光最终强度

// 材质的反射系数实际指的就是物体被照射处的颜色
// 散射光强度指的是散射光中 RGB(红、绿、蓝)3 个色彩通道的强度
```

从上述公式中可以看出，与环境光计算公式唯一的区别是引入了最后一项 `max(cos(入射角),0)`。
其含义是入射角越大，反射强度越弱，当入射角的余弦值为负时(即入射角大于 90°)，反射强度为 0。
由于入射角为入射光向量与法向量的夹角，因此，其余弦值并不需要调用三角函数进行计算，只需要首先将两个向量进行规格化，然后再进行点积即可！

![散射光的计算](asset/%E6%95%A3%E5%B0%84%E5%85%89%E7%9A%84%E8%AE%A1%E7%AE%97.png)

上图中的 N 代表被照射点表面的法向量，P 为被照射点，L 为从 P 点到光源的向量。N 与 L 的夹角即为入射角。
向量数学中，两个向量的点积为两个向量夹角的余弦值乘以两个向量的模，而规格化后向量的模为 1。因此，首先将两个向量规格化，再点积就可以求得两个向量夹角的余弦值。


### Metal 示例

在程序中控制环境光强度与摄像机位置！

```
typedef struct { /// 常量数据
    matrix_float4x4 worldMatrix;     
    matrix_float4x4 cameraMatrix;     
    matrix_float4x4 projectionMatrix; 

    bool isDirectionLight;  /// 是否是方向光

    vector_float3 light;  /// 光强度
    vector_float3 lightLocation;  /// 定位光：例如白织灯泡，从某个位置向四周发射光
    vector_float3 lightDirection; /// 定向光：例如太阳光，光照方向平行
    
} Uniforms;
```

在顶点着色器计算最终散射光强度：
* `transformNormal`: 转换后的法向量；
    * 物体在世界坐标系，一般经历缩放、平移、旋转等复合变换；
    * 顶点的法向量，也需要同步经历这些复合变换；

```
vertex ShaderInOut vertexRender_Diffuse(VertexDesc in [[ stage_in ]],
                                constant Uniforms &uniformData [[buffer(kAttributeUniforms)]]) {
    ShaderInOut out;
    out.position = uniformData.projectionMatrix * uniformData.cameraMatrix * uniformData.worldMatrix * in.position;
    out.normal = in.normal;
    out.textureCoordinate = in.textureCoordinate;

    float3 transformNormal = normalize((uniformData.worldMatrix * float4(in.normal, 1.0)).xyz);

    /// 如果是定位光：则从光源到顶点计算出该向量
    float3 vp = normalize(uniformData.lightLocation - (uniformData.worldMatrix * in.position).xyz);
    if (uniformData.isDirectionLight) {
        /// 如果是定向光：光照方向即为该向量
        vp = normalize(uniformData.lightDirection);
    }
    float dotPos = max(0.0, dot(transformNormal, vp)); /// max(cos(入射角), 0)
    out.diffuse = uniformData.light * dotPos; /// 散射光最终强度 = 散射光强度 * max(cos(入射角), 0)
    return out;
}
```


在片段着色器计算最终效果：

```
fragment half4 fragmentShader_Diffuse(ShaderInOut        in             [[ stage_in ]],
                                      texture2d<float> colorTexture [[ texture(kAttributeTexture) ]]) {
    constexpr sampler linearSampler(mip_filter::linear, mag_filter::linear, min_filter::linear);
    float4 color = colorTexture.sample(linearSampler, in.textureCoordinate);
    return half4(color * float4(in.diffuse, 1.0));  /// 散射光照射结果 = 材质的反射系数 * 散射光最终强度
}
```

## 1.3、镜面光

使用了上一小节中介绍的散射光效果后，场景的整体效果有了较大的提升。但这并不是光照的全部，现实世界中，当光滑表面被照射时会有方向很集中的反射光。这就是镜面光(Specular)， 本小节将详细介绍镜面光的计算模型。

与散射光最终强度仅依赖于入射光与被照射点法向量的夹角不同，镜面光的最终强度还依赖于观察者的位置。也就是说，如果从摄像机到被照射点的向量不在反射光方向集中的范围内，观察者将不会看到镜面光！

![镜面光基本情况](asset/%E9%95%9C%E9%9D%A2%E5%85%89%E7%9A%84%E5%9F%BA%E6%9C%AC%E6%83%85%E5%86%B5.png)

镜面光的计算模型比前面的两种光都要复杂一些，具体公式如下：

```
镜面光照射结果 = 材质的反射系数 * 镜面光强度 * max(0, (cos(半法向量与法向量的夹角))^粗糙度系数)
```

实际开发中往往分两步进行计算，此时公式被拆解为如下情况：

```
镜面光最终强度 = 镜面光强度 * max(0, (cos(半法向量与法向量的夹角))^粗糙度系数)
镜面光照射结果 = 材质的反射系数 * 镜面光最终强度
```

从上述公式中可以看出，与散射光计算公式主要有两点区别。
* 1、计算余弦值时对应的角不再是入射角，而是半向量与法向量的夹角。
    * 半向量指的是从被照射点到光源的向量与从被照射点到观察点向量的平均向量
* 2、求得的余弦值还需要对粗糙度进行乘方运算，此运算可以达到粗糙度越小，镜面光面积越大的效果，这也是很贴近现实世界的。

![计算镜面反射光](asset/%E8%AE%A1%E7%AE%97%E9%95%9C%E9%9D%A2%E5%8F%8D%E5%B0%84%E5%85%89.png)

上图中，V 为从被照射点到观察点的向量，N 为被照射点表面的法向量，H 为 半向量，L 为从被照射点到光源的向量。
* 半向量 H 与 V 及 L 共面，并且其与这两个向量的夹角相等；
* 已知 V 和 L 后计算 H 非常简单，只要首先将 V 和 L 规格化，然后将规格化后的 V 与 L 求和并再次规格化即可；
* 求得半向量后，再求其与法向量夹角的余弦值就非常简单了，只需将规格化后的法向量与半向量进行点积即可。


### Metal 示例

在程序中控制环境光强度与摄像机位置！

```
typedef struct { /// 常量数据
    matrix_float4x4 worldMatrix;     
    matrix_float4x4 cameraMatrix;     
    matrix_float4x4 projectionMatrix; 

    bool isDirectionLight;  /// 是否是方向光
    vector_float3 cameraPos; // 相机位置
    
    vector_float3 light;  /// 光强度
    vector_float3 lightLocation;  /// 定位光：例如白织灯泡，从某个位置向四周发射光
    vector_float3 lightDirection; /// 定向光：例如太阳光，光照方向平行
    
} Uniforms;
```

在顶点着色器计算最终镜面光强度：
* `transformNormal`: 转换后的法向量；
    * 物体在世界坐标系，一般经历缩放、平移、旋转等复合变换；
    * 顶点的法向量，也需要同步经历这些复合变换；

```
vertex ShaderInOut vertexRender_Specular(VertexDesc in [[ stage_in ]],
                                constant Uniforms &uniformData [[buffer(kAttributeUniforms)]]) {
    ShaderInOut out;
    out.position = uniformData.projectionMatrix * uniformData.cameraMatrix * uniformData.worldMatrix * in.position;
    out.normal = in.normal;
    out.textureCoordinate = in.textureCoordinate;
    
    /// 计算镜面光
    float3 transformNormal = normalize((uniformData.worldMatrix * float4(in.normal, 1.0)).xyz);
    
    /// 计算像素点到摄像机的向量
    float3 eye = normalize(uniformData.cameraPos - (uniformData.worldMatrix * in.position).xyz);
    /// 计算像素点到光源位置的向量
    float3 vp = normalize(uniformData.lightLocation - (uniformData.worldMatrix * in.position).xyz);
    if (uniformData.isDirectionLight) { /// 如果是定向光：光照方向即为该向量
        vp = normalize(uniformData.lightDirection);
    }
    float3 halfVector = normalize(eye + vp);
    float shininess = 5.0; /// 粗糙度、越小越光滑
    float dotPos = dot(transformNormal, halfVector);
    float powerFactor = max(0.0, pow(dotPos, shininess)); /// max(0, (cos(半法向量与法向量的夹角))^粗糙度系数)
    /// 镜面光最终强度 = 镜面光强度 * max(0, (cos(半法向量与法向量的夹角))^粗糙度系数)
    out.specular = uniformData.light * powerFactor; 
    
    return out;
}
```

在片段着色器计算最终效果：

```
fragment half4 fragmentShader_Specular(ShaderInOut        in             [[ stage_in ]],
                                      texture2d<float> colorTexture [[ texture(kAttributeTexture) ]]) {
    constexpr sampler linearSampler(mip_filter::linear, mag_filter::linear, min_filter::linear);
    float4 color = colorTexture.sample(linearSampler, in.textureCoordinate); 
    return half4(color * float4(in.specular, 1.0)); /// 镜面光照射结果 = 材质的反射系数 * 镜面光最终强度
}
```


## 1.4、合成光

前面 3 个小节案例中的每个仅采用了一种光照通道，而现实世界中 3 种通道是同时作用的。 
因此，本小节将前面 3 个小节不同通道(环境光、散射光、镜面光) 的光照效果综合起来！


# 2、定位光与定向光

定位光光源类似于现实生活中的白炽灯泡，其在某个固定的位置，发出的光向四周发散。定位光照射的一个明显特点就是，在给定光源位置的情况下，对不同位置的物体产生的光照效果不同。

现实世界中并不都是定位光，例如，照射到地面上的太阳光，光线之间是平行的，这种光称为定向光。定向光照射的明显特点是，在给定光线方向的情况下，场景中不同位置的物体反映出的光照效果完全一致！

![定位光与定向光](asset/%E5%AE%9A%E4%BD%8D%E5%85%89%E4%B8%8E%E5%AE%9A%E5%90%91%E5%85%89.png)

```
fragment half4 fragmentShader_Compound(ShaderInOut        in             [[ stage_in ]],
                                      texture2d<float> colorTexture [[ texture(kAttributeTexture) ]]) {
    constexpr sampler linearSampler(mip_filter::linear, mag_filter::linear, min_filter::linear);
    float4 color = colorTexture.sample(linearSampler, in.textureCoordinate); /// 材质的反射系数
    return half4(color * float4((in.ambient + in.diffuse + in.specular), 1.0));
}
```

# 3、光照计算

光照计算可以在顶点着色器进行、也可以在片段着色器进行：
* 在顶点着色器中对每个顶点进行光照计算后得到顶点的最终光照强度，再由管线插值后传入片元着色器以计算片元的颜色；
    * 这样一方面效率比较高，另一方面产生的光照效果也不错。
    * 但由于这种计算方式插值的是基于顶点计算后的光照强度，因此在要求很高，希望有非常细腻光照效果的场合下就略显粗糙了。
* 在片段着色器计算最终光照强度：将插值后的法向量数据传入片元着色器，然后在片元着色器中进行光照计算。

每片元计算光照与每顶点计算光照算法并没有本质区别，只是代码执行的位置不同、效果与效率不同而已。实际开发中应该权衡速度、效果的要求，选用合适的计算策略。
