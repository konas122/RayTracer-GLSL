#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "vao.h"
#include "shader.h"
#include "texture.h"


// Texture2D diffuseMap;
// Texture2D specularMap;
Texture2D envMap;
ShaderProgram shaderProgram;
VertexArrayObject vertexArrayObject;

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}


bool createScene() {
	float vertices[] = {
		 1.0f,  1.0f, 0.0f,  // top right
		 1.0f, -1.0f, 0.0f,  // bottom right
		-1.0f, -1.0f, 0.0f,  // bottom left
		-1.0f,  1.0f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	if (!vertexArrayObject.Create(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]))) {
		return false;
	}
	
	if (!envMap.Create("assets/photo_studio_01_1k.hdr")) {
		return false;
	}

	if (!shaderProgram.Create("shaders/PathTraceVS.glsl", "shaders/PathTracePS.glsl")) {
		return false;
	}

	return true;
}


void renderScene() {
	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shaderProgram.Bind();
	shaderProgram.SetUniform1i("envMap", 0);
	shaderProgram.SetUniform2i("screenSize", SCR_WIDTH, SCR_HEIGHT);

	vertexArrayObject.Bind();

	envMap.Bind(0);

	vertexArrayObject.Draw(GL_TRIANGLES, 6);
}


void destroyScene() {
	vertexArrayObject.Destroy();
	shaderProgram.Destroy();
}


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "RayTracing", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	if (!createScene()) {
		std::cout << "Failed to init Scene" << std::endl;
		return -1;
	}

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		renderScene();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	destroyScene();
	glfwTerminate();

	return 0;
}
