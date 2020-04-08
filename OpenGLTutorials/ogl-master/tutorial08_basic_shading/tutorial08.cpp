﻿// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

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

GLuint DepthTextureID;
//GLuint ColorTextureID;
GLuint DepthFrameBufferID;

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
	Shader shader;
	//std::vector<Shader> ShaderPasses;

	GLuint vertbId = 0;
	std::vector<glm::vec3> verts;
	GLuint normbId = 0;
	std::vector<glm::vec3> norms;
	GLuint uvbId = 0;
	std::vector<glm::vec2> uvs;
	glm::mat4 ViewMatrix;
	glm::mat4 ModelMatrix;
	glm::mat4 MVP;
	GLuint Texture = 0;	
	GLuint VertexArrayID = 0;
};

Shader loadShaderPass(const std::string& vert, const std::string& frag)
{
	Shader shaderPass;

	shaderPass.Name = vert;

	shaderPass.ID = LoadShaders(vert.c_str(), frag.c_str());		
	
	/*glGenVertexArrays(1, &shaderPass.VertexArrayID);*/

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

static const GLenum FrameBuffers[] = {
	GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT
};

// We are just intersted in the "depth" value 
// to use in the next pass
void initDepthTexture()
{
	// create a RGBA color texture
	//glGenTextures(1, &ColorTextureID);
	//glBindTexture(GL_TEXTURE_2D, ColorTextureID);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
	//	SCREEN_WIDTH, SCREEN_HEIGHT,
	//	0, GL_RGBA, GL_UNSIGNED_BYTE,
	//	NULL);

	// create a depth texture
	glGenTextures(1, &DepthTextureID);
	glBindTexture(GL_TEXTURE_2D, DepthTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0, GL_DEPTH_COMPONENT, GL_FLOAT,
		NULL);

	// attach color	
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, DepthTextureID, 0);

	// reset FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

	mesh.shader = loadShaderPass("shader.vert", "shader.frag");

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
	mesh.shader = loadShaderPass("shader.vert", "shader.frag");

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

void drawDepthBuffer(const Mesh& mesh)
{

}

void draw(const Mesh& mesh)
{
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glDrawBuffer(GL_FRONT);	

	//glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);

	glUseProgram(mesh.shader.ID);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindVertexArray(mesh.VertexArrayID);		

	glUniform3f(mesh.shader.LightID, LightPos.x, LightPos.y, LightPos.z);
	glUniformMatrix4fv(mesh.shader.MatrixID, 1, GL_FALSE, &mesh.MVP[0][0]);
	glUniformMatrix4fv(mesh.shader.ViewMatrixID, 1, GL_FALSE, &mesh.ViewMatrix[0][0]);
	glUniformMatrix4fv(mesh.shader.ModelMatrixID, 1, GL_FALSE, &mesh.ModelMatrix[0][0]);

	
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
	//glDisableVertexArrayAttrib(mesh.VertexArrayID, 1);
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
	//glDisableVertexArrayAttrib(mesh.VertexArrayID, 0);
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
	glFlush();
	
}


void destroy(const Mesh& mesh)
{
	if (mesh.vertbId > 0) glDeleteBuffers(1, &mesh.vertbId);
	if (mesh.uvbId > 0) glDeleteBuffers(1, &mesh.uvbId);
	if (mesh.normbId > 0) glDeleteBuffers(1, &mesh.normbId);
	if (mesh.shader.ID > 0) glDeleteProgram(mesh.shader.ID);
	if (mesh.VertexArrayID > 0)glDeleteVertexArrays(1, &mesh.VertexArrayID);
	if (mesh.Texture > 0) glDeleteTextures(1, &mesh.Texture);
	//if (shader.VertexArrayID > 0)glDeleteVertexArrays(1, &shader.VertexArrayID);
}


Mesh cube;
Mesh obj;

void render(void)
{
	//glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
	// set up render target

	//// Clear the screen	  
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	computeMatricesFromInputs();
	glm::mat4 ProjectionMatrix = getProjectionMatrix();

	cube.ViewMatrix = getViewMatrix();
	cube.ModelMatrix = glm::mat4(1.0);
	cube.ModelMatrix = translate(cube.ModelMatrix, glm::vec3(0, 0, 2));
	cube.MVP = ProjectionMatrix * cube.ViewMatrix * cube.ModelMatrix;
	draw(cube);
	drawDepthBuffer(cube);

	obj.ViewMatrix = getViewMatrix();
	obj.ModelMatrix = glm::mat4(1.0);
	obj.ModelMatrix = translate(obj.ModelMatrix, glm::vec3(0, 0, 0));
	obj.MVP = ProjectionMatrix * obj.ViewMatrix * obj.ModelMatrix;
	draw(obj);
	drawDepthBuffer(obj);

	// Draw to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw(obj);
	draw(cube);

	//// Draw to screen
	//glBindFramebuffer(GL_FRAMEBUFFER, DepthFrameBufferID);
	//glEnable(GL_DEPTH_TEST);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//draw(obj);
	//draw(cube);


	glfwSwapBuffers(window);
	glfwPollEvents();
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
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hidden Silhouette Shader", NULL, NULL);
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
	if (glewInit() != GLEW_OK) 
	{
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

	initDepthTexture();

	// Init meshes
	initObj(obj);
	initCube(cube);

	do render();
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

