#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "ChessFlow/Shader.h"
#include "ChessFlow/Texture.h"

namespace ChessFlow {
	
	class Square {

	public:
		Square();

	public:
		static float vertices[4 * 5];
		static unsigned int indices[2 * 3];
		static glm::vec3 lightColor, darkColor;

		static Shader squareShader;

		static unsigned int vao, vbo, ebo;
		static glm::mat4 proj, view;
		glm::mat4 model;

		glm::vec3 position;
		glm::vec3* color;

		unsigned int id;
		bool useTexture;
		bool toInvertColor = false;
		static ChessFlow::Texture sampleTexture;
		ChessFlow::Texture* tex;

	public:
		static void init();
		void setPosition(glm::vec3 newPos);
		void draw();
	};
}