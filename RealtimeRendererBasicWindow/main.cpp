#include <glad/glad.h> // note glad generated with GLAD1
#include <GLFW/glfw3.h>

#include <iostream>

void error_callback(int error, const char* description)
{
	std::cerr << "Error: " << description << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main()
{
	// initialize glfw
	if (!glfwInit())
	{
		std::cout << "GLFW: Failed to initilize" << std::endl;
	}

	// setting error callbacks
	glfwSetErrorCallback(error_callback);


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// creating window and context
	GLFWwindow* window = glfwCreateWindow(640, 480, "Viewport", NULL, NULL);
	if (!window)
	{
		std::cout << "Window creation failed" << std::endl;
	}

	// receiving inputs
	glfwSetKeyCallback(window, key_callback);

	// make context current
	glfwMakeContextCurrent(window);
	
	//gladLoadGL(glfwGetProcAddress);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD: Failed to initilize" << std::endl;
	}

	glfwSwapInterval(1);

	// rendering a triangle
	float points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	// GPU: vertex buffer object -> vertex points, texture-coordinates, vertex normals
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

	// GPU: vertex array object -> keep track of different vbo's
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// vertex shader will run for each vertex, 3 points will have 3 vertex shaders in our GPU running
	// gl_Position is reserved name - x,y,z,perspective
	const char* vertex_shader =
		"#version 400\n"
		"in vec3 vp;"
		"void main() {"
		"	gl_Position = vec4(vp, 1.0);"
		"}";

	// fragment shader - once per pixel-sized fragment that the surface of the triangle covers
	// frag_color - r,g,b,alpha
	const char* fragment_shader = 
		"#version 400\n"
		"out vec4 frag_colour;"
		"void main() {"
		"	frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
		"}";

	// load shaders and compile
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	// combine shaders into a single executable GPU shader programme
	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	// checking window close flag
	while (!glfwWindowShouldClose(window))
	{
		// rendering with OpenGL
		//int width, height;
		//glfwGetFramebufferSize(window, &width, &height);
		//glViewport(0, 0, width, height);

		// wipe the drawing surface
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_programme);
		glBindVertexArray(vao);

		// draw points 0-3 from the currently bound vao with current in-use shader
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// swapping buffers
		glfwSwapBuffers(window);

		// processing events
		glfwPollEvents();
	}

	// destruction
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}