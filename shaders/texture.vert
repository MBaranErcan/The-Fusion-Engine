#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;

out vec4 color;
out vec2 texCoord;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(aPos, 1.0);
	color = vec4(aColor, 1.0);
	texCoord = aTex;
}