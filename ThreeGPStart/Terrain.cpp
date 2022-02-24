#include "Terrain.h"

GLuint Terrain::GeneratePositionsVBO(std::vector<glm::vec3>& vertices)
{
    GLuint positionsVBO{ 0 };
    // Generate ID for the VBO
    glGenBuffers(1, &positionsVBO); // generates 1 unique ID

    // Bind to the GL context, since it's the first time, will also create it
    glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);

    // Fill the buffer with the vertex data
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Clear binding
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return positionsVBO;
}

GLuint Terrain::GeneratePositionsEBO(std::vector<GLuint>& elements)
{
    GLuint positionsEBO{ 0 };
    // Generate ID for the EBO
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

GLuint Terrain::GenerateNormalsVBO(std::vector<glm::vec3>& normals)
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

GLuint Terrain::GenerateUVCoords(std::vector<glm::vec2>& UVs)
{
    GLuint UVcoords{ 0 };
    // Generate ID for the UVcoords
    glGenBuffers(1, &UVcoords); // generates 1 unique ID

    // Bind to the GL context, since it's the first time, will also create it
    glBindBuffer(GL_ARRAY_BUFFER, UVcoords);

    // Fill the buffer with the vertex data
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * UVs.size(), UVs.data(), GL_STATIC_DRAW);

    // Clear binding
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return UVcoords;
}

GLuint Terrain::GenerateTexture(Helpers::ImageLoader& image)
{
    GLuint texture{ 0 };

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.Width(), image.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.GetData());
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}

void Terrain::SetVAO(std::vector<glm::vec3>& vertices, std::vector<GLuint>& elements, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& UVs)
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

    glBindBuffer(GL_ARRAY_BUFFER, GenerateUVCoords(UVs));
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

// This function returns the red channel of the pixel found at specified coordinates
float Terrain::GetPixelValue(int x, int z, Helpers::ImageLoader& image)
{
    size_t offset = ((size_t)x + (size_t)z * image.Width()) * 4;
    BYTE* imageData = image.GetData();
    return (float)imageData[offset] / 255.0f; // Red channel
}

// This function uses bilinear interpolation to apply the height map
void Terrain::ApplyHeightMap(std::vector<glm::vec3>& vertices, Helpers::ImageLoader& heightmapImage)
{
    float vertexXtoImage = (float)heightmapImage.Width() / (m_numCellsX + 1);
    float vertexZtoImage = (float)heightmapImage.Height() / (m_numCellsZ + 1);

    int w = heightmapImage.Width() - 1;
    int h = heightmapImage.Height() - 1;

    for (size_t z = 0; z <= m_numCellsZ; z++)
    {
        for (size_t x = 0; x <= m_numCellsX; x++)
        {
            int x1 = std::floor((m_numCellsX - x) * vertexXtoImage);
            int x2 = Clamp(x1 + 1, 0, w);

            int y1 = std::floor(z * vertexZtoImage);
            int y2 = Clamp(y1 + 1, 0, h);

            float xp = (m_numCellsX + 1 - x) * vertexXtoImage - x1;
            float yp = z * vertexZtoImage - y1;

            float p11 = GetPixelValue(x1, y1, heightmapImage);
            float p21 = GetPixelValue(x2, y1, heightmapImage);
            float p12 = GetPixelValue(x1, y2, heightmapImage);
            float p22 = GetPixelValue(x2, y2, heightmapImage);

            float px1 = Lerp(p11, p21, xp);
            float px2 = Lerp(p12, p22, xp);

            float height = Lerp(px1, px2, yp);
            
            vertices.at(z * ((size_t)m_numCellsX + 1) + x).y = height * m_scaling + (KenPerlin(x, z) + 1) / 10;
        }

    }
}

float Terrain::Noise(int x, int y)
{
    int n = x + y * 57;  // 57 is the seed – can be tweaked
    n = (n >> 13) ^ n;
    int nn = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
    return 1.0f - ((float)nn / 1073741824.0f);
}

float Terrain::Clamp(float x, int min, int max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

float Terrain::Lerp(float a, float b, float x)
{
    return (1.0f - x) * a + x * b;
}

float Terrain::CosineLerp(float a, float b, float x)
{
    float ft = x * 3.1415927f;
    float f = (1.0f - cos(ft)) * 0.5f;
    return a * (1.0f - f) + b * f;
}

float Terrain::KenPerlin(float xPos, float zPos)
{
    float s = Noise((int)xPos, (int)zPos);
    float t = Noise((int)xPos + 1, (int)zPos);
    float u = Noise((int)xPos, (int)zPos + 1);
    float v = Noise((int)xPos + 1, (int)zPos + 1);
    float c1 = CosineLerp(s, t, xPos);
    float c2 = CosineLerp(u, v, xPos);
    return CosineLerp(c1, c2, zPos);
}

void Terrain::CalculateNormals(std::vector<glm::vec3>& vertices, std::vector<GLuint>& elements, std::vector<glm::vec3>& normals)
{
    for (size_t i = 0; i < elements.size(); i++)
    {
        normals.emplace_back(glm::vec3(0, 0, 0));
    }

    for (size_t i = 0; i < elements.size(); i += 3)
    {
        glm::vec3 edge1 = vertices.at(elements.at(i + 1)) - vertices.at(elements.at(i));
        glm::vec3 edge2 = vertices.at(elements.at(i + 2)) - vertices.at(elements.at(i));
        glm::vec3 normal = glm::cross(edge1, edge2);
        normal = glm::normalize(normal);

        normals.at(elements.at(i)) += normal;
        normals.at(elements.at(i + 1)) += normal;
        normals.at(elements.at(i + 2)) += normal;
    }

    for (size_t i = 0; i < vertices.size(); i++)
    {
        normals.at(i) = glm::normalize(normals.at(i));
    }
}

Terrain::~Terrain()
{
    glDeleteBuffers(1, &m_VAO);
}

bool Terrain::CreateTerrain(int numCellsX, int numCellsZ, float sizeX, float sizeZ, const std::string& textureFilepath, const std::string& heightmapFilepath)
{
    m_sizeX = sizeX;
    m_sizeZ = sizeZ;
    m_numCellsX = numCellsX;
    m_numCellsZ = numCellsZ;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<GLuint> elements;
    std::vector<glm::vec2> UVs;
    Helpers::ImageLoader heightmapImage;
    Helpers::ImageLoader textureImage;

    // Load and generate the texture
    if (textureImage.Load(textureFilepath) == false)
    {
        MessageBox(NULL, L"Terrain texture not loading", L"Error", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    m_texture = GenerateTexture(textureImage);

    // Load the heightmap
    if (heightmapImage.Load(heightmapFilepath) == false)
    {
        MessageBox(NULL, L"Terrain heightmap not loading", L"Error", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    // Set the vertices and UVs
    for (int z = 0; z <= m_numCellsZ; z++)
    {
        for (int x = 0; x <= m_numCellsX; x++)
        {
            vertices.emplace_back(glm::vec3(x * sizeX / m_numCellsX, 0, z * sizeZ / m_numCellsZ));
            UVs.emplace_back(glm::vec2(x, z));
        }
    }

    // Set the elements
    for (int z = 0; z < m_numCellsZ; z++)
    {
        for (int x = 0; x < m_numCellsX; x++)
        {
            if (z % 2 == x % 2)
            {
               elements.emplace_back(z * (m_numCellsX + 1) + x + m_numCellsX + 1);
               elements.emplace_back(z * (m_numCellsX + 1) + x + 1);
               elements.emplace_back(z * (m_numCellsX + 1) + x);
               
               elements.emplace_back(z * (m_numCellsX + 1) + x + m_numCellsX + 1);
               elements.emplace_back(z * (m_numCellsX + 1) + x + m_numCellsX + 2);
               elements.emplace_back(z * (m_numCellsX + 1) + x + 1);
               
            }
            else
            {
                elements.emplace_back(z * (m_numCellsX + 1) + x + m_numCellsX + 1);
                elements.emplace_back(z * (m_numCellsX + 1) + x + m_numCellsX + 2);
                elements.emplace_back(z * (m_numCellsX + 1) + x);

                elements.emplace_back(z * (m_numCellsX + 1) + x + m_numCellsX + 2);
                elements.emplace_back(z * (m_numCellsX + 1) + x + 1);
                elements.emplace_back(z * (m_numCellsX + 1) + x);
            }
        }
    }

    // Apply heighgtmap and calculate normals
    ApplyHeightMap(vertices, heightmapImage);
    CalculateNormals(vertices, elements, normals);
    
    // Set VAO
    SetVAO(vertices, elements, normals, UVs);
 
    return true;
}

void Terrain::Render(GLuint m_program, glm::mat4 combined_xform)
{
    // Use our program. Doing this enables the shaders we attached previously.
    glUseProgram(m_program);

    // Send the combined matrix to the shader in a uniform
    GLuint combined_xform_id = glGetUniformLocation(m_program, "combined_xform");
    glUniformMatrix4fv(combined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));

    glm::mat4 model_xform = glm::mat4(1);

    // Send the model matrix to the shader in a uniform
    GLuint model_xform_id = glGetUniformLocation(m_program, "model_xform");
    glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glUniform1i(glGetUniformLocation(m_program, "sampler_tex"), 0);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_numElements, GL_UNSIGNED_INT, (void*)0);
}
