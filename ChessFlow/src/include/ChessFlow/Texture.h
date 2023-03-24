#pragma once

#include <stb_image.h>
#include <string>

namespace ChessFlow {
	class Texture {
	
	public:
		Texture();
		//~Texture();
		void loadFromMemory(unsigned char* data, int width, int height, int noOfChannels);
		void loadFromFile(const std::string& filepath);

	public:
		unsigned int textureId;
		unsigned char* imageData;

	};
}