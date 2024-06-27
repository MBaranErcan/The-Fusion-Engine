#version 460 core

out vec4 fragColor;

in vec4 color;
in vec2 texCoord;

uniform sampler2D tex0;

void main()
{
	fragColor = (texture(tex0, texCoord) * 0.7) + (color * 0.3);
}