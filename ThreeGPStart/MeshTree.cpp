#include "MeshTree.h"

// This function returns a MeshNode found by name
MeshNode* MeshTree::FindMesh(std::string name, MeshNode* node)
{
    if (name == node->m_mesh->GetName())
        return node;

    for (MeshNode* child : node->m_children)
    {
        MeshNode* result = FindMesh(name, child);
        if (result != nullptr)
            return result;
    }

    return nullptr;
}

// This function sets the transform of a node and its children
void MeshTree::SetTransforms(MeshNode* node, glm::mat4 xform)
{
    xform = glm::translate(xform, node->m_pos);
    xform = glm::rotate(xform, node->m_rot.x, glm::vec3(1, 0, 0));
    xform = glm::rotate(xform, node->m_rot.y, glm::vec3(0, 1, 0));
    xform = glm::rotate(xform, node->m_rot.z, glm::vec3(0, 0, 1));

    node->m_mesh->SetXForm(xform);

    for (MeshNode* child : node->m_children)
    {
        SetTransforms(child, xform);
    }
}

MeshTree::~MeshTree()
{
    delete m_root;
}

// This function returns a MeshNode found by name
MeshNode* MeshTree::FindMesh(std::string name)
{
    if(m_root != nullptr)
        return FindMesh(name, m_root);

    return nullptr;
}

// This function moves a Mesh found by name
void MeshTree::AddPosition(std::string name, glm::vec3 pos)
{
    MeshNode* node = FindMesh(name);

    if (node != nullptr)
    {
        node->m_pos = node->m_pos + pos;
    }
}

// This function rotates a Mesh found by name
void MeshTree::AddRotation(std::string name, glm::vec3 rot)
{
    MeshNode* node = FindMesh(name);

    if (node != nullptr)
    {
        node->m_rot = node->m_rot + rot;
    }
}

// This function returns the position of a Mesh found by name
glm::vec3 MeshTree::FindPosition(std::string name)
{
    MeshNode* node = FindMesh(name);

    glm::vec3 pos = glm::vec3(0, 0, 0);

    while (node != nullptr)
    {
        pos = pos + node->m_pos;
        node = node->m_parent;
    }

    return pos;
}

// This function returns the rotation of a Mesh found by name
glm::vec3 MeshTree::FindRotation(std::string name)
{
    MeshNode* node = FindMesh(name);

    glm::vec3 rot = glm::vec3(0, 0, 0);

    while (node != nullptr)
    {
        rot = rot + node->m_rot;
        node = node->m_parent;
    }

    return rot;
}

// This function adds a new node in the tree
void MeshTree::AddNode(MyMesh* mesh, glm::vec3 pos, glm::vec3 rot, std::string parent)
{
    if (parent == "null" || m_root == nullptr)
    {
        m_root = new MeshNode(mesh, pos, rot, nullptr);
    }
    else
    {
        MeshNode* node = FindMesh(parent);

        if (node != nullptr)
        {
            node->m_children.push_back(new MeshNode(mesh, pos, rot, node));
        }
    }
}

// This function sets all transforms to 1
void MeshTree::SetTransforms()
{
    if (m_root != nullptr)
        SetTransforms(m_root, glm::mat4(1));
}
