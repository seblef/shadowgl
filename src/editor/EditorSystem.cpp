#include "EditorSystem.h"
#include "EdCamera.h"
#include "EdMap.h"
#include "IWindow.h"
#include "PreviewResources.h"
#include "Resources.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "filedialog/ImFileDialog.h"
#include "../mediacommon/IMedia.h"
#include "../mediacommon/IVideoDevice.h"
#include "../renderer/Renderer.h"
#include "../loguru.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>


namespace Editor
{

EditorSystem::EditorSystem(IMedia* media, const YAML::Node& cfg) :
    _canQuit(false),
    _media(media),
    _map(0)
{
    _previewRes = new PreviewResources(media->getVideo());
    new Resources;
    initUI();
}

EditorSystem::~EditorSystem()
{
    if(_map)
        delete _map;
    shutdownUI();
    Resources::deleteSingleton();
    delete _previewRes;
}

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
    Renderer::getSingletonRef().onResize(width, height);
}


void EditorSystem::initUI()
{
    GLFWwindow *window = (GLFWwindow*)_media->getWindow();
    glfwSetWindowSizeCallback(window, windowSizeCallback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();

	ifd::FileDialog::Instance().CreateTexture = [](uint8_t* data, int w, int h, char fmt) -> void* {
		GLuint tex;

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, (fmt == 0) ? GL_BGRA : GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		return (void*)tex;
	};
	ifd::FileDialog::Instance().DeleteTexture = [](void* tex) {
		GLuint texID = (GLuint)((uintptr_t)tex);
		glDeleteTextures(1, &texID);
	};
}

void EditorSystem::shutdownUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


bool EditorSystem::update()
{
    GLFWwindow *window = (GLFWwindow*)_media->getWindow();

	_media->getVideo()->newFrame();

    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    Renderer::getSingletonRef().update(.0f, &_camera.getCamera());

    _mainMenu.draw();
    _navPanel.draw();
    drawWindows();


    bool open;
    ImGui::ShowDemoWindow(&open);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return glfwWindowShouldClose(window) || _canQuit;
}

void EditorSystem::quit()
{
    _canQuit = true;
}

void EditorSystem::openWindow(IWindow* win)
{
    _openWindows.push_back(win);
}

void EditorSystem::drawWindows()
{
    std::vector<IWindow*> closedWindows;
    for(auto& win : _openWindows)
    {
        win->draw();
        if(!win->isOpen())
            closedWindows.push_back(win);
    }

    for(auto const& win : closedWindows)
    {
        delete win;
        _openWindows.remove(win);
    }
}

void EditorSystem::loadMap(const std::string& filename)
{
    if(_map)
        delete _map;
    LOG_S(INFO) << "Loading map " << filename;
    _map = new EdMap(filename);
    _camera.initialize(_map->getWidth(), _map->getHeight());
}

}
