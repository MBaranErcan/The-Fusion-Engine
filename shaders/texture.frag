#version 440 core

out vec4 frag_color;

in vec4 vert_color;

void main()
{
	frag_color = vert_color;
}