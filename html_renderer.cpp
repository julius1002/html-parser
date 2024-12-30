#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_metal.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/examples/libs/glfw/include/GLFW/glfw3.h"
#include <iostream>
#include "html_parser.h"

#define GLFW_INCLUDE_NONE

namespace HtmlRenderer
{
    void create_elements(HtmlParser::HtmlTree htmlTree)
    {
        ImVec4 black_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        for (auto const &elem : htmlTree)
        {
            switch (elem.tagName)
            {
                {
                case HtmlParser::HtmlTag::HTML:
                {
                    create_elements(elem.children);
                    return;
                }
                case HtmlParser::HtmlTag::NAV:
                case HtmlParser::HtmlTag::HEAD:
                case HtmlParser::HtmlTag::BODY:
                case HtmlParser::HtmlTag::DIV:
                case HtmlParser::HtmlTag::SPAN:
                {
                    ImGui::BeginChild("DivContainer", ImVec2(300, 100), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
                    if (elem.content != "")
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, black_color);
                        ImGui::Text(elem.content.c_str());
                        ImGui::PopStyleColor();
                    }
                    create_elements(elem.children);

                    ImGui::EndChild();
                    continue;
                }
                case HtmlParser::HtmlTag::P:
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, black_color);
                    ImGui::Dummy(ImVec2(0, 10));
                    ImGui::Text(elem.content.c_str());
                    ImGui::Dummy(ImVec2(0, 10));
                    ImGui::PopStyleColor();
                    continue;
                }
                case HtmlParser::HtmlTag::INPUT:
                {
                    static char buffer[256] = "";
                    ImGui::PushItemWidth(200);
                    if (ImGui::InputText("##", buffer, sizeof(buffer)))
                    {
                    }
                    ImGui::PopItemWidth();
                    ImGui::SameLine();
                    continue;
                }
                case HtmlParser::HtmlTag::BUTTON:
                {
                    ImGui::Button(elem.content.c_str());
                    continue;
                }
                case HtmlParser::HtmlTag::OL:
                case HtmlParser::HtmlTag::UL:
                {
                    ImGui::BeginListBox("ul", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()));
                    {
                        create_elements(elem.children);
                        ImGui::EndListBox();
                    }
                    continue;
                }
                case HtmlParser::HtmlTag::LI:
                {
                    ImGui::Text(elem.content.c_str());
                    create_elements(elem.children);
                    continue;
                }
                case HtmlParser::HtmlTag::IMG:
                {
                }
                case HtmlParser::HtmlTag::TABLE:
                {
                }
                case HtmlParser::HtmlTag::TR:
                {
                }
                case HtmlParser::HtmlTag::TH:
                {
                }
                case HtmlParser::HtmlTag::TD:
                {
                }
                default:
                {
                }
                }
            }
        }
    }

    int render(HtmlParser::HtmlTree htmlTree)
    {
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return -1;
        }

        GLFWwindow *window = glfwCreateWindow(1280, 720, "Dear ImGui + OpenGL3", NULL, NULL);
        if (!window)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 120");
        ImVec4 new_bg_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, new_bg_color);

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImVec2 viewport_size = ImGui::GetIO().DisplaySize;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

            ImGui::SetNextWindowSize(viewport_size);
            ImGui::SetNextWindowPos(ImVec2(0, 0));

            ImGui::Begin("Fullscreen Window", nullptr, window_flags);

            create_elements(htmlTree);

            ImGui::End();

            ImGui::Render();
            glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);
        glfwTerminate();
        return 0;
    }
}