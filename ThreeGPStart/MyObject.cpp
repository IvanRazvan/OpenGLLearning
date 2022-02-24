#include "MyObject.h"

void MyObject::GenerateTexture()
{
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_texture.Width(), m_texture.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_texture.GetData());
	glGenerateMipmap(GL_TEXTURE_2D);
}

MyObject::~MyObject()
{
	for (MyMesh* mesh : m_meshes)
		delete mesh;
}

bool MyObject::CreateObject(std::string filepath)
{
	// Read information from a file regarding texture and meshes
	ifstream in(filepath + "info.txt");

	if (in.is_open() == false)
		return false;

	string textureName;
	in >> textureName;

	if (m_texture.Load(filepath + textureName) == false)
		return false;
	GenerateTexture();

	int meshCount;
	in >> meshCount;

	for (GLuint i = 0; i < meshCount; i++)
	{
		string meshName;
		in >> meshName;

		string parent;
		in >> parent;

		float x, y, z;
		in >> x >> y >> z;
		glm::vec3 pos = glm::vec3(x, y, z);

		in >> x >> y >> z;
		glm::vec3 rot = glm::vec3(glm::radians(x), glm::radians(y), glm::radians(z));

		m_meshes.push_back(new MyMesh());
		if (m_meshes.at(i)->Create(filepath, meshName, pos, rot) == false)
			return false;

		// Add each mesh in the tree 
		m_tree.AddNode(m_meshes.at(i), pos, rot, parent);
	}
	
	in.close();

	return true;
}

void MyObject::Render(GLuint m_program, glm::mat4 combined_xform)
{
	// Use our program. Doing this enables the shaders we attached previously.
	glUseProgram(m_program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(glGetUniformLocation(m_program, "sampler_tex"), 0);

	// Send the combined matrix to the shader in a uniform
	GLuint combined_xform_id = glGetUniformLocation(m_program, "combined_xform");
	glUniformMatrix4fv(combined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));

	m_tree.AddPosition("hull.obj", glm::vec3(0, 0, 0.01f));
	m_tree.AddRotation("propeller.obj", glm::vec3(0, 0.01f, 0));
	m_tree.AddRotation("gun_base.obj", glm::vec3(0, 0.01f, 0));

	m_tree.SetTransforms();

	for (MyMesh* mesh : m_meshes)
	{
		mesh->Render(m_program);
	}
}
