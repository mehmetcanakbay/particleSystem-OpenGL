#include "Public/ParticleLayer.h"

ParticleLayer::ParticleLayer()
{
}

void ParticleLayer::OnInit()
{
	particleSystem.Initialize(1000000, 0.005f);
	particleRenderer.Initialize(&particleSystem);
	particleRenderer.Bind();
	
	//shader creation
	particleShader.Initialize("resources/shaders/ParticleShader.shader");
	particleShader.Bind();
}

void ParticleLayer::OnTick(float deltaTime)
{
	particleRenderer.Render(deltaTime);
}

void ParticleLayer::OnImGuiRender()
{
}

void ParticleLayer::OnFinish()
{
	particleRenderer.Unbind();
}
