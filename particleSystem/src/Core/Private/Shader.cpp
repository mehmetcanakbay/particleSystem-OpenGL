#include "Public/Shader.h"
#include <fstream>
#include <sstream>
#include "Public/Renderer.h"

ShaderSourceHolder Shader::ParseShader(const std::string& shaderFilePath)
{
	std::ifstream stream(shaderFilePath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << "\n";
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(ShaderSourceHolder& source)
{
	unsigned int vertex_id = glCreateShader(GL_VERTEX_SHADER); //these should be scoped since they'll get destroyed anyway
	const char* src = source.vertexSource.c_str();
	glShaderSource(vertex_id, 1, &src, nullptr);
	glCompileShader(vertex_id);

	unsigned int frag_id = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragsrc = source.fragSource.c_str();
	glShaderSource(frag_id, 1, &fragsrc, nullptr);
	glCompileShader(frag_id);

	unsigned int program = glCreateProgram();
	glAttachShader(program, vertex_id);
	glAttachShader(program, frag_id);
	glLinkProgram(program);
	glValidateProgram(program);
	glDeleteShader(vertex_id);
	glDeleteShader(frag_id);

	glUseProgram(0);

	return program;
}

Shader::Shader(const std::string& shaderFilePath)
{
	source = ParseShader(shaderFilePath);
	shader_id = CompileShader(source);
}

void Shader::Bind()
{
	glUseProgram(shader_id);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

Shader::~Shader() {
	glDeleteProgram(shader_id);
}
