#include "Public/Renderer.h"
#include "glm/gtc/matrix_transform.hpp"

#include <stdio.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>


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

	ParticleSystem particleSystem(3000000, 0.005f);
	//ParticleSystem particleSystem(10000, 0.005f);
	ParticleSystemRenderer particleRenderer(&particleSystem);
	particleRenderer.Bind();

	//shader creation
	Shader particleShader("resources/shaders/ParticleShader.shader");
	particleShader.Bind();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float prevFrameTime = -1.0f;
	float elapsedTime = 0.0f;
	float deltaTime;

	while (!glfwWindowShouldClose(window)) {
		if (prevFrameTime == -1.0f) {//if not initialized
			prevFrameTime = (float)glfwGetTime()-0.000000005f; // make it a very small value

		}
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();



		float time = (float)glfwGetTime();
		deltaTime = time - prevFrameTime;
		prevFrameTime = time;

		elapsedTime += deltaTime;
		//particleSystem.Tick(deltaTime);
		//particleSystem.UpdateParticlePositions(deltaTime);
		particleRenderer.Render(&deltaTime);
		while (!particleRenderer.Render(&deltaTime));

		while (GLenum enumout = glGetError()) {
			std::cout <<"ERROR RENDER: " << enumout << std::endl;
		}

		{
			ImGui::Begin("Debug");
			ImGui::Text("FPS: %.3f, deltaTime: %.6f", (1 / deltaTime), deltaTime);
			ImGui::Text("Elapsed Time: %.3f", elapsedTime);
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