#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "ChessFlow/Shader.h"
#include "stb_image.h"
#include <iostream>

#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>

int __main() {
    plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);

    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 800, "Title", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGL();

    float vertices[] = {
        // positions         // texture coords
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, // top right
         0.5f, -0.5f, 0.0f,  1.0f, 1.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, // bottom left
        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChanels;
    unsigned char* data = stbi_load("Assets/m.png", &width, &height, &nrChanels, 0);

    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, nrChanels == 3 ? GL_RGB : GL_RGBA, width, height, 0, nrChanels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
        PLOGD << width << " " << height << " " << nrChanels;
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else PLOGE << "Failed to load texture.";

    stbi_image_free(data);

    ChessFlow::Shader mS;// = ChessFlow::Shader();
    mS.loadFromFile("Assets/TexVertex.glsl", "Assets/TexFrag.glsl");

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glUseProgram(mS.shaderProgram);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }
    return 0;
}