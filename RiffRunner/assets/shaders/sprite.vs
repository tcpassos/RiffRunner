#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texc;

out vec2 texCoord;

uniform mat4 projection;
uniform mat4 model;

void main()
{
	gl_Position = projection * model * vec4(position, 1.0);
	texCoord = texc;
}