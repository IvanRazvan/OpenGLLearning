#pragma once

#include "ExternalLibraryHeaders.h"
#include "Helper.h"
#include "Mesh.h"
#include "ImageLoader.h"

/*
	MyMesh class implements the generation, handling and rendering of a mesh.
*/

class MyMesh
{
private:
	std::string m_name;
	glm::mat4 m_xform;

	std::vector<GLuint> m_VAO;
	std::vector<GLuint> m_numElements;

	GLuint GeneratePositionsVBO(const Helpers::Mesh& mesh);
	GLuint GenerateNormalsVBO(const Helpers::Mesh& mesh);
	GLuint GeneratePositionsEBO(const Helpers::Mesh& mesh);
	GLuint GenerateUVCoords(const Helpers::Mesh& mesh);
	void SetVAO(Helpers::ModelLoader& model);

public:
	MyMesh() = default;
	~MyMesh();

	void SetXForm(glm::mat4 xform);

	std::string GetName() const;
	
	bool Create(std::string meshFilepath, std::string name, glm::vec3 pos, glm::vec3 rot);

	void Render(GLuint m_program);
};

