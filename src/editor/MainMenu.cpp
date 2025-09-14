#include "EditorSystem.h"
#include "EdMaterial.h"
#include "EdParticles.h"
#include "Helpers.h"
#include "MainMenu.h"
#include "MaterialWindow.h"
#include "ParticlesWindow.h"
#include "Resources.h"
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
            if(ImGui::BeginMenu("Save"))
            {
                if(ImGui::MenuItem("Save map"))
                    saveMap();
                if(ImGui::MenuItem("Save map as"))
                    ifd::FileDialog::Instance().Save("SaveMapAsDialog", "Save map as", "Map file (*.yaml){.yaml}");
                ImGui::EndMenu();
            }
            if(ImGui::MenuItem("Quit"))
                EditorSystem::getSingletonRef().quit();
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("View"))
        {
            NavigationPanel& panel = EditorSystem::getSingletonRef().getNavigation();
            bool open_nav = panel.isOpen();
            ImGui::MenuItem("Navigation panel", 0, &open_nav);
            panel.setOpen(open_nav);
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
    {
        if(ifd::FileDialog::Instance().HasResult())
        {
            std::string filename = ifd::FileDialog::Instance().GetResult();
            loadMaterial(filename);
        }
        ifd::FileDialog::Instance().Close();
    }

    if(ifd::FileDialog::Instance().IsDone("PSFileDialog"))
    {
        if(ifd::FileDialog::Instance().HasResult())
        {
            std::string filename = ifd::FileDialog::Instance().GetResult();
            loadParticles(filename);
        }
        ifd::FileDialog::Instance().Close();
    }

    if(ifd::FileDialog::Instance().IsDone("ActorsFileDialog"))
        ifd::FileDialog::Instance().Close();

    if(ifd::FileDialog::Instance().IsDone("SaveMapAsDialog"))
    {
        if(ifd::FileDialog::Instance().HasResult())
        {
            std::string filename = ifd::FileDialog::Instance().GetResult();
            saveMapAs(filename);
        }
        ifd::FileDialog::Instance().Close();
    }
}

bool MainMenu::loadMap(const std::string& mapFilename)
{
    EditorSystem::getSingletonRef().loadMap(mapFilename);
    return true;
}

bool MainMenu::loadMaterial(const std::string& matFileName)
{
    LOG_S(INFO) << "Loading material " << matFileName;
    std::string matName = removeExtension(getRelativePath(matFileName));

    EdMaterial* material = (EdMaterial*)Resources::getSingletonRef().load(RES_MATERIAL, matName);
    if(material)
    {
        if(!material->isEdited())
            new MaterialWindow(material);
        return true;
    }
    else
        return false;
}

bool MainMenu::loadParticles(const std::string& filename)
{
    LOG_S(INFO) << "Loading particles system " << filename;
    std::string particlesName = removeExtension(getRelativePath(filename));

    EdParticles* particles = (EdParticles*)Resources::getSingletonRef().load(RES_PARTICLES, particlesName);
    if(particles)
    {
        if(!particles->isEdited())
            new ParticlesWindow(particles);
        return true;
    }
    else
        return false;
}

bool MainMenu::saveMap()
{
    EditorSystem::getSingletonRef().saveMap();
    return true;
}

bool MainMenu::saveMapAs(const std::string& filename)
{
    EditorSystem::getSingletonRef().saveMapAs(filename);
    return true;
}

}