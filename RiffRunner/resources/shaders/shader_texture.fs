#version 460

in vec2 texCoord;
out vec4 color;

uniform sampler2D texBuff;
uniform vec4 texColor;

void main()
{
	color =  texture(texBuff,texCoord) * texColor;
}