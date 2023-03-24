#include "ChessFlow/Shader.h"

#include <glad/glad.h>
#include <fstream>
#include <plog/Log.h>
#include <glm/gtc/type_ptr.hpp>

#include "ChessFlow/HandleError.h"

namespace ChessFlow {
	const std::string Shader::sampleVertex = "\n#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main() {\n"
		"	gl_Position = vec4(aPos, 1.0);\n"
		"}\n";
	const std::string Shader::sampleFragment = "\n#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"	FragColor = vec4(0.0, 0.0, 1.0, 1.0);\n"
		"}\n";


	Shader::Shader() = default;

	Shader::~Shader() {
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(shaderProgram);
	}

	std::string Shader::read(const std::string& path) {
		PLOGV << ("Reading Shader at " + path);
		std::string out = "";
		std::ifstream inStream(path);
		if(inStream.is_open()) {
			std::string line;
			while(std::getline(inStream, line)) {
				out += (line + "\n");
			}
			inStream.close();
		}
		else PLOGE << "Unable to open shader";
		return out;
	}

	unsigned int Shader::compile(const std::string& source, ShaderType type) {
		unsigned int out = glCreateShader(type == ShaderType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
		const char* src = source.c_str();
		PLOGV << "Compiling " << (type == ShaderType::Vertex ? "Vertex" : "Fragment") << " Shader... ID = " << out;
		//PLOGV << src;

		GLCall(glShaderSource(out, 1, &src, NULL));
		GLCall(glCompileShader(out));

		int success;
		GLCall(glGetShaderiv(out, GL_COMPILE_STATUS, &success));
		if(!success) {
			char infoLog[512];
			GLCall(glGetShaderInfoLog(out, 512, NULL, infoLog));
			PLOGE << "SHADER::" << (type == ShaderType::Vertex ? "VERTEX" : "FRAGMENT") << "::COMPILATION FAILED\n" << infoLog;
		}
		else PLOGV << "Compilation Success";

		return out;
	}

	void Shader::loadFromFile(const std::string& _vShader, const std::string& _fShader) {
		std::string vShader = read(_vShader), fShader = read(_fShader);
		loadFromMemory(vShader, fShader);
	}

	void Shader::loadFromMemory(const std::string& vShader, const std::string& fShader) {
		vertexShader = compile(vShader, ShaderType::Vertex);
		fragmentShader = compile(fShader, ShaderType::Fragment);
		shaderProgram = glCreateProgram();
		GLCall(glAttachShader(shaderProgram, vertexShader));
		GLCall(glAttachShader(shaderProgram, fragmentShader));
		PLOGV << "Linking shader " << shaderProgram;
		GLCall(glLinkProgram(shaderProgram));
	}

	void Shader::use() {
		GLCall(glUseProgram(shaderProgram));
	}

	void Shader::setBool(std::string name, bool val) {
		GLCall(glUseProgram(shaderProgram));
		int id = glGetUniformLocation(shaderProgram, name.c_str());
		if(id == -1)
			PLOGE << "Uniform \"" << name << "\" not found";
		GLCall(glUniform1i(id, val));
		GLCall(glUseProgram(NULL));
	}

	void Shader::setVec2(std::string name, glm::vec2 vec) {
		GLCall(glUseProgram(shaderProgram));
		int id = glGetUniformLocation(shaderProgram, name.c_str());
		if(id == -1)
			PLOGE << "Uniform \"" << name << "\" not found";
		auto k = glm::value_ptr(vec);
		GLCall(glUniform2fv(id, 1, glm::value_ptr(vec)));
		GLCall(glUseProgram(NULL));
	}

	void Shader::setVec3(std::string name, glm::vec3 vec) {
		GLCall(glUseProgram(shaderProgram));
		int id = glGetUniformLocation(shaderProgram, name.c_str());
		if(id == -1)
			PLOGE << "Uniform \"" << name << "\" not found";
		GLCall(glUniform3fv(id, 1, glm::value_ptr(vec)));
		GLCall(glUseProgram(NULL));
	}

	void Shader::setVec4(std::string name, glm::vec4 vec) {
		GLCall(glUseProgram(shaderProgram));
		int id = glGetUniformLocation(shaderProgram, name.c_str());
		if(id == -1)
			PLOGE << "Uniform \"" << name << "\" not found";
		GLCall(glUniform4fv(id, 1, glm::value_ptr(vec)));
		GLCall(glUseProgram(NULL));
	}

	void Shader::setMatrix4(std::string name, glm::mat4 mat)
	{
		GLCall(glUseProgram(shaderProgram));
		int id = glGetUniformLocation(shaderProgram, name.c_str());
		if(id == -1)
			PLOGE << "Uniform \"" << name << "\" not found";
		GLCall(glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(mat)));
		GLCall(glUseProgram(NULL));
	}

	void Shader::setTexture(ChessFlow::Texture tex) {
		glBindTexture(GL_TEXTURE_2D, tex.textureId);
	}
}