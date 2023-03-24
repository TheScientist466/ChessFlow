#pragma once

#include <string>
#include <glm/glm.hpp>

#include "ChessFlow/Texture.h"

namespace ChessFlow {
    struct Shader {
        enum class ShaderType {
            Vertex,
            Fragment
        };

        static const std::string sampleVertex;
        static const std::string sampleFragment;

        Shader();
        ~Shader();

        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int shaderProgram;

        static unsigned int compile(const std::string& source, ShaderType type);
        static std::string read(const std::string& path);
        void loadFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        void loadFromMemory(const std::string& vertexShaderSource = sampleVertex, const std::string& fragmentShaderSource = sampleFragment);

        void use();

        void setBool(std::string name, bool val);

        void setVec2(std::string name, glm::vec2 vec);
        void setVec3(std::string name, glm::vec3 vec);
        void setVec4(std::string name, glm::vec4 vec);

        void setMatrix4(std::string name, glm::mat4 mat);

        void setTexture(ChessFlow::Texture tex);
    };
}