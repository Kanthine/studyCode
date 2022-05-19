#version 300 es
uniform mat4 uMVPMatrix;    /// 总变换矩阵
uniform float uStartAngle;  /// 本帧起始角度(即最左侧顶点的对应角度)
uniform float uWidthSpan;   /// 横向长度总跨度
in vec3 aPosition;          /// 顶点位置
in vec2 aTexCoor;           /// 顶点纹理坐标
out vec2 vTextureCoord;     /// 用于传递给片元着色器的纹理坐标

void main() {
   float angleSpanH = 4.0 * 3.14159265; /// 横向角度总跨度，用于进行 X 距离与角度的换算
   float startX = -uWidthSpan / 2.0; /// 起始 X 坐标，即最左侧顶点的 X 坐标
   float currAngle=uStartAngle+((aPosition.x-startX)/uWidthSpan)*angleSpanH;
   float tz=sin(currAngle)*0.1;
   gl_Position = uMVPMatrix * vec4(aPosition.x,aPosition.y,tz,1);
   vTextureCoord = aTexCoor;
}
