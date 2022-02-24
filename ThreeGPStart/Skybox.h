#pragma once
#include "ImageLoader.h"

/*
	Skybox class implements a cube map skybox.
*/

class Skybox
{
private:
	GLuint m_VAO{ 0 };
	GLuint m_numElements{ 0 };

	GLuint GeneratePositionsVBO(std::vector<glm::vec3>& vertices);
	GLuint GeneratePositionsEBO(std::vector<GLuint>& elements);
	void GenerateTexture(std::string facesFilenames[6]);
	void SetVAO(std::vector<glm::vec3>& vertices, std::vector<GLuint>& elements);

public:
	Skybox() = default;
	~Skybox();

	bool CreateSkybox(const std::string& fileDir);

	void Render(GLuint program, glm::mat4 projection_xform, glm::mat4 view_xform);
};

