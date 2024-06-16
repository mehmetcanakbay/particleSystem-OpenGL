#include "Public/Renderer.h"
#include "glm/gtc/matrix_transform.hpp"

#include <stdio.h>
#include <iostream>
#include <string>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Public/Vertex.h"
#include "Public/ParticleSystem.h"
#include "Public/Shader.h"

int main() {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//GLFWwindow* window = glfwCreateWindow(16 * 40 * 2, 9 * 40 * 2, "ParticleSystem", NULL, NULL);
	GLFWwindow* window = glfwCreateWindow(900, 900, "ParticleSystem", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	const char* glsl_version = "#version 330 core";

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ParticleSystem particleSystem(1000, 0.007f);
	ParticleSystemRenderer particleRenderer(&particleSystem);
	particleRenderer.Bind();

	//shader creation
	Shader particleShader("resources/shaders/ParticleShader.shader");
	particleShader.Bind();
	
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		particleSystem.UpdateParticlePositions();
		particleRenderer.Render();

		while (GLenum enumout = glGetError()) {
			std::cout <<"ERROR: " << enumout << std::endl;
		}

		{
			ImGui::Begin("Debug");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	particleRenderer.Unbind();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}