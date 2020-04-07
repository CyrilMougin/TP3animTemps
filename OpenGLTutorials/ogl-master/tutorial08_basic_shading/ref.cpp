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
	GLuint vertbId;
	std::vector<glm::vec3> verts;
	GLuint normbId;
	std::vector<glm::vec3> norms;
	GLuint uvbId;
	std::vector<glm::vec2> uvs;

	GLuint ShaderID;
	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;
	GLuint trans;
	GLuint Texture;
	GLuint TextureID;

};


void initCube(Model& mod)
{
	mod.verts = std::vector<glm::vec3>(24);
	mod.norms = std::vector<glm::vec3>(24);

	mod.verts[0] = glm::vec3(-0.5, 0, 0.5);
	mod.verts[1] = glm::vec3(-0.5, 1.0, 0.5);
	mod.verts[2] = glm::vec3(0.5, 1.0, 0.5);
	mod.verts[3] = glm::vec3(0.5, 0, 0.5);

	mod.verts[4] = glm::vec3(-0.5, 0, 0.5);
	mod.verts[5] = glm::vec3(0.5, 0, 0.5);
	mod.verts[6] = glm::vec3(0.5, 0, -0.5);
	mod.verts[7] = glm::vec3(-0.5, 0, -0.5);

	mod.verts[8] = glm::vec3(-0.5, 0, 0.5);
	mod.verts[9] = glm::vec3(-0.5, 0, -0.5);
	mod.verts[10] = glm::vec3(-0.5, 1.0, -0.5);
	mod.verts[11] = glm::vec3(-0.5, 1.0, 0.5);

	mod.verts[12] = glm::vec3(-0.5, 1.0, 0.5);
	mod.verts[13] = glm::vec3(0.5, 1.0, 0.5);
	mod.verts[14] = glm::vec3(0.5, 1.0, -0.5);
	mod.verts[15] = glm::vec3(-0.5, 1.0, -0.5);

	mod.verts[16] = glm::vec3(-0.5, 0, -0.5);
	mod.verts[17] = glm::vec3(0.5, 0, -0.5);
	mod.verts[18] = glm::vec3(0.5, 1.0, -0.5);
	mod.verts[19] = glm::vec3(-0.5, 1.0, -0.5);

	mod.verts[20] = glm::vec3(0.5, 0, 0.5);
	mod.verts[21] = glm::vec3(0.5, 0, -0.5);
	mod.verts[22] = glm::vec3(0.5, 1.0, -0.5);
	mod.verts[23] = glm::vec3(0.5, 1.0, 0.5);

	for (int i = 0; i < 4; i++)
		mod.norms[i] = glm::vec3(0, 0, 1);//front

	for (int i = 4; i < 8; i++)
		mod.norms[i] = glm::vec3(0, -1, 0);//left

	for (int i = 8; i < 12; i++)
		mod.norms[i] = glm::vec3(-1, 0, 0);

	for (int i = 12; i < 16; i++)
		mod.norms[i] = glm::vec3(0, 1, 0);

	for (int i = 16; i < 20; i++)
		mod.norms[i] = glm::vec3(0, 0, -1);

	for (int i = 20; i < 24; i++)
		mod.norms[i] = glm::vec3(1, 0, 0);

	glGenBuffers(1, &mod.vertbId);
	glBindBuffer(GL_ARRAY_BUFFER, mod.vertbId);
	glBufferData(GL_ARRAY_BUFFER, mod.verts.size() * sizeof(glm::vec3), &mod.verts[0], GL_STATIC_DRAW);

	glGenBuffers(1, &mod.normbId);
	glBindBuffer(GL_ARRAY_BUFFER, mod.normbId);
	glBufferData(GL_ARRAY_BUFFER, mod.norms.size() * sizeof(glm::vec3), &mod.norms[0], GL_STATIC_DRAW);

	// Get a handle for our "MVP" uniform
	mod.ShaderID = LoadShaders("StandardShading.vert", "StandardShading.frag");
	mod.MatrixID = glGetUniformLocation(mod.ShaderID, "MVP");
	mod.ViewMatrixID = glGetUniformLocation(mod.ShaderID, "V");
	mod.ModelMatrixID = glGetUniformLocation(mod.ShaderID, "M");
	mod.trans = glGetUniformLocation(mod.ShaderID, "trans");
	mod.Texture = loadDDS("uvmap.DDS");
	mod.TextureID = glGetUniformLocation(mod.ShaderID, "myTextureSampler");

}


void initObj(Model& mod)
{
	loadOBJ("suzanne.obj", mod.verts, mod.uvs, mod.norms);

	glGenBuffers(1, &mod.vertbId);
	glBindBuffer(GL_ARRAY_BUFFER, mod.vertbId);
	glBufferData(GL_ARRAY_BUFFER, mod.verts.size() * sizeof(glm::vec3), &mod.verts[0], GL_STATIC_DRAW);

	glGenBuffers(1, &mod.uvbId);
	glBindBuffer(GL_ARRAY_BUFFER, mod.uvbId);
	glBufferData(GL_ARRAY_BUFFER, mod.uvs.size() * sizeof(glm::vec2), &mod.uvs[0], GL_STATIC_DRAW);


	glGenBuffers(1, &mod.normbId);
	glBindBuffer(GL_ARRAY_BUFFER, mod.normbId);
	glBufferData(GL_ARRAY_BUFFER, mod.norms.size() * sizeof(glm::vec3), &mod.norms[0], GL_STATIC_DRAW);

	// Get a handle for our "MVP" uniform
	mod.ShaderID = LoadShaders("StandardShading.vert", "StandardShading.frag");
	mod.MatrixID = glGetUniformLocation(mod.ShaderID, "MVP");
	mod.ViewMatrixID = glGetUniformLocation(mod.ShaderID, "V");
	mod.ModelMatrixID = glGetUniformLocation(mod.ShaderID, "M");
	mod.trans = glGetUniformLocation(mod.ShaderID, "trans");
	mod.Texture = loadDDS("uvmap.DDS");
	mod.TextureID = glGetUniformLocation(mod.ShaderID, "myTextureSampler");

}

void drawObj(const Model& mod)
{
	GLuint LightID = glGetUniformLocation(mod.ShaderID, "LightPosition_worldspace");

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mod.Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(mod.TextureID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mod.vertbId);
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
	glBindBuffer(GL_ARRAY_BUFFER, mod.uvbId);
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
	glBindBuffer(GL_ARRAY_BUFFER, mod.normbId);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, mod.verts.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}


void drawCube(Model mod)
{

	// 1rst attribute buffer : vertices
	glm::mat4 translation = glm::translate(translation, glm::vec3(0.0f, 0.0f, 5.0));
	glUniformMatrix4fv(mod.trans, 1, GL_FALSE, &translation[0][0]);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mod.vertbId);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		0            // array buffer offset
	);


	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mod.normbId);
	glVertexAttribPointer(
		1,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		0                          // array buffer offset
	);

	// Draw the triangles !

	glDrawArrays(GL_TRIANGLES, 0, mod.verts.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}


void destroy(Model mod)
{
	glDeleteBuffers(1, &mod.vertbId);
	glDeleteBuffers(1, &mod.uvbId);
	glDeleteBuffers(1, &mod.normbId);
	glDeleteProgram(mod.ShaderID);
	glDeleteTextures(1, &mod.Texture);
	//glDeleteVertexArrays(1, &mod.vert);
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
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);


	// INIT OBJ
	// Read our .obj file
	// Load the texture
	Model cube;
	initObj(cube);


	// INIT CUBE
	Model obj;
	initCube(obj);


	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);


		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glm::mat4 translation = glm::mat4(1.0f);


		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform

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

