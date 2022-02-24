#include "MeshNode.h"

MeshNode::MeshNode(MyMesh* mesh, glm::vec3 pos, glm::vec3 rot, MeshNode* parent)
	: m_mesh(mesh), m_pos(pos), m_rot(rot), m_parent(parent)
{

}

MeshNode::~MeshNode()
{
	for (MeshNode* child : m_children)
		delete child;
}
