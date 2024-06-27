#pragma once
#include <string>

struct ShaderSourceHolder {
	std::string vertexSource;
	std::string fragSource;
};

class Shader {
private:
	unsigned int shader_id;
	ShaderSourceHolder source; //Cache this just in case.
	
	ShaderSourceHolder ParseShader(const std::string& shaderFilePath);

	unsigned int CompileShader(ShaderSourceHolder& source);
public:
	Shader(const std::string& shaderFilePath);
	Shader();
	~Shader();

	void Initialize(const std::string& shaderFilePath);

	void Bind();
	void Unbind();
};