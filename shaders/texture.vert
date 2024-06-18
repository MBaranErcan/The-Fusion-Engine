#version 440 core

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_color;

out vec4 vert_color;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(in_pos, 1.0);
	vert_color = vec4(in_color, 1.0);
}