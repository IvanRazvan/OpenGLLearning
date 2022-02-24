#include "Cube.h"

GLuint Cube::GeneratePositionsVBO(std::vector<glm::vec3>& vertices)
{
	GLuint positionsVBO{ 0 };

	glGenBuffers(1, &positionsVBO); // generates 1 unique ID

	// Bind to the GL context, since it's the first time, will also create it
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);

	// Fill the buffer with the vertex data
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// Clear binding
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return positionsVBO;
}

GLuint Cube::GenerateColoursVBO(std::vector<glm::vec3>& colours)
{
	GLuint coloursVBO{ 0 };

	glGenBuffers(1, &coloursVBO); // generates 1 unique ID

	// Bind to the GL context, since it's the first time, will also create it
	glBindBuffer(GL_ARRAY_BUFFER, coloursVBO);

	// Fill the buffer with the vertex data
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colours.size(), colours.data(), GL_STATIC_DRAW);

	// Clear binding
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return coloursVBO;
}

GLuint Cube::GenerateNormalsVBO(std::vector<glm::vec3>& normals)
{
	GLuint normalsVBO{ 0 };
	// Generate ID for the VBO
	glGenBuffers(1, &normalsVBO); // generates 1 unique ID

	// Bind to the GL context, since it's the first time, will also create it
	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);

	// Fill the buffer with the vertex data
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);

	// Clear binding
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return normalsVBO;
}

GLuint Cube::GeneratePositionsEBO(std::vector<GLuint>& elements)
{
	GLuint positionsEBO{ 0 };

	glGenBuffers(1, &positionsEBO); // generates 1 unique ID

	// Bind to the GL context, since it's the first time, will also create it
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, positionsEBO);

	// Fill the buffer with the vertex data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * elements.size(), elements.data(), GL_STATIC_DRAW);

	// Clear binding
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Sets the number of elements
	m_numElements = elements.size();

	return positionsEBO;
}

void Cube::SetVAO(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& colours, std::vector<glm::vec3>& normals, std::vector<GLuint>& elements)
{
	// Generate ID for VAO
	glGenVertexArrays(1, &m_VAO);

	// Bind to context, it's first time so also create
	glBindVertexArray(m_VAO);
	// From now on what we do is recorded for this VAO and repeated when we render

	glBindBuffer(GL_ARRAY_BUFFER, GeneratePositionsVBO(vertices));
	// Enable first(0) attribute in the program (the vertices) to stream to the vertex shader
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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GeneratePositionsEBO(elements));

	glBindBuffer(GL_ARRAY_BUFFER, GenerateNormalsVBO(normals));
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

	glBindBuffer(GL_ARRAY_BUFFER, GenerateColoursVBO(colours));
	// Enable second(1) attribute in the program (the vertices) to stream to the vertex shader
	glEnableVertexAttribArray(2);
	// Describe the make up of the vertex stream
	glVertexAttribPointer(
		2, //attribute number
		3, // size in components, 3-xyz
		GL_FLOAT, // type of the item
		GL_FALSE, // normalised or not
		0, // stride
		(void*)0 // offset
	);

	// Clear context
	glBindVertexArray(0);
}

Cube::~Cube()
{
	glDeleteBuffers(1, &m_VAO);
}

bool Cube::CreateCube(int size, glm::vec3 position)
{
	// Move the cube
	m_position = glm::vec3(0);
	m_rotation = glm::vec3(0);
	Move(position);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> colours;
	std::vector<glm::vec3> normals;
	std::vector<GLuint> elements;

	// Generate vertices
	size = size / 2;
	vertices.push_back(glm::vec3(-size,-size, size)); // V1
	vertices.push_back(glm::vec3( size,-size, size)); // V2
	vertices.push_back(glm::vec3( size,-size,-size)); // V3
	vertices.push_back(glm::vec3(-size,-size,-size)); // V4
	vertices.push_back(glm::vec3(-size, size, size)); // V5
	vertices.push_back(glm::vec3( size, size, size)); // V6
	vertices.push_back(glm::vec3( size, size,-size)); // V7
	vertices.push_back(glm::vec3(-size, size,-size)); // V8

	vertices.push_back(glm::vec3(-size,-size, size)); // V1
	vertices.push_back(glm::vec3( size,-size, size)); // V2
	vertices.push_back(glm::vec3( size,-size,-size)); // V3
	vertices.push_back(glm::vec3(-size,-size,-size)); // V4
	vertices.push_back(glm::vec3(-size, size, size)); // V5
	vertices.push_back(glm::vec3( size, size, size)); // V6
	vertices.push_back(glm::vec3( size, size,-size)); // V7
	vertices.push_back(glm::vec3(-size, size,-size)); // V8

	vertices.push_back(glm::vec3(-size,-size, size)); // V1
	vertices.push_back(glm::vec3( size,-size, size)); // V2
	vertices.push_back(glm::vec3( size,-size,-size)); // V3
	vertices.push_back(glm::vec3(-size,-size,-size)); // V4
	vertices.push_back(glm::vec3(-size, size, size)); // V5
	vertices.push_back(glm::vec3( size, size, size)); // V6
	vertices.push_back(glm::vec3( size, size,-size)); // V7
	vertices.push_back(glm::vec3(-size, size,-size)); // V8
	
	// Generate colours
	colours.push_back(glm::vec3(0.0f, 1.0f, 0.0f)); // G
	colours.push_back(glm::vec3(0.0f, 1.0f, 0.0f)); // G
	colours.push_back(glm::vec3(0.0f, 0.0f, 1.0f)); // B
	colours.push_back(glm::vec3(0.0f, 0.0f, 1.0f)); // B
	colours.push_back(glm::vec3(0.0f, 1.0f, 0.0f)); // G
	colours.push_back(glm::vec3(0.0f, 1.0f, 0.0f)); // G
	colours.push_back(glm::vec3(0.0f, 0.0f, 1.0f)); // B
	colours.push_back(glm::vec3(0.0f, 0.0f, 1.0f)); // B

	colours.push_back(glm::vec3(1.0f, 0.5f, 0.0f)); // O
	colours.push_back(glm::vec3(1.0f, 0.0f, 0.0f)); // R
	colours.push_back(glm::vec3(1.0f, 0.0f, 0.0f)); // R
	colours.push_back(glm::vec3(1.0f, 0.5f, 0.0f)); // O
	colours.push_back(glm::vec3(1.0f, 0.5f, 0.0f)); // O
	colours.push_back(glm::vec3(1.0f, 0.0f, 0.0f)); // R
	colours.push_back(glm::vec3(1.0f, 0.0f, 0.0f)); // R
	colours.push_back(glm::vec3(1.0f, 0.5f, 0.0f)); // O

	colours.push_back(glm::vec3(1.0f, 1.0f, 0.0f)); // Y
	colours.push_back(glm::vec3(1.0f, 1.0f, 0.0f)); // Y
	colours.push_back(glm::vec3(1.0f, 1.0f, 0.0f)); // Y
	colours.push_back(glm::vec3(1.0f, 1.0f, 0.0f)); // Y
	colours.push_back(glm::vec3(1.0f, 1.0f, 1.0f)); // W
	colours.push_back(glm::vec3(1.0f, 1.0f, 1.0f)); // W
	colours.push_back(glm::vec3(1.0f, 1.0f, 1.0f)); // W
	colours.push_back(glm::vec3(1.0f, 1.0f, 1.0f)); // W

	// Generate elements
	elements.push_back(0); elements.push_back(1); elements.push_back(5);
	elements.push_back(0); elements.push_back(5); elements.push_back(4);

	elements.push_back(2); elements.push_back(3); elements.push_back(7);
	elements.push_back(2); elements.push_back(7); elements.push_back(6);

	elements.push_back(9); elements.push_back(10); elements.push_back(14);
	elements.push_back(9); elements.push_back(14); elements.push_back(13);

	elements.push_back(15); elements.push_back(11); elements.push_back(8);
	elements.push_back(15); elements.push_back(8); elements.push_back(12);
	
	elements.push_back(18); elements.push_back(17); elements.push_back(16);
	elements.push_back(18); elements.push_back(16); elements.push_back(19);

	elements.push_back(20); elements.push_back(21); elements.push_back(22);
	elements.push_back(20); elements.push_back(22); elements.push_back(23);


	// Generate normals
	for (size_t i = 0; i < 24; i++)
	{
		normals.push_back(glm::vec3(0, 0, 0));
	}

	for (size_t i = 0; i+2 < elements.size(); i += 3)
	{
		glm::vec3 edge1 = vertices.at(elements.at(i + 1)) - vertices.at(elements.at(i));
		glm::vec3 edge2 = vertices.at(elements.at(i + 2)) - vertices.at(elements.at(i));
		glm::vec3 normal = glm::cross(edge1, edge2);
		normal = glm::normalize(normal);

		normals.at(elements.at(i)) += normal;
		normals.at(elements.at(i + 1)) += normal;
		normals.at(elements.at(i + 2)) += normal;
	}

	for (size_t i = 0; i < normals.size(); i++)
	{
		normals.at(i) = glm::normalize(normals.at(i));
	}

	// Set the VAO
	SetVAO(vertices, colours, normals, elements);

    return true;
}

void Cube::Move(glm::vec3 pos)
{
	m_position = m_position + pos;
}

void Cube::Rotate(glm::vec3 rot)
{
	m_rotation = m_rotation + rot;
}

void Cube::Render(GLuint m_program, glm::mat4 combined_xform)
{
	// Use our program. Doing this enables the shaders we attached previously.
	glUseProgram(m_program);

	// Send the combined matrix to the shader in a uniform
	GLuint combined_xform_id = glGetUniformLocation(m_program, "combined_xform");
	glUniformMatrix4fv(combined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));

	// Rotate the cube
	Rotate(glm::vec3(0.001f, 0, 0.001f));

	glm::mat4 rot = glm::mat4(1);
	rot = glm::rotate(rot, m_rotation.x, glm::vec3(1, 0, 0));
	rot = glm::rotate(rot, m_rotation.y, glm::vec3(0, 1, 0));
	rot = glm::rotate(rot, m_rotation.z, glm::vec3(0, 0, 1));
	glm::mat4 trans = glm::translate(glm::mat4(1), m_position);
	glm::mat4 model_xform = trans * rot;

	// Send the model matrix to the shader in a uniform
	GLuint model_xform_id = glGetUniformLocation(m_program, "model_xform");
	glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));

	// Bind our VAO and render
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_numElements, GL_UNSIGNED_INT, (void*)0);
}
