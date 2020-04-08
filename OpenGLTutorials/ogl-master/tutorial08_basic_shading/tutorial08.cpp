// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

GLuint depthrenderbuffer;
GLuint DepthTextureID;

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

glm::vec3 LightPos = glm::vec3(4, 4, 4);

struct Shader
{
	std::string Name = "";
	GLuint ID = 0;
	GLuint MatrixID = 0;
	GLuint ViewMatrixID = 0;
	GLuint ModelMatrixID = 0;
	GLuint LightID = 0;
	int in_PositionLocation = 0;
	int in_UVLocation = 0;
	int in_NormalLocation = 0;
	GLuint TextureID = 0;
	GLuint DepthTextureID = 0;
};

struct Mesh
{
	GLuint vertbId = 0;
	std::vector<glm::vec3> verts;
	GLuint normbId = 0;
	std::vector<glm::vec3> norms;
	GLuint uvbId = 0;
	std::vector<glm::vec2> uvs;
	glm::mat4 ViewMatrix;
	glm::mat4 ModelMatrix;
	glm::mat4 MVP;
	std::vector<Shader> ShaderPasses;
	GLuint Texture = 0;
	GLuint VertexArrayID = 0;
};

Shader loadShaderPass(const std::string& vert, const std::string& frag)
{
	Shader shaderPass;

	shaderPass.Name = vert;

	shaderPass.ID = LoadShaders(vert.c_str(), frag.c_str());	
	glUseProgram(shaderPass.ID);
	
	shaderPass.MatrixID = glGetUniformLocation(shaderPass.ID, "MVP");
	shaderPass.ViewMatrixID = glGetUniformLocation(shaderPass.ID, "V");
	shaderPass.ModelMatrixID = glGetUniformLocation(shaderPass.ID, "M");
	shaderPass.LightID = glGetUniformLocation(shaderPass.ID, "LightPosition_worldspace");
	shaderPass.DepthTextureID = glGetUniformLocation(shaderPass.ID, "depthTexture");	
	

	// VERTS
	shaderPass.in_PositionLocation = glGetAttribLocation(shaderPass.ID, "vertexPosition_modelspace");
	glEnableVertexAttribArray(shaderPass.in_PositionLocation);
	glVertexAttribPointer(shaderPass.in_PositionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up the vertex attributes pointer

	// UVS
	shaderPass.in_UVLocation = glGetAttribLocation(shaderPass.ID, "vertexUV");
	glEnableVertexAttribArray(shaderPass.in_UVLocation);
	glVertexAttribPointer(shaderPass.in_UVLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// NORMS
	shaderPass.in_NormalLocation = glGetAttribLocation(shaderPass.ID, "vertexNormal_modelspace");
	glEnableVertexAttribArray(shaderPass.in_NormalLocation);
	glVertexAttribPointer(shaderPass.in_NormalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	return shaderPass;
}

// Bind the shader
//void bindShader(GLuint shaderID)
//{
//	glUseProgram(shaderID);
//}
//
//// Unbind the shader
//void unbindShader()
//{
//	glUseProgram(0);
//}

void initDepthBuffer()
{
	glGenTextures(1, &DepthTextureID);
	glBindTexture(GL_TEXTURE_2D, DepthTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, DepthTextureID, 0);
}

void initCube(Mesh& mesh)
{
	//model.verts = std::vector<glm::vec3>(24);
	mesh.norms = std::vector<glm::vec3>(36);

	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float sx = 1.f;
	float sy = 1.f;
	float sz = 1.f;

	float edgeLenght = 1.f;
	float halfLenght = edgeLenght / 2;

	// FRONT facet
	mesh.verts.push_back(glm::vec3(-halfLenght, -halfLenght, halfLenght));
	mesh.verts.push_back(glm::vec3(halfLenght, -halfLenght, halfLenght));
	mesh.verts.push_back(glm::vec3(halfLenght, halfLenght, halfLenght));
	mesh.verts.push_back(glm::vec3(-halfLenght, halfLenght, halfLenght));
	mesh.verts.push_back(glm::vec3(halfLenght, halfLenght, halfLenght));
	mesh.verts.push_back(glm::vec3(-halfLenght, -halfLenght, halfLenght));
	for (int i = 0; i < 6; i++) mesh.norms[i] = glm::vec3(0, 0, 1);

	// RIGHT facet
	mesh.verts.push_back(glm::vec3(halfLenght, halfLenght, halfLenght));
	mesh.verts.push_back(glm::vec3(halfLenght, halfLenght, -halfLenght));
	mesh.verts.push_back(glm::vec3(halfLenght, -halfLenght, -halfLenght));
	mesh.verts.push_back(glm::vec3(halfLenght, halfLenght, halfLenght));
	mesh.verts.push_back(glm::vec3(halfLenght, -halfLenght, -halfLenght));
	mesh.verts.push_back(glm::vec3(halfLenght, -halfLenght, halfLenght));
	for (int i = 6; i < 12; i++) mesh.norms[i] = glm::vec3(1, 0, 0);

	// BACK facet
	mesh.verts.push_back(glm::vec3(-halfLenght, -halfLenght, -halfLenght));
	mesh.verts.push_back(glm::vec3(halfLenght, -halfLenght, -halfLenght));
	mesh.verts.push_back(glm::vec3(halfLenght, halfLenght, -halfLenght));
	mesh.verts.push_back(glm::vec3(-halfLenght, -halfLenght, -halfLenght));
	mesh.verts.push_back(glm::vec3(halfLenght, halfLenght, -halfLenght));
	mesh.verts.push_back(glm::vec3(-halfLenght, halfLenght, -halfLenght));
	for (int i = 12; i < 18; i++) mesh.norms[i] = glm::vec3(0, 0, 1);

	// LEFT facet
	mesh.verts.push_back(glm::vec3(-halfLenght, -halfLenght, -halfLenght));
	mesh.verts.push_back(glm::vec3(-halfLenght, -halfLenght, halfLenght));
	mesh.verts.push_back(glm::vec3(-halfLenght, halfLenght, halfLenght));
	mesh.verts.push_back(glm::vec3(-halfLenght, -halfLenght, -halfLenght));
	mesh.verts.push_back(glm::vec3(-halfLenght, halfLenght, halfLenght));
	mesh.verts.push_back(glm::vec3(-halfLenght, halfLenght, -halfLenght));
	for (int i = 18; i < 24; i++) mesh.norms[i] = glm::vec3(1, 0, 0);

	// TOP facet
	mesh.verts.push_back(glm::vec3(halfLenght, halfLenght, halfLenght));
	mesh.verts.push_back(glm::vec3(-halfLenght, halfLenght, halfLenght));
	mesh.verts.push_back(glm::vec3(halfLenght, halfLenght, -halfLenght));
	mesh.verts.push_back(glm::vec3(-halfLenght, halfLenght, halfLenght));
	mesh.verts.push_back(glm::vec3(halfLenght, halfLenght, -halfLenght));
	mesh.verts.push_back(glm::vec3(-halfLenght, halfLenght, -halfLenght));
	for (int i = 24; i < 30; i++) mesh.norms[i] = glm::vec3(0, 1, 0);

	// BOTTOM facet
	mesh.verts.push_back(glm::vec3(-halfLenght, -halfLenght, -halfLenght));
	mesh.verts.push_back(glm::vec3(halfLenght, -halfLenght, -halfLenght));
	mesh.verts.push_back(glm::vec3(-halfLenght, -halfLenght, halfLenght));
	mesh.verts.push_back(glm::vec3(halfLenght, -halfLenght, -halfLenght));
	mesh.verts.push_back(glm::vec3(-halfLenght, -halfLenght, halfLenght));
	mesh.verts.push_back(glm::vec3(halfLenght, -halfLenght, halfLenght));
	for (int i = 30; i < 36; i++) mesh.norms[i] = glm::vec3(0, 1, 0);

	glGenVertexArrays(1, &mesh.VertexArrayID);
	glGenBuffers(1, &mesh.vertbId);
	glGenBuffers(1, &mesh.normbId);

	/*glBindVertexArray(mesh.VertexArrayID);*/

	// Get a handle for our "MVP" uniform
	// Get a handle for our "MVP" uniform
	mesh.ShaderPasses.push_back(loadShaderPass("shader.vert", "shader.frag"));
	mesh.Texture = loadDDS("uvmap.DDS");	

	// VERTS
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertbId);
	glBufferData(GL_ARRAY_BUFFER, mesh.verts.size() * sizeof(glm::vec3), mesh.verts.data(), GL_STATIC_DRAW);

	// UVS
	//glBindBuffer(GL_ARRAY_BUFFER, mesh.uvbId);
	//glBufferData(GL_ARRAY_BUFFER, mesh.uvs.size() * sizeof(glm::vec3), mesh.uvs.data(), GL_STATIC_DRAW);

	// NORMS
	glBindBuffer(GL_ARRAY_BUFFER, mesh.normbId);
	glBufferData(GL_ARRAY_BUFFER, mesh.norms.size() * sizeof(glm::vec3), mesh.norms.data(), GL_STATIC_DRAW);
}


void initObj(Mesh& mesh)
{
	loadOBJ("suzanne.obj", mesh.verts, mesh.uvs, mesh.norms);

	glGenVertexArrays(1, &mesh.VertexArrayID);
	glGenBuffers(1, &mesh.normbId);
	glGenBuffers(1, &mesh.vertbId);
	glGenBuffers(1, &mesh.uvbId);
	
	// Get a handle for our "MVP" uniform
	mesh.ShaderPasses.push_back(loadShaderPass("silhouette.vert", "silhouette.frag"));
	mesh.ShaderPasses.push_back(loadShaderPass("silhouette2.vert", "silhouette2.frag"));	
	mesh.Texture = loadDDS("uvmap.DDS");	

	// VERTS
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertbId);
	glBufferData(GL_ARRAY_BUFFER, mesh.verts.size() * sizeof(glm::vec3), mesh.verts.data(), GL_STATIC_DRAW);

	// UVS
	glBindBuffer(GL_ARRAY_BUFFER, mesh.uvbId);
	glBufferData(GL_ARRAY_BUFFER, mesh.uvs.size() * sizeof(glm::vec2), mesh.uvs.data(), GL_STATIC_DRAW);

	// NORMS
	glBindBuffer(GL_ARRAY_BUFFER, mesh.normbId);
	glBufferData(GL_ARRAY_BUFFER, mesh.norms.size() * sizeof(glm::vec3), mesh.norms.data(), GL_STATIC_DRAW);
}

void draw(const Mesh& mesh)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	for (auto shader = mesh.ShaderPasses.begin(); shader != mesh.ShaderPasses.end(); shader++)
	{		
		glUseProgram(shader->ID);
		glBindVertexArray(mesh.VertexArrayID);

		//glClear(GL_COLOR_BUFFER_BIT);// | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glUniform3f(shader->LightID, LightPos.x, LightPos.y, LightPos.z);
		glUniformMatrix4fv(shader->MatrixID, 1, GL_FALSE, &mesh.MVP[0][0]);
		glUniformMatrix4fv(shader->ViewMatrixID, 1, GL_FALSE, &mesh.ViewMatrix[0][0]);
		glUniformMatrix4fv(shader->ModelMatrixID, 1, GL_FALSE, &mesh.ModelMatrix[0][0]);

	
		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, DepthTextureID);
		//glUniform1i(shader->DepthTextureID, 2);
		//

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.vertbId);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.uvbId);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.normbId);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, mesh.verts.size());

			
		glBindVertexArray(0);
		glUseProgram(0);// unbind shader
	}
}


void destroy(const Mesh& mesh)
{
	if (mesh.vertbId > 0) glDeleteBuffers(1, &mesh.vertbId);
	if (mesh.uvbId > 0) glDeleteBuffers(1, &mesh.uvbId);
	if (mesh.normbId > 0) glDeleteBuffers(1, &mesh.normbId);

	for (auto shader = mesh.ShaderPasses.begin(); shader != mesh.ShaderPasses.end(); shader++)
	{
		if (shader->ID > 0) glDeleteProgram(shader->ID);
	}

	if (mesh.Texture > 0) glDeleteTextures(1, &mesh.Texture);
	if (mesh.VertexArrayID > 0)glDeleteVertexArrays(1, &mesh.VertexArrayID);
}

int main(void)
{

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tutorial 08 - Basic Shading", NULL, NULL);
	if (window == NULL) 
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	//glEnable(GL_DEPTH_TEST);	
	//glDepthFunc(GL_LESS);

	//////////////////////////// TODO
	//////////////////////////// TODO
	//////////////////////////// TODO
	//////////////////////////// TODO
	//////////////////////////// TODO
	//glEnable(GL_CULL_FACE);

	initDepthBuffer();

	// INIT OBJ
	// Read our .obj file
	// Load the texture
	Mesh obj;
	initObj(obj);

	// INIT CUBE
	Mesh cube;
	initCube(cube);

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();

		cube.ViewMatrix = getViewMatrix();
		cube.ModelMatrix = glm::mat4(1.0);
		cube.ModelMatrix = translate(cube.ModelMatrix, glm::vec3(0, 0, 2));
		cube.MVP = ProjectionMatrix * cube.ViewMatrix * cube.ModelMatrix;
		draw(cube);

		obj.ViewMatrix = getViewMatrix();
		obj.ModelMatrix = glm::mat4(1.0);
		obj.ModelMatrix = translate(obj.ModelMatrix, glm::vec3(0, 0, 0));
		obj.MVP = ProjectionMatrix * obj.ViewMatrix * obj.ModelMatrix;
		draw(obj);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (
		glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteTextures(1, &DepthTextureID);
	destroy(obj);
	destroy(cube);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

