#include "Public/Renderer.h"

static void Render() {
    if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(16*40*2, 9*40*2, "ParticleSystem", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// const char* glsl_version = "#version 330 core";

	// IMGUI_CHECKVERSION();
	// ImGui::CreateContext();
	// ImGuiIO& io = ImGui::GetIO(); (void)io;
	// ImGui::StyleColorsDark();
	// ImGui_ImplGlfw_InitForOpenGL(window, true);
	// ImGui_ImplOpenGL3_Init(glsl_version);

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//while (!glfwWindowShouldClose(window)) {

	//}




	// ImGui_ImplOpenGL3_Shutdown();
	// ImGui_ImplGlfw_Shutdown();
	// ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}