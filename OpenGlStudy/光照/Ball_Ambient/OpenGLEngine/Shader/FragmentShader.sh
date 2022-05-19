#version 300 es
precision mediump float;
uniform float uR;   // 传入的球半径
in vec3 vPosition;  // 接收从顶点着色器过来的顶点位置
in vec4 vAmbient;   // 接收从顶点着色器过来的环境光强度
out vec4 fragColor; // 输出的片元颜色

void main() {
   vec3 color;
   float n = 8.0; /// 外接立方体每个坐标轴方向切分的份数
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
   vec4 finalColor = vec4(color,0);   // 最终颜色
   fragColor = finalColor * vAmbient; // 根据环境光强度计算最终片元颜色值
   /// 使用环境光强度与片元本身颜色值加权计算产生最终片元颜色值的相关代码
}


/** 环境光(Ambient)指的是从四面八方照射到物体上，全方位 360°都均匀的光。
 * 其代表的是现实世界中从光源射出，经过多次反射后，各方向基本均匀的光。
 * 环境光最大的特点是不依赖于光源的位置，而且没有方向性！
 *
 * 环境光不但入射是均匀的，反射也是各向均匀的。用于计算环境光的数学模型非常简单，具体公式：
 *      环境光照射结果 = 材质的反射系数 x 环境光强度
 *
 * 仅仅有环境光的场景效果是很差的，没有层次感
 */
