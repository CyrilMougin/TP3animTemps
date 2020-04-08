// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
// #include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <opengl_utilities.h>

#include "learn_opengl_common.h"

// Properties
GLuint screenWidth = 800, screenHeight = 600;

#if 0
// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
#endif

void Do_Movement();
GLuint generateMultiSampleTexture(GLuint samples);

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// New globals for seperate loop function
GLuint framebuffer;
// GLFWwindow* window;
GLuint cubeVAO;
Shader *shader_ptr = NULL;

// The MAIN function, from here we start our application and run our Game loop
GRAPHICS_PARAMETER *main_from_tutorial_with_wgl(HWND hdlg, HINSTANCE hinstance,
	std::string *message_string_ptr)
{

#if 0
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif

	int gl_errors_found_count = 0;
	std::string gl_error_message;
	bool error_found = false;
	GRAPHICS_PARAMETER *graphics_parameter_ptr = new GRAPHICS_PARAMETER();
	windows_camera_wrapper_basic::CAMERA_WRAPPER *free_camera_wrapper_ptr = NULL;
	opengl_camera::CAMERA *camera_ptr = NULL;
	vector<Mesh> *meshes_vector_ptr = NULL;

	HDC hdc = NULL;
	std::string local_message_string;
	RECT client_rect;
	int width = 0;
	int height = 0;

	graphics_parameter_ptr->set_hwnd(hdlg);

	graphics_parameter_ptr->set_hrc(init_opengl(hdlg, &hdc, hinstance, 3, 3,
		NULL,
		&local_message_string));

	if (graphics_parameter_ptr->get_hrc() == NULL)
	{
		error_found = true;

		if (message_string_ptr != NULL)
		{
			*message_string_ptr += "setup_splash_screen()
				";
				*message_string_ptr += "   HRC is NULL.
				";
				*message_string_ptr += local_message_string.c_str();
			local_message_string.erase();
		}
	}
	else
	{
		graphics_parameter_ptr->set_hdc(hdc);

		if (graphics_parameter_ptr->get_hrc() == NULL)
		{
			error_found = true;

			if (message_string_ptr != NULL)
			{
				*message_string_ptr += "setup_splash_screen()
					";
					*message_string_ptr += "   OpenGL failed to open.
					";
					*message_string_ptr += local_message_string.c_str();
				local_message_string.erase();
			}
		}
		else
		{
			GetClientRect(hdlg, &client_rect);
			width = client_rect.right - client_rect.left;
			height = client_rect.bottom - client_rect.top;

			graphics_parameter_ptr->set_client_rect(&client_rect);

			glViewport(0, 0, width, height);

			initialize_cameras(graphics_parameter_ptr, width, height, VK_TAB, VK_TAB, 0.001);
		}
	}

	screenWidth = width;
	screenHeight = height;

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup OpenGL options
	glEnable(GL_MULTISAMPLE); // Enabled by default on some drivers, but not all so always enable to make sure
	glEnable(GL_DEPTH_TEST);

	// Setup and compile our shaders
   // Shader shader("advanced.shv", "advanced.shf");
   // shader_ptr = &shader;
	shader_ptr = new Shader("advanced.shv", "advanced.shf");

#pragma region "object_initialization"
	// Set the object data (buffers, vertex attributes)
	GLfloat cubeVertices[] = {
		// Positions       
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};
	// Setup cube VAO
	GLuint cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
#pragma endregion


	// Framebuffers
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Create a multisampled color attachment texture
	GLuint textureColorBufferMultiSampled = generateMultiSampleTexture(4);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
	// Create a renderbuffer object for depth and stencil attachments
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return graphics_parameter_ptr;
}

void loop_from_tutorial_with_wgl(HWND hdlg, GRAPHICS_PARAMETER *graphics_parameter_ptr)
{
#if 0
	// Set frame time
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
#endif

	// Check and call events
	// glfwPollEvents();
	// Do_Movement();      

	report_opengl_error_in_popup(hdlg, "After glfwPollEvents()");

	// 1. Draw scene as normal in multisampled buffers
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	report_opengl_error_in_popup(hdlg, "After glClear()");

	// Set transformation matrices		
	shader_ptr->Use();

	report_opengl_error_in_popup(hdlg, "After shader_ptr->Use()");

	glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 1000.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader_ptr->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader_ptr->Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(shader_ptr->Program, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4()));

	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	report_opengl_error_in_popup(hdlg, "After glDrawArrays()");

	glBindVertexArray(0);

	// 2. Now blit multisampled buffer(s) to default framebuffers
	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	report_opengl_error_in_popup(hdlg, "After glBindFramebuffer(GL_READ_FRAMEBUFFER) and glBindFramebuffer(GL_DRAW_FRAMEBUFFER)");

	glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	report_opengl_error_in_popup(hdlg, "After glBlitFramebuffer()");

	// Swap the buffers
	//  glfwSwapBuffers(window);
	SwapBuffers(graphics_parameter_ptr->get_hdc());

	report_opengl_error_in_popup(hdlg, "At end of loop, after SwapBuffers()");
}

GLuint generateMultiSampleTexture(GLuint samples)
{
	GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB8, screenWidth, screenHeight, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	return texture;
}