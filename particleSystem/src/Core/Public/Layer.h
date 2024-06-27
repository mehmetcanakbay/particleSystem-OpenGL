#pragma once

class Layer {
public:
	virtual void OnInit() {};
	virtual void OnTick(float deltaTime) {};

	virtual void OnFinish() {};
	virtual void OnImGuiRender() {};
};