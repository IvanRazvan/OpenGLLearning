#pragma once

#include "MeshNode.h"

/*
	MeshTree class is a tree structure for hierarchical transformations
*/

class MeshTree
{
private:
	MeshNode* m_root{ nullptr };

	MeshNode* FindMesh(std::string name, MeshNode* node);
	void SetTransforms(MeshNode* node, glm::mat4 xform);

public:
	~MeshTree();

	MeshNode* FindMesh(std::string name);

	void AddPosition(std::string name, glm::vec3 pos);
	void AddRotation(std::string name, glm::vec3 rot);

	glm::vec3 FindPosition(std::string name);
	glm::vec3 FindRotation(std::string name);

	void AddNode(MyMesh* mesh, glm::vec3 pos, glm::vec3 rot, std::string parent);

	void SetTransforms();

};

