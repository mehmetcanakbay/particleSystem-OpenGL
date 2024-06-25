#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

class Layer;

class Renderer {

private:
	void InitializeRenderer();

	void StartRenderer();

	void AddLayer(Layer* layer);
};