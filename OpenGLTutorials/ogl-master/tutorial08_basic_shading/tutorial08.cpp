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

struct Model
{
	GLuint vertbId = 0;
	std::vector<glm::vec3> verts;
	GLuint normbId = 0;
	std::vector<glm::vec3> norms;
	GLuint uvbId = 0;
	std::vector<glm::vec2> uvs;

	GLuint ShaderID = 0;
	GLuint MatrixID = 0;
	GLuint ViewMatrixID = 0;
	GLuint ModelMatrixID = 0;
	//GLuint trans = -1;
	GLuint Texture = 0;
	GLuint TextureID = 0;
	GLuint LightID = 0;
	GLuint VertexArrayID = 0;

	int in_PositionLocation = 0;
	int in_UVLocation = 0;
	int in_NormalLocation = 0;
};

glm::vec3 LightPos = glm::vec3(4, 4, 4);

// Bind the shader
void bindShader(GLuint shaderID)
{
	glUseProgram(shaderID);
}

// Unbind the shader
void unbindShader()
{
	glUseProgram(0);
}


void initCube(Model& model)
{
	//model.verts = std::vector<glm::vec3>(24);
	model.norms = std::vector<glm::vec3>(36);	

	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float sx = 1.f;
	float sy = 1.f;
	float sz = 1.f;
	
	float edgeLenght = 1.f;
	float halfLenght = edgeLenght / 2;

	// FRONT facet
	model.verts.push_back(glm::vec3(-halfLenght, -halfLenght, halfLenght));
	model.verts.push_back(glm::vec3(halfLenght, -halfLenght, halfLenght));
	model.verts.push_back(glm::vec3(halfLenght, halfLenght, halfLenght));
	model.verts.push_back(glm::vec3(-halfLenght, halfLenght, halfLenght));
	model.verts.push_back(glm::vec3(halfLenght, halfLenght, halfLenght));
	model.verts.push_back(glm::vec3(-halfLenght, -halfLenght, halfLenght));
	for (int i = 0; i < 6; i++) model.norms[i] = glm::vec3(0, 0, 1);

	// RIGHT facet
	model.verts.push_back(glm::vec3(halfLenght, halfLenght, halfLenght));
	model.verts.push_back(glm::vec3(halfLenght, halfLenght, -halfLenght));
	model.verts.push_back(glm::vec3(halfLenght, -halfLenght, -halfLenght));
	model.verts.push_back(glm::vec3(halfLenght, halfLenght, halfLenght));
	model.verts.push_back(glm::vec3(halfLenght, -halfLenght, -halfLenght));
	model.verts.push_back(glm::vec3(halfLenght, -halfLenght, halfLenght));
	for (int i = 6; i < 12; i++) model.norms[i] = glm::vec3(1, 0, 0);

	// BACK facet
	model.verts.push_back(glm::vec3(-halfLenght, -halfLenght, -halfLenght));
	model.verts.push_back(glm::vec3(halfLenght, -halfLenght, -halfLenght));
	model.verts.push_back(glm::vec3(halfLenght, halfLenght, -halfLenght));
	model.verts.push_back(glm::vec3(-halfLenght, -halfLenght, -halfLenght));
	model.verts.push_back(glm::vec3(halfLenght, halfLenght, -halfLenght));
	model.verts.push_back(glm::vec3(-halfLenght, halfLenght, -halfLenght));
	for (int i = 12; i < 18; i++) model.norms[i] = glm::vec3(0, 0, 1);

	// LEFT facet
	model.verts.push_back(glm::vec3(-halfLenght, -halfLenght, -halfLenght));
	model.verts.push_back(glm::vec3(-halfLenght, -halfLenght, halfLenght));
	model.verts.push_back(glm::vec3(-halfLenght, halfLenght, halfLenght));
	model.verts.push_back(glm::vec3(-halfLenght, -halfLenght, -halfLenght));
	model.verts.push_back(glm::vec3(-halfLenght, halfLenght, halfLenght));
	model.verts.push_back(glm::vec3(-halfLenght, halfLenght, -halfLenght));
	for (int i = 18; i < 24; i++) model.norms[i] = glm::vec3(1, 0, 0);

	// TOP facet
	model.verts.push_back(glm::vec3(halfLenght, halfLenght, halfLenght));
	model.verts.push_back(glm::vec3(-halfLenght, halfLenght, halfLenght));
	model.verts.push_back(glm::vec3(halfLenght, halfLenght, -halfLenght));
	model.verts.push_back(glm::vec3(-halfLenght, halfLenght, halfLenght));
	model.verts.push_back(glm::vec3(halfLenght, halfLenght, -halfLenght));
	model.verts.push_back(glm::vec3(-halfLenght, halfLenght, -halfLenght));
	for (int i = 24; i < 30; i++) model.norms[i] = glm::vec3(0, 1, 0);

	// BOTTOM facet
	model.verts.push_back(glm::vec3(-halfLenght, -halfLenght, -halfLenght));
	model.verts.push_back(glm::vec3(halfLenght, -halfLenght, -halfLenght));
	model.verts.push_back(glm::vec3(-halfLenght, -halfLenght, halfLenght));
	model.verts.push_back(glm::vec3(halfLenght, -halfLenght, -halfLenght));
	model.verts.push_back(glm::vec3(-halfLenght, -halfLenght, halfLenght));
	model.verts.push_back(glm::vec3(halfLenght, -halfLenght, halfLenght));
	for (int i = 30; i < 36; i++) model.norms[i] = glm::vec3(0, 1, 0);

	glGenVertexArrays(1, &model.VertexArrayID);
	glGenBuffers(1, &model.vertbId);
	glGenBuffers(1, &model.normbId);	

	glBindVertexArray(model.VertexArrayID);

	// Get a handle for our "MVP" uniform
	model.ShaderID = LoadShaders("shader.vert", "shader.frag");
	model.MatrixID = glGetUniformLocation(model.ShaderID, "MVP");
	model.ViewMatrixID = glGetUniformLocation(model.ShaderID, "V");
	model.ModelMatrixID = glGetUniformLocation(model.ShaderID, "M");		
	model.LightID = glGetUniformLocation(model.ShaderID, "LightPosition_worldspace");

	// VERTS
	glBindBuffer(GL_ARRAY_BUFFER, model.vertbId);
	glBufferData(GL_ARRAY_BUFFER, model.verts.size() * sizeof(glm::vec3), model.verts.data(), GL_STATIC_DRAW);
	model.in_PositionLocation = glGetAttribLocation(model.ShaderID, "vertexPosition_modelspace");
	glEnableVertexAttribArray(model.in_PositionLocation);
	glVertexAttribPointer(model.in_PositionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up the vertex attributes pointer

	// NORMS
	glBindBuffer(GL_ARRAY_BUFFER, model.normbId);
	glBufferData(GL_ARRAY_BUFFER, model.norms.size() * sizeof(glm::vec3), model.norms.data(), GL_STATIC_DRAW);
	model.in_NormalLocation = glGetAttribLocation(model.ShaderID, "vertexNormal_modelspace");
	glEnableVertexAttribArray(model.in_NormalLocation);
	glVertexAttribPointer(model.in_NormalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
}


void initObj(Model& model)
{	
	loadOBJ("suzanne.obj", model.verts, model.uvs, model.norms);

	glGenVertexArrays(1, &model.VertexArrayID);
	glGenBuffers(1, &model.normbId);
	glGenBuffers(1, &model.vertbId);
	glGenBuffers(1, &model.uvbId);

	glBindVertexArray(model.VertexArrayID);

	// Get a handle for our "MVP" uniform
	model.ShaderID = LoadShaders("shader.vert", "shader.frag");
	model.MatrixID = glGetUniformLocation(model.ShaderID, "MVP");
	model.ViewMatrixID = glGetUniformLocation(model.ShaderID, "V");
	model.ModelMatrixID = glGetUniformLocation(model.ShaderID, "M");
	model.Texture = loadDDS("uvmap.DDS");
	model.TextureID = glGetUniformLocation(model.ShaderID, "myTextureSampler");
	model.LightID = glGetUniformLocation(model.ShaderID, "LightPosition_worldspace");

	glBindVertexArray(model.VertexArrayID);

	// VERTS
	glBindBuffer(GL_ARRAY_BUFFER, model.vertbId);
	glBufferData(GL_ARRAY_BUFFER, model.verts.size() * sizeof(glm::vec3), model.verts.data(), GL_STATIC_DRAW);
	model.in_PositionLocation = glGetAttribLocation(model.ShaderID, "vertexPosition_modelspace");	
	glBindBuffer(GL_ARRAY_BUFFER, model.vertbId);
	glBufferData(GL_ARRAY_BUFFER, model.verts.size() * sizeof(glm::vec3), model.verts.data(), GL_STATIC_DRAW);	

	// UVS
	glBindBuffer(GL_ARRAY_BUFFER, model.uvbId);
	glBufferData(GL_ARRAY_BUFFER, model.uvs.size() * sizeof(glm::vec3), model.uvs.data(), GL_STATIC_DRAW);
	model.in_UVLocation = glGetAttribLocation(model.ShaderID, "vertexUV");
	glEnableVertexAttribArray(model.in_UVLocation);
	glVertexAttribPointer(model.in_UVLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	

	// NORMS
	glBindBuffer(GL_ARRAY_BUFFER, model.normbId);
	glBufferData(GL_ARRAY_BUFFER, model.norms.size() * sizeof(glm::vec3), model.norms.data(), GL_STATIC_DRAW);
	model.in_NormalLocation = glGetAttribLocation(model.ShaderID, "vertexNormal_modelspace");
	glEnableVertexAttribArray(model.in_NormalLocation);
	glVertexAttribPointer(model.in_NormalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

}

void drawObj(const Model& model)	
{
	bindShader(model.ShaderID);

	computeMatricesFromInputs();
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	//glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 5.0f));

	glUniform3f(model.LightID, LightPos.x, LightPos.y, LightPos.z);


	glUniformMatrix4fv(model.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(model.ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(model.ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	
	//glUniformMatrix4fv(model.trans, 1, GL_FALSE, &translation[0][0]);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model.Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(model.TextureID, 0);

	glBindVertexArray(model.VertexArrayID);




	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, model.vertbId);
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
	glBindBuffer(GL_ARRAY_BUFFER, model.uvbId);
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
	glBindBuffer(GL_ARRAY_BUFFER, model.normbId);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	
	glDrawArrays(GL_TRIANGLES, 0, model.verts.size());


	glBindVertexArray(0);
	unbindShader();
}


void drawCube(Model model)
{
	bindShader(model.ShaderID);

	computeMatricesFromInputs();
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	ModelMatrix = translate(ModelMatrix, glm::vec3(0, 0, 2));

	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;		

	glUniform3f(model.LightID, LightPos.x, LightPos.y, LightPos.z);
	glUniformMatrix4fv(model.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(model.ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(model.ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);		

	glBindVertexArray(model.VertexArrayID);

	// 1rst attribute buffer : vertices	
	glEnableVertexAttribArray(model.in_PositionLocation);
	glBindBuffer(GL_ARRAY_BUFFER, model.vertbId);
	glVertexAttribPointer(
		model.in_PositionLocation,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		0            // array buffer offset
	);


	// 2rd attribute buffer : normals
	glEnableVertexAttribArray(model.in_NormalLocation);
	glBindBuffer(GL_ARRAY_BUFFER, model.normbId);
	glVertexAttribPointer(
		model.in_NormalLocation,          // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		0                          // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, model.verts.size());

	glBindVertexArray(0);
	unbindShader();
}


void destroy(const Model& model)
{
	if(model.vertbId > 0) glDeleteBuffers(1, &model.vertbId);
	if (model.uvbId > 0) glDeleteBuffers(1, &model.uvbId);
	if (model.normbId > 0) glDeleteBuffers(1, &model.normbId);
	if (model.ShaderID > 0) glDeleteProgram(model.ShaderID);
	if (model.Texture > 0) glDeleteTextures(1, &model.Texture);	
	if (model.VertexArrayID > 0)glDeleteVertexArrays(1, &model.VertexArrayID);
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
	window = glfwCreateWindow(1024, 768, "Tutorial 08 - Basic Shading", NULL, NULL);
	if (window == NULL) {
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
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);	
	glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);


	// INIT OBJ
	// Read our .obj file
	// Load the texture
	Model obj;
	initObj(obj);


	// INIT CUBE
	Model cube;
	initCube(cube);


	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);


		/*
			GLuint vertb,
			GLuint normb,
			GLuint uvb,
			GLuint trans,
			GLuint TextureID,
		*/
		drawObj(obj);
		drawCube(cube);


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


	
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	destroy(obj);
	destroy(cube);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

