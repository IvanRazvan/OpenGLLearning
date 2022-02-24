#pragma once
#include <fstream>
#include "MyMesh.h"
#include "MeshTree.h"

using namespace std;

/*
	MyObject class implements the generation, handling and rendering of an object.
*/

class MyObject
{
private:
	std::vector<MyMesh*> m_meshes;
	MeshTree m_tree;
	Helpers::ImageLoader m_texture;
	GLuint tex{ 0 };

	void GenerateTexture();

public:
	MyObject() = default;
	~MyObject();

	
	bool CreateObject(std::string filepath);
	
	void Render(GLuint m_program, glm::mat4 combined_xform);
};

