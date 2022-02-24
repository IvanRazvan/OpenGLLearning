#pragma once

#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"

#include "Terrain.h"
#include "Cube.h"
#include "Skybox.h"
#include "MyObject.h"
#include "Light.h"

class Renderer
{
private:
	// Program object - to host shaders
	GLuint m_TexturedProgram{ 0 };
	GLuint m_ColouredProgram{ 0 };
	GLuint m_SkyboxProgram{ 0 };

	Cube cube;
	Terrain terrain;
	Skybox skybox;
	MyObject aquapig;

	glm::vec3 ambient_light;

	int num_directional_lights{ 0 };
	std::vector<DirectionalLight> directional_lights;

	int num_point_lights{ 0 };
	std::vector<PointLight> point_lights;

	int num_spot_lights{ 0 };
	std::vector<SpotLight> spot_lights;

	bool m_wireframe{ false };

	bool CreateProgram(GLuint& program, std::string vertex, std::string fragment);
public:
	Renderer();
	~Renderer();

	void AddLights(GLuint program);

	// Draw GUI
	void DefineGUI();

	// Create and / or load geometry, this is like 'level load'
	bool InitialiseGeometry();

	// Render the scene
	void Render(const Helpers::Camera& camera, float deltaTime);
};

