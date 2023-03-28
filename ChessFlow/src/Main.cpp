#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "ChessFlow/Square.h"
#include "ChessFlow/Board.h"
#include "ChessFlow/Piece.h"

int WINDOW_WIDTH = 1200;
int WINDOW_HEIGHT = 800;

bool showImGuiDemo = false;

double currentTime, deltaTime, previousTime;
bool externClose = false;

int fps = 0;
float targetFramesperSecond = 60.0;

glm::vec2 mousePos;
glm::vec2 windowOffset;
glm::vec2 mousePosClamped;
bool mouseClikced;
bool mouseUpdateDetect = false;

ChessFlow::Board board;

char fenToSet[128] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

//ChessFlow::Texture frameBufferTexture;

void checkMousePos(GLFWwindow* window, double x, double y) {
    glm::vec2 mPos = glm::vec2(x - windowOffset.x, WINDOW_HEIGHT - y + windowOffset.y) * 8.f / (float)WINDOW_HEIGHT;
    //mPos = ChessFlow::Square::proj * mPos;
    mousePos = mPos;
    mousePosClamped = glm::clamp(mousePos, glm::vec2(0, 0), glm::vec2(7, 7));
    ChessFlow::Piece::mousePos = mousePos;
    //PLOGI << std::floor(mPos.x) << " " << std::floor(mPos.y);
}

void mouseButton(GLFWwindow* window, int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_LEFT && mouseUpdateDetect) {
        glm::vec2 mPos = glm::floor(mousePos);
        if(action == GLFW_PRESS) {
            mouseClikced = true;
            board.onMouseDown();
        }
        else if(action == GLFW_RELEASE && mouseClikced) {
            mouseClikced = false;
            //PLOGW << "Move  B" << (char)('a' + p->sq.position.x) << p->sq.position.y + 1;
            board.onMouseUp();
        }
    }
}

void windowResize(GLFWwindow* window, int w, int h) {
    WINDOW_WIDTH = w;
    WINDOW_HEIGHT = h;
    //ChessFlow::Square::proj = glm::ortho(0.f, 8.f * (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.f, 8.f);
}


int main() {
    plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);
    PLOGV << "APPLICATION STARTED";

    PLOGV << "Initializing GLFW";
    if(!glfwInit()) {
        PLOGF << "Could not initialize GLFW";
        return -1;
    }
    else PLOGV << "GLFW initialized";
    PLOGV << "Creating Window";
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ChessFlow", NULL, NULL);
    if(!window) {
        PLOGF << "Could not initialize window";
        return -1;
    }
    else PLOGV << "Window Created";

    PLOGV << "Loading OpenGL";
    glfwMakeContextCurrent(window);
    //glfwSwapInterval(0);
    if(!gladLoadGL()) {
        PLOGF << "Could not load OpenGL";
        return -1;
    }
    else PLOGV << "OpenGL loaded";

    glfwSetCursorPosCallback(window, checkMousePos);
    glfwSetMouseButtonCallback(window, mouseButton);
    glfwSetWindowSizeCallback(window, windowResize);

    PLOGV << "Creating FrameBuffer";
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    unsigned int textureColorbuffer;
    ChessFlow::Texture fBuffTex;
    fBuffTex.create(800, 800, 3);
    textureColorbuffer = fBuffTex.textureId;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        PLOGF << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
    else
        PLOGV << "FrameBuffer has been created";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ChessFlow::Square::init();
    ChessFlow::Square::proj = glm::ortho(0.f, 8.f * (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.f, 8.f);
    ChessFlow::Piece::init();

    
    //board.createBoard();
    board.init();

    float lColor[3] =	{ ChessFlow::Square::lightColor.x,  ChessFlow::Square::lightColor.y,    ChessFlow::Square::lightColor.z}, 
          dColor[3] =	{ ChessFlow::Square::darkColor.x,   ChessFlow::Square::darkColor.y,     ChessFlow::Square::darkColor.z };

    PLOGV << "Initializing ImGui";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    
    ImGui::StyleColorsLight();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    PLOGV << "ImGui Initialized";

    if(showImGuiDemo)
        PLOGW << "ImGui demo shown";

    ImFont* ubFont = io.Fonts->AddFontFromFileTTF("Assets/UbuntuMono-Regular.ttf", 18);


    while(!(glfwWindowShouldClose(window) || externClose)) {
        currentTime = glfwGetTime();
        deltaTime = currentTime - previousTime;

        if(!(deltaTime >= 1.0 / targetFramesperSecond)) {
            
            continue;
        }

        previousTime = currentTime;
        glfwPollEvents();

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        board.update();
        board.draw();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();

        ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoCollapse);// | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        //ImGui::SetWindowSize(ImVec2(WINDOW_WIDTH - WINDOW_HEIGHT, WINDOW_HEIGHT));
        //ImGui::SetWindowPos(ImVec2(WINDOW_HEIGHT, 0));
        ImGui::PushFont(ubFont);

        if(ImGui::TreeNode("FPS")) {
            ImGui::Text("FPS : %.2f", 1.f / deltaTime);
            ImGui::SliderFloat("Target FPS", &targetFramesperSecond, 5.f, 200.f, "%.1f");
            ImGui::TreePop();
        }
        //ImGui::SetNextItemOpen(true);
        if(ImGui::TreeNode("Colors")) {
            ImGui::ColorEdit3("LightColor", lColor);
            ImGui::ColorEdit3("DarkColor", dColor);
            ImGui::TreePop();
        }
        //ImGui::Checkbox("Use Texture", &ChessFlow::Square::useTexture);
        glm::vec2 temp = mousePos;
        ImGui::Text("Mouse Position");
        ImGui::InputFloat2("", glm::value_ptr(temp), "%.2f");
        ImGui::Text("Square %c%d", 'a' + (int)std::floor(mousePosClamped.x), 1 + (int)std::floor(mousePosClamped.y));
        ImGui::Text("LastMove %s", ChessFlow::Piece::lastMove.c_str());
        //temp = mousePos;
        ImGui::InputText("FEN", fenToSet, IM_ARRAYSIZE(fenToSet));

        if(ImGui::Checkbox("Show Demo Window", &showImGuiDemo))
            if(showImGuiDemo)
                PLOGW << "ImGui Demo Window is shown";

        if(ImGui::Button("Flip Board"))
            board.flip();
        ImGui::SameLine();
        if(ImGui::Button("Set Fen")) {
            board.setFen(fenToSet);
        }
        ImGui::SameLine();
        if(ImGui::Button("Revert"))
            board.setDefaultFen();
        ImGui::SameLine();
        if(ImGui::Button("Close"))
            externClose = true;


        ImGui::PopFont();
        ImGui::End();

        ImGui::SetNextWindowSize(ImVec2(800, 800));
        ImGui::Begin("Board", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
        ImGui::SetWindowSize(ImVec2(800, 800));
        float minDimentions = std::min(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
        ImGui::SetCursorPos(ImVec2(0, 0));
        ImGui::Image((void*)(intptr_t)textureColorbuffer, ImVec2(minDimentions, minDimentions), ImVec2(0, 1), ImVec2(1, 0));
        //mouseUpdateDetect = ImGui::IsItemClicked();
        windowOffset = glm::vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
        //PLOGW << ImGui::Get;
        mouseUpdateDetect =  ImGui::IsItemHovered();
        ImGui::End();

        if(showImGuiDemo)
            ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ChessFlow::Square::lightColor = glm::vec3(lColor[0], lColor[1], lColor[2]);
        ChessFlow::Square::darkColor =  glm::vec3(dColor[0], dColor[1], dColor[2]);

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    PLOGV << "ImGui Terminated";

    glfwTerminate();
    PLOGV << "GLFW terminated";
    PLOGV << "Application Ended";
    return 0;
}