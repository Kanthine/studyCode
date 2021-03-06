#version 300 es
precision mediump float;				//给出默认的浮点精度

uniform int uisShadow;//是否绘制阴影
uniform mat4 uMVPMatrix; //总变换矩阵
uniform mat4 uMMatrix; //变换矩阵
uniform mat4 uMCameraMatrix; //摄像机矩阵
uniform mat4 uMProjMatrix; //投影矩阵
uniform vec3 uLightLocation;	//光源位置
uniform vec3 uCamera;	//摄像机位置
uniform vec3 uplaneN;//需要影子平面的法向量
uniform vec3 uplaneA;//需要影子平面的一个点
in vec3 aPosition;  //顶点位置
in vec3 aNormal;    //顶点法向量
in vec2 aTexCoor;    //顶点纹理坐标
out vec2 vTextureCoord;  //用于传递给片元着色器的变量
out vec4 vambient;
out vec4 vdiffuse;
out vec4 vspecular; 
out vec4 vfragLosition;

//定位光光照计算的方法
void pointLight(					//定位光光照计算的方法
  in vec3 normal,				//法向量
  inout vec4 vambient,			//环境光最终强度
  inout vec4 vdiffuse,				//散射光最终强度
  inout vec4 vspecular,			//镜面光最终强度
  in vec3 ulightLocation,			//光源位置
  in vec4 lightAmbient,			//环境光强度
  in vec4 lightDiffuse,			//散射光强度
  in vec4 lightSpecular			//镜面光强度
){
  vambient=lightAmbient;			//直接得出环境光的最终强度  
  vec3 normalTarget=aPosition+normal;	//计算变换后的法向量
  vec3 newNormal=(uMMatrix*vec4(normalTarget,1)).xyz-(uMMatrix*vec4(aPosition,1)).xyz;
  newNormal=normalize(newNormal); 	//对法向量规格化
  //计算从表面点到摄像机的向量
  vec3 eye= normalize(uCamera-(uMMatrix*vec4(aPosition,1)).xyz);  
  //计算从表面点到光源位置的向量vp
  vec3 vp= normalize(ulightLocation-(uMMatrix*vec4(aPosition,1)).xyz);  
  vp=normalize(vp);//格式化vp
  vec3 halfVector=normalize(vp+eye);	//求视线与光线的半向量    
  float shininess=50.0;				//粗糙度，越小越光滑
  float nDotViewPosition=max(0.0,dot(newNormal,vp)); 	//求法向量与vp的点积与0的最大值
  vdiffuse=lightDiffuse*nDotViewPosition;				//计算散射光的最终强度
  float nDotViewHalfVector=dot(newNormal,halfVector);	//法线与半向量的点积 
  float powerFactor=max(0.0,pow(nDotViewHalfVector,shininess)); 	//镜面反射光强度因子
  vspecular=lightSpecular*powerFactor;    			//计算镜面光的最终强度
}

void main()     
{                            	
   if(uisShadow==1)
   {//若为阴影绘制则根据公式进行投影计算
      vec3 A=uplaneA;//vec3(0.0,0.05,0.0);
      vec3 n=uplaneN;//vec3(0.0,1.0,0.0);
      vec3 S=uLightLocation;      
      vec3 V=(uMMatrix*vec4(aPosition,1)).xyz;      
      vec3 VL=S+(V-S)*(dot(n,(A-S))/dot(n,(V-S)));
      vfragLosition= uMProjMatrix*uMCameraMatrix*vec4(VL,1); //根据总变换矩阵计算此次绘制此顶点位置
      gl_Position=vfragLosition;
      vec4 ambientTemp=vec4(0.0,0.0,0.0,0.0);
   vec4 diffuseTemp=vec4(0.0,0.0,0.0,0.0);
   vec4 specularTemp=vec4(0.0,0.0,0.0,0.0);
   
   pointLight(normalize(aNormal),ambientTemp,diffuseTemp,specularTemp,uLightLocation,vec4(0.3,0.3,0.3,0.3),vec4(0.7,0.7,0.7,0.3),vec4(0.3,0.3,0.3,0.3));
   
   vambient=ambientTemp;
   vdiffuse=diffuseTemp;
   vspecular=specularTemp;
   
   
   }
	 else
	 {
	    vfragLosition= uMVPMatrix * vec4(aPosition,1); //根据总变换矩阵计算此次绘制此顶点位置
	    gl_Position =vfragLosition;
	    vec4 ambientTemp=vec4(0.0,0.0,0.0,0.0);
   vec4 diffuseTemp=vec4(0.0,0.0,0.0,0.0);
   vec4 specularTemp=vec4(0.0,0.0,0.0,0.0);
   
   pointLight(normalize(aNormal),ambientTemp,diffuseTemp,specularTemp,uLightLocation,vec4(0.3,0.3,0.3,1.0),vec4(0.7,0.7,0.7,1.0),vec4(0.3,0.3,0.3,1.0));
   
   vambient=ambientTemp;
   vdiffuse=diffuseTemp;
   vspecular=specularTemp;
	 }

   vTextureCoord = aTexCoor;//将接收的纹理坐标传递给片元着色器
}