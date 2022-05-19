#version 300 es
uniform mat4 uMVPMatrix;
uniform float uStartAngle;
uniform float uWidthSpan;
in vec3 aPosition;
in vec2 aTexCoor;
out vec2 vTextureCoord;

void main() {
   float angleSpanH=4.0*3.14159265;
   float startX=-uWidthSpan/2.0;
   float currAngleH=uStartAngle+((aPosition.x-startX)/uWidthSpan)*angleSpanH;
   float tzH=sin(currAngleH)*0.1;
   
   float angleSpanZ=4.0*3.14159265;
   float uHeightSpan=0.75*uWidthSpan;
   float startY=-uHeightSpan/2.0;
   float currAngleZ=uStartAngle+3.14159265/3.0+((aPosition.y-startY)/uHeightSpan)*angleSpanZ;
   float tzZ=sin(currAngleZ)*0.1;
   
   gl_Position = uMVPMatrix * vec4(aPosition.x,aPosition.y,tzH+tzZ,1);
   vTextureCoord = aTexCoor;
}
