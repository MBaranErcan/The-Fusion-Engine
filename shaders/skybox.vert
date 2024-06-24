#version 460 core

layout (location = 0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w); // Z being equal to W will make the depth of 1.0f
	texCoords = vec3(aPos.xy, -aPos.z);
}
