#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 combined_xform;

out vec3 textureCoords;

void main()
{
	vec4 pos = combined_xform * vec4(position, 1.0f);
	gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
	textureCoords = vec3(position.x, position.y, -position.z);
}