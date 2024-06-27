#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <vector>

class Layer;

class Renderer {
private:

public:
	void InitializeRenderer();
	void StartRenderer();

	void FinishRenderer();

	void AddLayer(Layer* layer);

	inline float ReturnDeltaTime() const { return deltaTime; }
	inline float ReturnElapsedTime() const { return elapsedTime; }

	inline void StopRenderer() { rendererShouldStop = true; };

private:
	std::vector<Layer*> layerList;

	GLFWwindow* window;
	float prevFrameTime = -1.0;
	float deltaTime = 0;
	float elapsedTime = 0;

	bool rendererShouldStop = false;
};