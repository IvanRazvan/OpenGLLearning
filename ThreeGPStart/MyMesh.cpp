#include "MyMesh.h"

GLuint MyMesh::GeneratePositionsVBO(const Helpers::Mesh& mesh)
{
	GLuint positionsVBO{ 0 };
	// Generate ID for the VBO
	glGenBuffers(1, &positionsVBO); // generates 1 unique ID

	// Bind to the GL context, since it's the first time, will also create it
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);

	// Fill the buffer with the vertex data
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.vertices.size(), mesh.vertices.data(), GL_STATIC_DRAW);

	// Clear binding
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return positionsVBO;
}

GLuint MyMesh::GeneratePositionsEBO(const Helpers::Mesh& mesh)
{
	GLuint positionsEBO{ 0 };
	// Generate ID for the EBO
	glGenBuffers(1, &positionsEBO); // generates 1 unique ID

	// Bind to the GL context, since it's the first time, will also create it
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, positionsEBO);

	// Fill the buffer with the vertex data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.elements.size(), mesh.elements.data(), GL_STATIC_DRAW);

	// Clear binding
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_numElements.at(m_numElements.size() - 1) = mesh.elements.size();

	return positionsEBO;
}

GLuint MyMesh::GenerateNormalsVBO(const Helpers::Mesh& mesh)
{
	GLuint normalsVBO{ 0 };
	// Generate ID for the VBO
	glGenBuffers(1, &normalsVBO); // generates 1 unique ID

	// Bind to the GL context, since it's the first time, will also create it
	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);

	// Fill the buffer with the vertex data
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.normals.size(), mesh.normals.data(), GL_STATIC_DRAW);

	// Clear binding
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return normalsVBO;
}

GLuint MyMesh::GenerateUVCoords(const Helpers::Mesh& mesh)
{
	GLuint UVcoords{ 0 };
	// Generate ID for the UVcoords
	glGenBuffers(1, &UVcoords); // generates 1 unique ID

	// Bind to the GL context, since it's the first time, will also create it
	glBindBuffer(GL_ARRAY_BUFFER, UVcoords);

	// Fill the buffer with the vertex data
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh.uvCoords.size(), mesh.uvCoords.data(), GL_STATIC_DRAW);

	// Clear binding
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return UVcoords;
}

MyMesh::~MyMesh()
{
	for (GLuint VAO : m_VAO)
		glDeleteBuffers(1, &VAO);
}

void MyMesh::SetXForm(glm::mat4 xform)
{
	m_xform = xform;
}

std::string MyMesh::GetName() const
{
	return m_name;
}

bool MyMesh::Create(std::string meshFilepath, std::string name, glm::vec3 pos, glm::vec3 rot)
{
	m_name = name;

	// Load the model
	Helpers::ModelLoader model;
	if (model.LoadFromFile(meshFilepath + name) == false)
		return false;

	// Set VAO
	SetVAO(model);

	return true;
}

void MyMesh::SetVAO(Helpers::ModelLoader& model)
{
	for(const Helpers::Mesh& mesh : model.GetMeshVector())
	{
		GLuint VAO;
		// Generate ID for VAO
		glGenVertexArrays(1, &VAO);
		// Bind to context, it's first time so also create
		m_VAO.push_back(VAO);
		m_numElements.push_back(0);

		glBindVertexArray(VAO);
		// From now on what we do is recorded for this VAO and repeated when we render

		glBindBuffer(GL_ARRAY_BUFFER, GeneratePositionsVBO(mesh));
		glEnableVertexAttribArray(0);
		// Describe the make up of the vertex stream
		glVertexAttribPointer(
			0, //attribute number
			3, // size in components, 3-xyz
			GL_FLOAT, // type of the item
			GL_FALSE, // normalised or not
			0, // stride
			(void*)0 // offset
		);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GeneratePositionsEBO(mesh));

		glBindBuffer(GL_ARRAY_BUFFER, GenerateNormalsVBO(mesh));
		glEnableVertexAttribArray(1);
		// Describe the make up of the vertex stream
		glVertexAttribPointer(
			1, //attribute number
			3, // size in components, 3-xyz
			GL_FLOAT, // type of the item
			GL_FALSE, // normalised or not
			0, // stride
			(void*)0 // offset
		);

		glBindBuffer(GL_ARRAY_BUFFER, GenerateUVCoords(mesh));
		glEnableVertexAttribArray(2);
		// Describe the make up of the vertex stream
		glVertexAttribPointer(
			2, //attribute number
			2, // size in components, 2-xy
			GL_FLOAT, // type of the item
			GL_FALSE, // normalised or not
			0, // stride
			(void*)0 // offset
		);
		// Clear context
		glBindVertexArray(0);
	}
}

void MyMesh::Render(GLuint m_program)
{
	// Send the model matrix to the shader in a uniform
	GLuint model_xform_id = glGetUniformLocation(m_program, "model_xform");
	glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(m_xform));

	// Bind our VAO and render
	for (int i = 0; i < m_VAO.size(); i++)
	{
		glBindVertexArray(m_VAO.at(i));
		glDrawElements(GL_TRIANGLES, m_numElements.at(i), GL_UNSIGNED_INT, (void*)0);
	}
}

