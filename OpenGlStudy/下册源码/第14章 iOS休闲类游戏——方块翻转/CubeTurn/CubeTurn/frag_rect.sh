#version 300 es
precision mediump float;
uniform sampler2D sTexture;
uniform float uSJ;
in vec2 vTextureCoord;
out vec4 fragColor;

void main()
{
    vec4 color = texture(sTexture, vTextureCoord);
    fragColor = vec4(color.r,color.g,color.b,color.a*uSJ);
}