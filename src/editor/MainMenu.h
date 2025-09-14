#pragma once

#include "IEditorElement.h"
#include <string>

namespace Editor
{

class MainMenu : public IEditorElement
{
protected:
    void updateFileDialogs();

    bool loadMap(const std::string& mapFilename);
    bool loadMaterial(const std::string& matFilename);
    bool loadParticles(const std::string& filename);

    bool saveMap();
    bool saveMapAs(const std::string& filename);

public:

    MainMenu() {}
    ~MainMenu() {}

    void draw();
};

}