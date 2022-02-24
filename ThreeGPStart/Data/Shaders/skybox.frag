#version 330 core

uniform samplerCube skybox;

in vec3 textureCoords;

out vec4 fragmentColour;

void main()
{
	fragmentColour = texture(skybox, textureCoords);
}