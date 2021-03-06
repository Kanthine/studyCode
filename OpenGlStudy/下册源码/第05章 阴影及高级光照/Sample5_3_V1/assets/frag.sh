#version 300 es
//仅仅贴纹理的着色器
precision highp float;
in vec2 vTextureCoord; //接收从顶点着色器过来的参数
uniform sampler2D sTexture;//纹理内容数据
out vec4 fragColor;//输出到的片元颜色
void main()                         
{           
   //给此片元从纹理中采样出颜色值，为了使不同的距离值显示出来灰度不同除以100，
   //使距离值的对应颜色值在0～1之间，否则值大于1的话看起来都是白色了            
   float depthValue=texture(sTexture,vTextureCoord).r/100.0;
   fragColor=vec4(depthValue,depthValue,depthValue,1.0);
}              