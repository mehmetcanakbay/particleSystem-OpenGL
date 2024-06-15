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
	~Shader();

	void Bind();
	void Unbind();
};