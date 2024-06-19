#include <cstdlib>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "vao.h"
#include "header.h"
#include "shader.h"
#include "texture.h"
#include "camera.hpp"


Texture2D           envMap;
ShaderProgram       shaderProgram;
VertexArrayObject   vertexArrayObject;

ShaderProgram       finalProgram;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

int samples = 0;
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float vertices[] = {
    1.0f,  1.0f, 0.0f,  // top right
    1.0f, -1.0f, 0.0f,  // bottom right
    -1.0f, -1.0f, 0.0f, // bottom left
    -1.0f,  1.0f, 0.0f  // top left 
};

unsigned int indices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};


static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
        samples = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
        samples = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
        samples = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
        samples = 0;
    }
}


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        samples = 0;
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


bool createScene() {
	if (!vertexArrayObject.Create(vertices, sizeof(vertices), indices, sizeof(indices))) {
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
    auto randFloat = []() {
        return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    };
    float randWidth = static_cast<float>(randFloat() / SCR_WIDTH);
    float randHeight = static_cast<float>(randFloat() / SCR_HEIGHT);

    shaderProgram.Bind();
	shaderProgram.SetUniform1i("envMap", 0);
	shaderProgram.SetUniform2f("randSize", randWidth, randHeight);
    shaderProgram.SetUniform3f("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

    vertexArrayObject.Bind();

	envMap.Bind(0);

	vertexArrayObject.Draw(GL_TRIANGLES, 6);
    vertexArrayObject.Unbind();
}


void destroyScene() {
	vertexArrayObject.Destroy();
	shaderProgram.Destroy();
    finalProgram.Destroy();
}


bool initFinalProgram(unsigned int *FBO, unsigned int *TextureID, unsigned int *PrevScene) {
    glGenFramebuffers(1, FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, *FBO);

    glGenTextures(1, TextureID);
    glBindTexture(GL_TEXTURE_2D, *TextureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *TextureID, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenTextures(1, PrevScene);
    glBindTexture(GL_TEXTURE_2D, *PrevScene);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (!finalProgram.Create("shaders/PathTraceVS.glsl", "shaders/finalPS.glsl")) {
		return false;
	}

    return true;
}


void finalRender(unsigned int TextureID, unsigned int PrevScene) {
    finalProgram.Bind();
    finalProgram.SetUniform1i("sampleCount", std::max(1, ++samples));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureID);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, PrevScene);

    vertexArrayObject.Bind();
    vertexArrayObject.Draw(GL_TRIANGLES, 6);
    vertexArrayObject.Unbind();

    glBindTexture(GL_TEXTURE_2D, PrevScene);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, SCR_WIDTH, SCR_HEIGHT);
}


int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    glfwSetErrorCallback(glfw_error_callback);
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "RayTracing", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetCursorPosCallback(window, mouse_callback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	if (!createScene()) {
		std::cout << "Failed to init Scene" << std::endl;
		return -1;
	}

    unsigned int FBO, TextureID, PrevScene;
    if (!initFinalProgram(&FBO, &TextureID, &PrevScene)) {
		std::cout << "Failed to initialize FrameBuffer" << std::endl;
		return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!glfwWindowShouldClose(window)) {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		processInput(window);

        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderScene();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        finalRender(TextureID, PrevScene);

        {
            ImGui::Begin("Tool Bar");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::Text("Samples: %d", samples);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
		glfwPollEvents();
	}

	destroyScene();
	glfwTerminate();

	return 0;
}
