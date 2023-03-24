#include "ChessFlow/Texture.h"

#include <glad/glad.h>
#include <plog/Log.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace ChessFlow {
    Texture::Texture() = default;

    void Texture::loadFromMemory(unsigned char* data, int width, int height, int noOfChannels) {
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if(data) {
            glTexImage2D(GL_TEXTURE_2D, 0, noOfChannels == 3 ? GL_RGB : GL_RGBA, width, height, 0, noOfChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
            //PLOGD << width << " " << height << " " << nrChanels;
            glGenerateMipmap(GL_TEXTURE_2D);
            PLOGV << "Image loaded at " << textureId;
        }
        else PLOGE << "Invalid data at texture ID " << textureId;
    }

    void Texture::loadFromFile(const std::string& filePath) {
        int width, height, nrChanels;
        PLOGV << "loading image at \"" << filePath << "\"";
        imageData = stbi_load(filePath.c_str(), &width, &height, &nrChanels, 0);
        if(imageData) {
            loadFromMemory(imageData, width, height, nrChanels);
        }
        else PLOGE << "Failed to load image at \"" << filePath << "\"";
        stbi_image_free(imageData);
    }

}