#version 300 es
precision mediump float;                            //给出默认的浮点精度
in vec2 vTextureCoord;                         //接收从顶点着色器传过来的纹理坐标
in float currY;                                //接收从顶点着色器传过来的Y坐标
in vec4 pLocation;                                //接收从顶点着色器传过来的顶点坐标
uniform float slabY;                                //体积雾对应雾平面的高度
uniform float startAngle;                            //扰动起始角
uniform vec3 uCamaraLocation;                       //摄像机位置
uniform float landStartY;                            //过程纹理起始Y坐标
uniform float landYSpan;                            //过程纹理跨度
out vec4 fragColor;//输出到的片元颜色

float tjFogCal(vec4 pLocation){//计算体积雾浓度因子的方法
   
   float xAngle=pLocation.x/16.0*3.1415926;//计算出顶点X坐标折算出的角度
   
   float zAngle=pLocation.z/20.0*3.1415926;//计算出顶点Z坐标折算出的角度
   
   float slabYFactor=sin(xAngle+zAngle+startAngle);//联合起始角计算出角度和的正弦值
   //求从摄像机到待处理片元的射线参数方程Pc+(Pp-Pc)t与雾平面交点的t值
   float t=(slabY+slabYFactor-uCamaraLocation.y)/(pLocation.y-uCamaraLocation.y);
   //有效的t的范围应该在0~1的范围内，若不存在范围内表示待处理片元不在雾平面以下
   if(t>0.0&&t<1.0){//若在有效范围内则
      //求出射线与雾平面的交点坐标
      float xJD=uCamaraLocation.x+(pLocation.x-uCamaraLocation.x)*t;
      float zJD=uCamaraLocation.z+(pLocation.z-uCamaraLocation.z)*t;
      vec3 locationJD=vec3(xJD,slabY,zJD);
      
      float L=distance(locationJD,pLocation.xyz);//求出交点到待处理片元位置的距离
      float L0=10.0;
      
      return L0/(L+L0);//计算体积雾的雾浓度因子
   }else{
      return 1.0;//若待处理片元不在雾平面以下，则此片元不受雾影响
   }}
void main(){
     
   vec4 gColor;//从草皮纹理中采样出颜色
   vec4 rColor; //从岩石纹理中采样出颜色
   vec4 finalColor;                                    //片元最终颜色
   if(currY<landStartY){
                   
           finalColor=gColor;    //当片元Y坐标小于过程纹理起始Y坐标时采用草皮纹理
   }else if(currY>landStartY+landYSpan){
   
          finalColor=rColor;//当片元Y坐标大于过程纹理起始Y坐标加跨度时采用岩石纹理
   }else{//当片元Y坐标在过程纹理范围内时将草皮和岩石混合
              
           float currYRatio=(currY-landStartY)/landYSpan;//计算岩石纹理所占的百分比
           //将岩石、草皮纹理颜色按比例混合
           finalColor= currYRatio*rColor+(1.0- currYRatio)*gColor;
   }
   float fogFactor=tjFogCal(pLocation);//计算雾浓度因子
   //根据雾浓度因子、雾的颜色及片元本身采样的纹理颜色计算出片元的最终颜色
   fragColor=fogFactor*finalColor+ (1.0-fogFactor)*vec4(0.9765,0.7490,0.0549,0.0); //给此片元最终颜色值
}
