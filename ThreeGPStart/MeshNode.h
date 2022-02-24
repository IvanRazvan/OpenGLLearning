#pragma once

#include <iostream>
#include "MyMesh.h"

/*
	MeshNode class is used in a tree structure for hierarchical transformations 
*/

class MeshNode
{
public:
	MyMesh* m_mesh{ nullptr };
	glm::vec3 m_pos;
	glm::vec3 m_rot;
	MeshNode* m_parent{ nullptr };
	std::vector<MeshNode*> m_children;

	MeshNode() = default;
	MeshNode(MyMesh* mesh, glm::vec3 pos, glm::vec3 rot, MeshNode* parent);
	~MeshNode();
};

