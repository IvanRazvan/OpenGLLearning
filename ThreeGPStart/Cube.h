#pragma once
#include "Helper.h"

/*
	Cube class implements the generation, handling and rendering of a cube.
*/

class Cube
{
private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;

	GLuint m_VAO{ 0 };
	GLuint m_numElements{ 0 };

	GLuint GeneratePositionsVBO(std::vector<glm::vec3>& vertices);
	GLuint GenerateColoursVBO(std::vector<glm::vec3>& colours);
	GLuint GenerateNormalsVBO(std::vector<glm::vec3>& normals);
	GLuint GeneratePositionsEBO(std::vector<GLuint>& elements);
	void SetVAO(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& colours, std::vector<glm::vec3>& normals, std::vector<GLuint>& elements);

public:
	Cube() = default;
	~Cube();

	bool CreateCube(int size, glm::vec3 position);

	void Move(glm::vec3 pos);
	void Rotate(glm::vec3 rot);
	
	void Render(GLuint m_program, glm::mat4 combined_xform);
};

