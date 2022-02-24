#pragma once
#include "ExternalLibraryHeaders.h"
#include "Helper.h"
#include "ImageLoader.h"

/*
	Terrain class implements the generation, handling and rendering of a terrain.
*/

class Terrain
{
private:
	int m_numCellsX{ 0 }, m_numCellsZ{ 0 };
	float m_sizeX{ 0 }, m_sizeZ{ 0 };
	float m_scaling{ 100.0f };

	GLuint m_VAO{ 0 };
	GLuint m_texture{ 0 };
	GLuint m_numElements{ 0 };

	GLuint GeneratePositionsVBO(std::vector<glm::vec3>& vertices);
	GLuint GeneratePositionsEBO(std::vector<GLuint>& elements);
	GLuint GenerateNormalsVBO(std::vector<glm::vec3>& normals);
	GLuint GenerateUVCoords(std::vector<glm::vec2>& UVs);
	GLuint GenerateTexture(Helpers::ImageLoader& image);
	void SetVAO(std::vector<glm::vec3>& vertices, std::vector<GLuint>& elements, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& UVs);

	float GetPixelValue(int x, int z, Helpers::ImageLoader& image);
	void ApplyHeightMap(std::vector<glm::vec3>& vertices, Helpers::ImageLoader& heightmapImage);
	float Noise(int x, int y);
	float Clamp(float x, int min, int max);
	float Lerp(float a, float b, float x);
	float CosineLerp(float a, float b, float x);
	float KenPerlin(float xPos, float zPos);
	void CalculateNormals(std::vector<glm::vec3>& vertices, std::vector<GLuint>& elements, std::vector<glm::vec3>& normals);

public:
	Terrain() = default;
	~Terrain();
	
	bool CreateTerrain(int numCellsX, int numCellsZ, float sizeX, float sizeZ, const std::string& textureFilepath, const std::string& heightmapFilepath);
	
	void Render(GLuint program, glm::mat4 combined_xform);
};

