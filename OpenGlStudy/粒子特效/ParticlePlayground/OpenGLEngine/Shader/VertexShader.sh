#version 300 es
uniform mat4 uMVPMatrix;    /// 总变换矩阵
uniform float maxLifeSpan;  /// 最大允许生命期
in vec4 aPosition;          /// 从渲染管线接收的顶点位置属性: (x,y, Vx, 生命周期)
in vec2 aTexCoor;           /// 从渲染管线接收的纹理坐标

out vec2 vTextureCoord;     /// 用于传递给片元着色器的纹理坐标
out vec4 vPosition;         /// 用于传递给片元着色器的顶点位置属性
out float sjFactor;         /// 用于传递给片元着色器的总衰减因子


vec4 permute(vec4 x){return mod(((x*34.)+1.)*x,289.);}
vec4 taylorInvSqrt(vec4 r){return 1.79284291400159-.85373472095314*r;}

float snoise(vec3 v){
    const vec2 C=vec2(1./6.,1./3.);
    const vec4 D=vec4(0.,.5,1.,2.);
    
    // First corner
    vec3 i=floor(v+dot(v,C.yyy));
    vec3 x0=v-i+dot(i,C.xxx);
    
    // Other corners
    vec3 g=step(x0.yzx,x0.xyz);
    vec3 l=1.-g;
    vec3 i1=min(g.xyz,l.zxy);
    vec3 i2=max(g.xyz,l.zxy);
    
    //  x0 = x0 - 0. + 0.0 * C
    vec3 x1=x0-i1+1.*C.xxx;
    vec3 x2=x0-i2+2.*C.xxx;
    vec3 x3=x0-1.+3.*C.xxx;
    
    // Permutations
    i=mod(i,289.);
    vec4 p=permute(permute(permute(
                i.z+vec4(0.,i1.z,i2.z,1.))
                +i.y+vec4(0.,i1.y,i2.y,1.))
                +i.x+vec4(0.,i1.x,i2.x,1.));
                
                // Gradients
                // ( N*N points uniformly over a square, mapped onto an octahedron.)
                float n_=1./7.;// N=7
                vec3 ns=n_*D.wyz-D.xzx;
                
                vec4 j=p-49.*floor(p*ns.z*ns.z);//  mod(p,N*N)
                
                vec4 x_=floor(j*ns.z);
                vec4 y_=floor(j-7.*x_);// mod(j,N)
                
                vec4 x=x_*ns.x+ns.yyyy;
                vec4 y=y_*ns.x+ns.yyyy;
                vec4 h=1.-abs(x)-abs(y);
                
                vec4 b0=vec4(x.xy,y.xy);
                vec4 b1=vec4(x.zw,y.zw);
                
                vec4 s0=floor(b0)*2.+1.;
                vec4 s1=floor(b1)*2.+1.;
                vec4 sh=-step(h,vec4(0.));
                
                vec4 a0=b0.xzyw+s0.xzyw*sh.xxyy;
                vec4 a1=b1.xzyw+s1.xzyw*sh.zzww;
                
                vec3 p0=vec3(a0.xy,h.x);
                vec3 p1=vec3(a0.zw,h.y);
                vec3 p2=vec3(a1.xy,h.z);
                vec3 p3=vec3(a1.zw,h.w);
                
                //Normalise gradients
                vec4 norm=taylorInvSqrt(vec4(dot(p0,p0),dot(p1,p1),dot(p2,p2),dot(p3,p3)));
                p0*=norm.x;
                p1*=norm.y;
                p2*=norm.z;
                p3*=norm.w;
                
                // Mix final noise value
                vec4 m=max(.6-vec4(dot(x0,x0),dot(x1,x1),dot(x2,x2),dot(x3,x3)),0.);
                m=m*m;
                return 42.*dot(m*m,vec4(dot(p0,x0),dot(p1,x1),
                dot(p2,x2),dot(p3,x3)));
            }
            
vec3 snoiseVec3(vec3 x){
        return vec3(snoise(vec3(x)*2.-1.),
        snoise(vec3(x.y-19.1,x.z+33.4,x.x+47.2))*2.-1.,
        snoise(vec3(x.z+74.2,x.x-124.5,x.y+99.4)*2.-1.)
    );
}
        
vec3 curlNoise(vec3 p){
    const float e=.1;
    vec3 dx=vec3(e,0.,0.);
    vec3 dy=vec3(0.,e,0.);
    vec3 dz=vec3(0.,0.,e);
    
    vec3 p_x0=snoiseVec3(p-dx);
    vec3 p_x1=snoiseVec3(p+dx);
    vec3 p_y0=snoiseVec3(p-dy);
    vec3 p_y1=snoiseVec3(p+dy);
    vec3 p_z0=snoiseVec3(p-dz);
    vec3 p_z1=snoiseVec3(p+dz);
    
    float x=p_y1.z-p_y0.z-p_z1.y+p_z0.y;
    float y=p_z1.x-p_z0.x-p_x1.z+p_x0.z;
    float z=p_x1.y-p_x0.y-p_y1.x+p_y0.x;
    
    const float divisor=1./(2.*e);
    return normalize(vec3(x,y,z)*divisor);
}
        

        


void main()
{ //主函数
    
    vec3 noise=curlNoise(vec3(aPosition.x*.02,aPosition.y*.008,aPosition.w*.05));
    vec3 distortion=vec3(aPosition.x*2.,aPosition.y,1.)*noise* aPosition.w;
    vec3 newPos= aPosition.xyz + distortion;

    gl_Position = uMVPMatrix * vec4(newPos.x,newPos.y,newPos.z,1); //根据总变换矩阵计算此次绘制此顶点位置
    vTextureCoord = aTexCoor;//将接收的纹理坐标传递给片元着色器
    vPosition=vec4(newPos.x,newPos.y,newPos.z,aPosition.w);//计算顶点位置属性，并将其传递给片元着色器
    
    gl_PointSize = 4.0;//设置点精灵对应点的尺寸

    /// 根据当前生命期值与最大允许生命期值计算要传递给片元着色器的总衰减因子
    /// 随着粒子生命期的增加，总衰减因子逐渐减小，直至为 0
    sjFactor = (maxLifeSpan - aPosition.w) / maxLifeSpan;
}

/*
{ //主函数

    gl_Position = uMVPMatrix * vec4(aPosition.x,aPosition.y,0.0,1); //根据总变换矩阵计算此次绘制此顶点位置
    vTextureCoord = aTexCoor;//将接收的纹理坐标传递给片元着色器
    vPosition=vec4(aPosition.x,aPosition.y,0.0,aPosition.w);//计算顶点位置属性，并将其传递给片元着色器

    gl_PointSize = 4.0;//设置点精灵对应点的尺寸

    /// 根据当前生命期值与最大允许生命期值计算要传递给片元着色器的总衰减因子
    /// 随着粒子生命期的增加，总衰减因子逐渐减小，直至为 0
    sjFactor = (maxLifeSpan - aPosition.w) / maxLifeSpan;
}
*/
