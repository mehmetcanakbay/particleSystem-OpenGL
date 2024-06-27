#pragma once
#include "Public/Layer.h"
#include "Public/ParticleSystem.h"
#include "Public/Shader.h"

class ParticleLayer : public Layer {
public:
	ParticleLayer();
	virtual void OnInit() override;
	virtual void OnTick(float deltaTime) override;
	virtual void OnImGuiRender() override;
	virtual void OnFinish() override;
private:
	ParticleSystem particleSystem;
	ParticleSystemRenderer particleRenderer;
	Shader particleShader;
};