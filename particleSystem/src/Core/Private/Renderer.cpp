#include "Public/Renderer.h"
#include "Public/Layer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>

void Renderer::InitializeRenderer()
{
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//GLFWwindow* window = glfwCreateWindow(16 * 40 * 2, 9 * 40 * 2, "ParticleSystem", NULL, NULL);
	window = glfwCreateWindow(900, 900, "ParticleSystem", NULL, NULL);
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

	//IMGUI initialization steps
	const char* glsl_version = "#version 330 core";

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	//alpha blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//call every layer list's init here
	for (int i = 0; i < layerList.size(); i++) {
		layerList[i]->OnInit();
	}
}

void Renderer::StartRenderer()
{
	while (!glfwWindowShouldClose(window) && !rendererShouldStop) {
		if (prevFrameTime == -1.0f) {//if not initialized
			prevFrameTime = (float)glfwGetTime() - 0.000000005f; // make it a very small value

		}
		//clear render
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//compute the delta time and elapsed time
		float time = (float)glfwGetTime();
		deltaTime = time - prevFrameTime;
		prevFrameTime = time;

		elapsedTime += deltaTime;

		//call every layer's on tick here
		for (int i = 0; i < layerList.size(); i++) {
			layerList[i]->OnTick(deltaTime);
		}

		//error check
		while (GLenum enumout = glGetError()) {
			std::cout << "ERROR RENDER: " << enumout << std::endl;
		}

		//imgui render here
		{
			for (int i = 0; i < layerList.size(); i++) {
				layerList[i]->OnImGuiRender();
			}
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

	FinishRenderer();
}

void Renderer::FinishRenderer()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	for (int i = 0; i < layerList.size(); i++) {
		layerList[i]->OnFinish();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}

void Renderer::AddLayer(Layer* layer)
{
	layerList.push_back(layer);
}
