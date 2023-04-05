#include "ChessFlow/Square.h"
#include "ChessFlow/HandleError.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <plog/Log.h>

namespace ChessFlow {
    float Square::vertices[4 * 5] = {
        //position          //tex-coords
        1.f, 1.f, 0.f,      1.0f, 0.0f,         //top right
        1.f, 0.f, 0.f,      1.0f, 1.0f,         //bottom right
        0.f, 0.f, 0.f,      0.0f, 1.0f,         //bottom left
        0.f, 1.f, 0.f,      0.0f, 0.0f          //top left
    };

    unsigned int Square::indices[2 * 3] = {
        0, 1, 2,
        2, 3, 0
    };

    glm::vec3 Square::lightColor(1.f, 1.f, 1.f), Square::darkColor(66.f/255.f, 167.f/255.f, 54.f/255.f);

    unsigned int Square::vao = 0, Square::vbo = 0, Square::ebo = 0;
    glm::mat4 Square::proj(1.0), Square::view(1.0);


    Shader Square::squareShader = Shader();

    //bool Square::useTexture = false;
    ChessFlow::Texture Square::sampleTexture;

    Square::Square() :
        model(1.f)
    {
        color = &darkColor;
        tex = &sampleTexture;
        useTexture = false;
    }

    void Square::init() {
        squareShader.loadFromFile("Assets/SquareVertex.glsl", "Assets/SquareFragment.glsl");
        sampleTexture.loadFromFile("Assets/a.png");

        squareShader.setMatrix4("projection", proj);
        squareShader.setMatrix4("view", view);
        //squareShader.setMatrix4("model", model);

        squareShader.setVec3("sqCol", lightColor);

        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glGenBuffers(1, &vbo));
        GLCall(glGenBuffers(1, &ebo));

        GLCall(glBindVertexArray(vao));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(ChessFlow::Square::vertices), ChessFlow::Square::vertices, GL_STATIC_DRAW));
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), 0));
        GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float))));
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glEnableVertexAttribArray(1));

        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ChessFlow::Square::indices), ChessFlow::Square::indices, GL_STATIC_DRAW));
    }

    void Square::setPosition(glm::vec3 newPos) {
        model = glm::translate(glm::mat4(1), newPos);
        position = newPos;
    }

    void Square::draw() {
        GLCall(glBindVertexArray(vao));
        squareShader.setMatrix4("model", model);
        squareShader.setMatrix4("projection", proj);
        squareShader.setMatrix4("view", view);
        squareShader.setBool("useColor", !useTexture);
        squareShader.setBool("toInvertColor", toInvertColor);

        
        squareShader.setVec3("sqCol", *color);
        if(tex != NULL)
            squareShader.setTexture(*tex);

        GLCall(glUseProgram(squareShader.shaderProgram));

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
        GLCall(glUseProgram(NULL));
        GLCall(glBindVertexArray(NULL));
    }
}