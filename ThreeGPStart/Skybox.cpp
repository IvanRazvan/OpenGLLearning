#include "Skybox.h"

GLuint Skybox::GeneratePositionsVBO(std::vector<glm::vec3>& vertices)
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

GLuint Skybox::GeneratePositionsEBO(std::vector<GLuint>& elements)
{
	GLuint positionsEBO{ 0 };

	glGenBuffers(1, &positionsEBO); // generates 1 unique ID

	// Bind to the GL context, since it's the first time, will also create it
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, positionsEBO);

	// Fill the buffer with the vertex data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * elements.size(), elements.data(), GL_STATIC_DRAW);

	// Clear binding
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_numElements = elements.size();

	return positionsEBO;
}

void Skybox::GenerateTexture(std::string facesFilenames[6])
{
	GLuint texture{ 0 };

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; i++)
	{
		Helpers::ImageLoader image;
		if (image.Load(facesFilenames[i]) == false)
		{
			MessageBox(NULL, L"Skybox texture not loading", L"Error", MB_OK | MB_ICONEXCLAMATION);
			return;
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, image.Width(), image.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.GetData());
	}
}

void Skybox::SetVAO(std::vector<glm::vec3>& vertices, std::vector<GLuint>& elements)
{
	// Generate ID for VAO
	glGenVertexArrays(1, &m_VAO);

	// Bind to context, it's first time so also create
	glBindVertexArray(m_VAO);
	// From now on what we do is recorded for this VAO and repeated when we render

	glBindBuffer(GL_ARRAY_BUFFER, GeneratePositionsVBO(vertices));
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

	// Clear context
	glBindVertexArray(0);
}

Skybox::~Skybox()
{
	glDeleteBuffers(1, &m_VAO);
}

bool Skybox::CreateSkybox(const std::string& fileDir)
{
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> elements;
	std::string facesFilenames[6];
	
	// Generate vertices
	vertices.push_back(glm::vec3(-1.0f,-1.0f, 1.0f)); // V1
	vertices.push_back(glm::vec3( 1.0f,-1.0f, 1.0f)); // V2
	vertices.push_back(glm::vec3( 1.0f,-1.0f,-1.0f)); // V3
	vertices.push_back(glm::vec3(-1.0f,-1.0f,-1.0f)); // V4
	vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f)); // V5
	vertices.push_back(glm::vec3( 1.0f, 1.0f, 1.0f)); // V6
	vertices.push_back(glm::vec3( 1.0f, 1.0f,-1.0f)); // V7
	vertices.push_back(glm::vec3(-1.0f, 1.0f,-1.0f)); // V8

	// Generate elements
	elements.push_back(1); elements.push_back(2); elements.push_back(6);
	elements.push_back(6); elements.push_back(5); elements.push_back(1);

	elements.push_back(0); elements.push_back(4); elements.push_back(7);
	elements.push_back(7); elements.push_back(3); elements.push_back(0);

	elements.push_back(4); elements.push_back(5); elements.push_back(6);
	elements.push_back(6); elements.push_back(7); elements.push_back(4);

	elements.push_back(0); elements.push_back(3); elements.push_back(2);
	elements.push_back(2); elements.push_back(1); elements.push_back(0);

	elements.push_back(0); elements.push_back(1); elements.push_back(5);
	elements.push_back(5); elements.push_back(4); elements.push_back(0);

	elements.push_back(3); elements.push_back(7); elements.push_back(6);
	elements.push_back(6); elements.push_back(2); elements.push_back(3);

	facesFilenames[0] = fileDir + "\\skybox_right.jpg";
	facesFilenames[1] = fileDir + "\\skybox_left.jpg";
	facesFilenames[2] = fileDir + "\\skybox_top.jpg";
	facesFilenames[3] = fileDir + "\\skybox_bottom.jpg";
	facesFilenames[4] = fileDir + "\\skybox_back.jpg";
	facesFilenames[5] = fileDir + "\\skybox_front.jpg";

	// Generate cubemap
	GenerateTexture(facesFilenames);

	// Set VAO
	SetVAO(vertices, elements);

    return true;
}

void Skybox::Render(GLuint program, glm::mat4 projection_xform, glm::mat4 view_xform)
{
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);

	// Use our program. Doing this enables the shaders we attached previously.
	glUseProgram(program);

	glm::mat4 combined_xform = projection_xform * glm::mat4(glm::mat3(view_xform));

	// Send the combined matrix to the shader in a uniform
	GLuint combined_xform_id = glGetUniformLocation(program, "combined_xform");
	glUniformMatrix4fv(combined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_numElements, GL_UNSIGNED_INT, 0);

	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
}
