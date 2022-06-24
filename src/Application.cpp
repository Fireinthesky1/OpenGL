#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/Test.h"



int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                                                      //i want my major version to be openGL 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                                                      //i want my minor version to be openGL 3 so the version is 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);                                      //setting my openGL profile to be core



    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }



    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;



    {
        GLCall(glEnable(GL_BLEND));                                                                         //enable blending
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));                                          //specify the blending equation                                                                                  //<- unbinds the shader       



        Renderer renderer;                                                                                  //create the renderer



        //initialize imgui
        ImGui::CreateContext();                                                                             //create the imgui context
        ImGui_ImplGlfwGL3_Init(window, true);                                                               //initialize the window
        ImGui::StyleColorsDark();                                                                           //set the imgiu theme


        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTexture2D>("2D Texture");



        //rendering loop----------------------------------------------------------------------------------------------------------------------------
        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();

            if (currentTest)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
        delete currentTest;
        if (currentTest != testMenu)
        {
            delete testMenu;
        }
    }



    //CLEANUP
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}