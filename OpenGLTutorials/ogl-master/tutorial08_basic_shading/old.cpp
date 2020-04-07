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


void initCube(
	GLuint& vertbId,
	std::vector<glm::vec3>& verts,
	GLuint& normbId,
	std::vector<glm::vec3>& norms)
{
	verts[0] = glm::vec3(-0.5, 0, 0.5);
	verts[1] = glm::vec3(-0.5, 1.0, 0.5);
	verts[2] = glm::vec3(0.5, 1.0, 0.5);
	verts[3] = glm::vec3(0.5, 0, 0.5);

	verts[4] = glm::vec3(-0.5, 0, 0.5);
	verts[5] = glm::vec3(0.5, 0, 0.5);
	verts[6] = glm::vec3(0.5, 0, -0.5);
	verts[7] = glm::vec3(-0.5, 0, -0.5);

	verts[8] = glm::vec3(-0.5, 0, 0.5);
	verts[9] = glm::vec3(-0.5, 0, -0.5);
	verts[10] = glm::vec3(-0.5, 1.0, -0.5);
	verts[11] = glm::vec3(-0.5, 1.0, 0.5);

	verts[12] = glm::vec3(-0.5, 1.0, 0.5);
	verts[13] = glm::vec3(0.5, 1.0, 0.5);
	verts[14] = glm::vec3(0.5, 1.0, -0.5);
	verts[15] = glm::vec3(-0.5, 1.0, -0.5);

	verts[16] = glm::vec3(-0.5, 0, -0.5);
	verts[17] = glm::vec3(0.5, 0, -0.5);
	verts[18] = glm::vec3(0.5, 1.0, -0.5);
	verts[19] = glm::vec3(-0.5, 1.0, -0.5);

	verts[20] = glm::vec3(0.5, 0, 0.5);
	verts[21] = glm::vec3(0.5, 0, -0.5);
	verts[22] = glm::vec3(0.5, 1.0, -0.5);
	verts[23] = glm::vec3(0.5, 1.0, 0.5);

	for (int i = 0; i < 4; i++)
		norms[i] = glm::vec3(0, 0, 1);//front

	for (int i = 4; i < 8; i++)
		norms[i] = glm::vec3(0, -1, 0);//left

	for (int i = 8; i < 12; i++)
		norms[i] = glm::vec3(-1, 0, 0);

	for (int i = 12; i < 16; i++)
		norms[i] = glm::vec3(0, 1, 0);

	for (int i = 16; i < 20; i++)
		norms[i] = glm::vec3(0, 0, -1);

	for (int i = 20; i < 24; i++)
		norms[i] = glm::vec3(1, 0, 0);

	glGenBuffers(1, &vertbId);
	glBindBuffer(GL_ARRAY_BUFFER, vertbId);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normbId);
	glBindBuffer(GL_ARRAY_BUFFER, normbId);
	glBufferData(GL_ARRAY_BUFFER, norms.size() * sizeof(glm::vec3), &norms[0], GL_STATIC_DRAW);

}


void initObj(
	GLuint& vertbId,
	std::vector<glm::vec3>& verts,
	GLuint& normbId,
	std::vector<glm::vec3>& norms,
	GLuint& uvbId,
	std::vector<glm::vec2>& uvs)
{	
	loadOBJ("suzanne.obj", verts, uvs, norms);

	glGenBuffers(1, &vertbId);
	glBindBuffer(GL_ARRAY_BUFFER, vertbId);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbId);
	glBindBuffer(GL_ARRAY_BUFFER, uvbId);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);


	glGenBuffers(1, &normbId);
	glBindBuffer(GL_ARRAY_BUFFER, normbId);
	glBufferData(GL_ARRAY_BUFFER, norms.size() * sizeof(glm::vec3), &norms[0], GL_STATIC_DRAW);
}

void drawObj(
	GLuint vertb,
	GLuint normb,
	GLuint uvb,
	GLuint trans,
	GLuint Texture,
	GLuint TextureID,
	int numv)
{
	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertb);
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
	glBindBuffer(GL_ARRAY_BUFFER, uvb);
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
	glBindBuffer(GL_ARRAY_BUFFER, normb);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, numv);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}


void drawCube(
	GLuint vertb,
	GLuint normb,
	GLuint trans,
	GLuint TextureID,
	int numv)
{

	// 1rst attribute buffer : vertices
	//glm::mat4 translation = glm::translate(translation, glm::vec3(0.0f, 0.0f, 5.0));
	//glUniformMatrix4fv(trans, 1, GL_FALSE, &translation[0][0]);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertb);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	 //2nd attribute buffer : UVs
	//glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, uvb);
	//glVertexAttribPointer(
	//	1,                                // attribute
	//	2,                                // size
	//	GL_FLOAT,                         // type
	//	GL_FALSE,                         // normalized?
	//	0,                                // stride
	//	(void*)0                          // array buffer offset
	//);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normb);
	glVertexAttribPointer(
		1,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the triangles !

	glDrawArrays(GL_TRIANGLES, 0, numv);

			glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
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

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("StandardShading.vert", "StandardShading.frag");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
	GLuint trans = glGetUniformLocation(programID, "trans");
	GLuint Texture = loadDDS("uvmap.DDS");
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");


	// INIT OBJ
	// Read our .obj file
	// Load the texture
	std::vector<glm::vec3> objVertices;
	std::vector<glm::vec2> objUvs;
	std::vector<glm::vec3> objNormals;
	GLuint objVertexBuffer;
	GLuint objNormalbuffer;
	GLuint objUvbuffer;
	initObj(objVertexBuffer, objVertices, objNormalbuffer, objNormals, objUvbuffer, objUvs);


	// INIT CUBE
	std::vector<glm::vec3> cubeVerts(24);
	std::vector<glm::vec3> cubeNormals(24);
	GLuint cubeVertexBuffer;
	GLuint cubeNormalbuffer;
	initCube(cubeVertexBuffer, cubeVerts, cubeNormalbuffer, cubeNormals);


	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glm::mat4 translation = glm::mat4(1.0f);

		translation = glm::translate(translation, glm::vec3(0.0f, 0.0f, 0.0f));


		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniformMatrix4fv(trans, 1, GL_FALSE, &translation[0][0]);

		glm::vec3 lightPos = glm::vec3(4, 4, 4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		/*
			GLuint vertb,
			GLuint normb,
			GLuint uvb,
			GLuint trans,
			GLuint TextureID,
		*/
		drawObj(
			objVertexBuffer,
			objNormalbuffer,
			objUvbuffer,
			trans,
			Texture,
			TextureID,
			objVertices.size());

		drawCube(
			cubeVertexBuffer,
			cubeNormalbuffer,
			trans,
			Texture,
			cubeVerts.size());


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &objVertexBuffer);
	glDeleteBuffers(1, &objUvbuffer);
	glDeleteBuffers(1, &objNormalbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

