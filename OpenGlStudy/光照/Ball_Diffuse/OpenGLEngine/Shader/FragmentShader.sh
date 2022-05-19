#version 300 es
precision mediump float;
uniform float uR;   // 传入的球半径
in vec3 vPosition;  // 接收从顶点着色器过来的顶点位置
in vec4 vDiffuse;
out vec4 fragColor; // 输出的片元颜色

void main() {
   vec3 color;
   float n = 8.0;       /// 外接立方体每个坐标轴方向切分的份数
   float span = 2.0 * uR / n;   /// 每一份的尺寸
   int i = int((vPosition.x + uR)/span);    /// 当前片元位置小方块的行数
   int j = int((vPosition.y + uR)/span);    /// 当前片元位置小方块的层数
   int k = int((vPosition.z + uR)/span);    /// 当前片元位置小方块的列数
   /// 计算当前片元行数、层数、列数的和并对 2 取模，用于确定当前片元的颜色
   int whichColor = int(mod(float(i+j+k),2.0));
   if(whichColor == 1) { //奇数时为红色
        color = vec3(0.678,0.231,0.129);
   } else { // 偶数时为白色
        color = vec3(1.0,1.0,1.0);
   }
    
   vec4 finalColor = vec4(color,0);
   fragColor=finalColor * vDiffuse;
}


/** 散射光(Diffuse)，其指的是从物体表面向全方位 360°均匀反射的光
 * 散射光具体代表的是现实世界中粗糙的物体表面被光照射时，反射光在各个方向基本均匀(也 称为“漫反射”)的情况
 * 虽然反射后的散射光在各个方向是均匀的，但散射光反射的强度与入射光的强度以及入射的 角度密切相关。
 * 因此当光源的位置发生变化时，散射光的效果会发生明显变化。主要体现为当光垂直地照射到物体表面时比斜照时要亮，其具体计算公式如下
 *      散射光照射结果=材质的反射系数x散射光强度 x max(cos(入射角),0)
 * 实际开发中往往分两步进行计算，此时公式被拆解为如下情况：
 *    散射光最终强度=散射光强度 x max(cos(入射角),0)
 *    散射光照射结果=材质的反射系数 x 散射光最终强度
 */
