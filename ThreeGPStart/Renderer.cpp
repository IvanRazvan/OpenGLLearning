#include "Renderer.h"
#include "Camera.h"

Renderer::Renderer() 
{
	// AMBIENT LIGHT
	ambient_light = glm::vec3(0.0f, 0.1f, 0.1f);

	// DIRECTIONAL LIGHTS
	directional_lights.push_back(DirectionalLight(glm::vec3(20, 0, 30), glm::vec3(1, 1, 1), 0.4f));
	num_directional_lights = directional_lights.size();

	//POINT LIGHTS
	point_lights.push_back(PointLight(glm::vec3(  0, 100,   0), glm::vec3(1, 0, 0), 1.0f, 250.0f));
	point_lights.push_back(PointLight(glm::vec3(200, 100, 200), glm::vec3(0, 1, 0), 1.0f, 150.0f));
	point_lights.push_back(PointLight(glm::vec3(400, 100, 400), glm::vec3(0, 0, 1), 1.0f, 250.0f));
	point_lights.push_back(PointLight(glm::vec3(400, 100,	0), glm::vec3(1, 0, 1), 1.0f, 250.0f));
	point_lights.push_back(PointLight(glm::vec3(  0, 100, 400), glm::vec3(0, 1, 1), 1.0f, 250.0f));
	num_point_lights = point_lights.size();

	//SPOT LIGHTS
	spot_lights.push_back(SpotLight(glm::vec3(200, 150, 50), glm::vec3(1, 1, 0), 1.5f, glm::vec3(0, 90, 0), 20, 250.0f));
	num_spot_lights = spot_lights.size();
}

// On exit must clean up any OpenGL resources e.g. the program, the buffers
Renderer::~Renderer()
{
	// TODO: clean up any memory used including OpenGL objects via glDelete* calls
	glDeleteProgram(m_TexturedProgram);
	glDeleteProgram(m_ColouredProgram);
	glDeleteProgram(m_SkyboxProgram);
}

void Renderer::AddLights(GLuint program)
{
	glUseProgram(program);

	//AMBIENT LIGHT
	GLuint ambient_light_id = glGetUniformLocation(program, "ambient_light");
	glUniform3fv(ambient_light_id, 1, glm::value_ptr(ambient_light));

	// DIRECTIONAL LIGHTS
	GLuint num_directional_lights_id = glGetUniformLocation(program, "num_directional_lights");
	glUniform1i(num_directional_lights_id, num_directional_lights);

	for (int i = 0; i < num_directional_lights; i++)
	{
		GLuint directional_light_direction_id = glGetUniformLocation(program, std::string("directional_lights[" + std::to_string(i) + "].direction").c_str());
		glUniform3fv(directional_light_direction_id, 1, glm::value_ptr(directional_lights.at(i).m_direction));

		GLuint directional_light_colour_id = glGetUniformLocation(program, std::string("directional_lights[" + std::to_string(i) + "].colour").c_str());
		glUniform3fv(directional_light_colour_id, 1, glm::value_ptr(directional_lights.at(i).m_colour));

		GLuint directional_light_intensity_id = glGetUniformLocation(program, std::string("directional_lights[" + std::to_string(i) + "].intensity").c_str());
		glUniform1f(directional_light_intensity_id, directional_lights.at(i).m_intensity);
	}

	// POINT LIGHTS
	GLuint num_point_lights_id = glGetUniformLocation(program, "num_point_lights");
	glUniform1i(num_point_lights_id, num_point_lights);

	for (int i = 0; i < num_point_lights; i++)
	{
		GLuint point_light_position_id = glGetUniformLocation(program, std::string("point_lights[" + std::to_string(i) + "].position").c_str());
		glUniform3fv(point_light_position_id, 1, glm::value_ptr(point_lights.at(i).m_position));

		GLuint point_light_colour_id = glGetUniformLocation(program, std::string("point_lights[" + std::to_string(i) + "].colour").c_str());
		glUniform3fv(point_light_colour_id, 1, glm::value_ptr(point_lights.at(i).m_colour));

		GLuint point_light_range_id = glGetUniformLocation(program, std::string("point_lights[" + std::to_string(i) + "].range").c_str());
		glUniform1f(point_light_range_id, point_lights.at(i).m_range);

		GLuint point_light_intensity_id = glGetUniformLocation(program, std::string("point_lights[" + std::to_string(i) + "].intensity").c_str());
		glUniform1f(point_light_intensity_id, point_lights.at(i).m_intensity);
	}

	// SPOT LIGHTS
	GLuint num_spot_lights_id = glGetUniformLocation(program, "num_spot_lights");
	glUniform1i(num_spot_lights_id, num_spot_lights);

	for (int i = 0; i < num_spot_lights; i++)
	{
		GLuint spot_light_position_id = glGetUniformLocation(program, std::string("spot_lights[" + std::to_string(i) + "].base.position").c_str());
		glUniform3fv(spot_light_position_id, 1, glm::value_ptr(spot_lights.at(i).m_position));

		GLuint spot_light_colour_id = glGetUniformLocation(program, std::string("spot_lights[" + std::to_string(i) + "].base.colour").c_str());
		glUniform3fv(spot_light_colour_id, 1, glm::value_ptr(spot_lights.at(i).m_colour));

		GLuint spot_light_intensity_id = glGetUniformLocation(program, std::string("spot_lights[" + std::to_string(i) + "].base.intensity").c_str());
		glUniform1f(spot_light_intensity_id, spot_lights.at(i).m_intensity);

		GLuint spot_light_range_id = glGetUniformLocation(program, std::string("spot_lights[" + std::to_string(i) + "].base.range").c_str());
		glUniform1f(spot_light_range_id, spot_lights.at(i).m_range);

		GLuint spot_light_direction_id = glGetUniformLocation(program, std::string("spot_lights[" + std::to_string(i) + "].direction").c_str());
		glUniform3fv(spot_light_direction_id, 1, glm::value_ptr(spot_lights.at(i).m_direction));

		GLuint spot_light_cosangle_id = glGetUniformLocation(program, std::string("spot_lights[" + std::to_string(i) + "].cosangle").c_str());
		glUniform1f(spot_light_cosangle_id, spot_lights.at(i).m_cos);
	}

	glUseProgram(0);
}

// Use IMGUI for a simple on screen GUI
void Renderer::DefineGUI()
{
	// Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui::Begin("3GP");						// Create a window called "3GP" and append into it.

	ImGui::Text("Visibility.");					// Display some text (you can use a format strings too)	

	ImGui::Checkbox("Wireframe", &m_wireframe);	// A checkbox linked to a member variable

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
	ImGui::End();
}

// Load, compile and link the shaders and create a program object to host them
bool Renderer::CreateProgram(GLuint& program, std::string vertex, std::string fragment)
{
	// Create a new program (returns a unqiue id)
	program = glCreateProgram();

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, vertex) };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, fragment) };
	if (vertex_shader == 0 || fragment_shader == 0)
		return false;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(program, vertex_shader);

	// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
	// Not needed if you use (location=0) in the vertex shader itself
	//glBindAttribLocation(m_program, 0, "vertex_position");

	// Attach the fragment shader (copies it)
	glAttachShader(program, fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(program))
		return false;

	return true;
}

// Load / create geometry into OpenGL buffers	
bool Renderer::InitialiseGeometry()
{
	// Load and compile shaders into m_program
	if (!CreateProgram(m_TexturedProgram, "Data/Shaders/vertex_shader.vert", "Data/Shaders/fragment_shader.frag"))
		return false;

	if (!CreateProgram(m_SkyboxProgram, "Data/Shaders/skybox.vert", "Data/Shaders/skybox.frag"))
		return false;

	if (!CreateProgram(m_ColouredProgram, "Data/Shaders/cube.vert", "Data/Shaders/cube.frag"))
		return false;

	if (!skybox.CreateSkybox("Data\\Models\\Sky\\Hills"))
		return false;
	
	if (!terrain.CreateTerrain(400, 400, 400, 400, "Data\\Textures\\white.png", "Data\\Heightmaps\\curvy.gif"))
		return false;

	if (!cube.CreateCube(10, glm::vec3(100, 5, 20)))
		return false;

	if (!aquapig.CreateObject("Data\\Models\\AquaPig\\"))
		return false;

	AddLights(m_TexturedProgram);
	AddLights(m_ColouredProgram);

	return true;
}

// Render the scene. Passed the delta time since last called.
void Renderer::Render(const Helpers::Camera& camera, float deltaTime)
{			
	// Configure pipeline settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Wireframe mode controlled by ImGui
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Clear buffers from previous frame
	glClearColor(0.0f, 0.0f, 0.0f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Compute viewport and projection matrix
	GLint viewportSize[4];
	glGetIntegerv(GL_VIEWPORT, viewportSize);
	const float aspect_ratio = viewportSize[2] / (float)viewportSize[3];
	
	glm::mat4 projection_xform = glm::perspective(glm::radians(45.0f), aspect_ratio, 1.0f, 1000.0f);
	// Compute camera view matrix and combine with projection matrix for passing to shader
	glm::mat4 view_xform = glm::lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetLookVector(), camera.GetUpVector());
	glm::mat4 combined_xform = projection_xform * view_xform;

	// Bind our VAO and render
	skybox.Render(m_SkyboxProgram, projection_xform, view_xform);
	
	GLuint camera_posisition_id;

	glUseProgram(m_TexturedProgram);
	camera_posisition_id = glGetUniformLocation(m_TexturedProgram, "camera_posisition");
	glUniform3fv(camera_posisition_id, 1, glm::value_ptr(camera.GetPosition()));

	glUseProgram(m_ColouredProgram);
	camera_posisition_id = glGetUniformLocation(m_ColouredProgram, "camera_posisition");
	glUniform3fv(camera_posisition_id, 1, glm::value_ptr(camera.GetPosition()));

	cube.Render(m_ColouredProgram, combined_xform);
	terrain.Render(m_TexturedProgram, combined_xform);
	aquapig.Render(m_TexturedProgram, combined_xform);
}

