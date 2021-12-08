#include "EditorSystem.h"
#include "MainMenu.h"
#include "imgui/imgui.h"
#include "filedialog/ImFileDialog.h"
#include "../loguru.hpp"

namespace Editor
{

void MainMenu::draw()
{
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::BeginMenu("Open"))
            {
                if(ImGui::MenuItem("Map"))
                    ifd::FileDialog::Instance().Open("MapsFileDialog", "Open a map", "Map file (*.yaml){.yaml}", false);
                if(ImGui::MenuItem("Material"))
                    ifd::FileDialog::Instance().Open("MaterialsFileDialog", "Open a material", "Material file (*.yaml){.yaml}", false);
                if(ImGui::MenuItem("Particle system"))
                    ifd::FileDialog::Instance().Open("PSFileDialog", "Open a particle system", "Particle system file (*.yaml){.yaml}", false);
                if(ImGui::MenuItem("Actor"))
                    ifd::FileDialog::Instance().Open("ActorsFileDialog", "Open an actor", "Actor file (*.act){.act}", false);
                ImGui::EndMenu();
            }
            if(ImGui::MenuItem("Quit"))
                EditorSystem::getSingletonRef().quit();
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    updateFileDialogs();
}

void MainMenu::updateFileDialogs()
{
    if(ifd::FileDialog::Instance().IsDone("MapsFileDialog"))
    {
        if(ifd::FileDialog::Instance().HasResult())
        {
            std::string filename = ifd::FileDialog::Instance().GetResult();
            loadMap(filename);
        }
        ifd::FileDialog::Instance().Close();
    }

    if(ifd::FileDialog::Instance().IsDone("MaterialsFileDialog"))
        ifd::FileDialog::Instance().Close();

    if(ifd::FileDialog::Instance().IsDone("PSFileDialog"))
        ifd::FileDialog::Instance().Close();

    if(ifd::FileDialog::Instance().IsDone("ActorsFileDialog"))
        ifd::FileDialog::Instance().Close();
}

bool MainMenu::loadMap(const std::string& mapFilename)
{
    LOG_S(INFO) << "Loading map " << mapFilename;
    return true;
}

}