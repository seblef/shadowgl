#pragma once

#include "../core/BBox3.h"
#include "../core/Vector2.h"
#include <list>
#include <string>

namespace YAML
{
class Node;
}

namespace Editor
{

class EdGround;
class Object;


class EdMap
{
protected:
    bool _valid;
    std::string _name;
    std::string _filename;

    std::string _groundFile;
    EdGround* _ground;

    int _width, _height;
    Core::Vector2 _startPosition;

    std::list<Object*> _objects;

    void parseMapNode(const YAML::Node& node);
    void parseGlobalLightNode(const YAML::Node& node);
    void parseResourcesNode(const YAML::Node& node);
    void parseMaterialsNode(const YAML::Node& node);
    void parseGeometriesNode(const YAML::Node& node);
    void parseStaticsNode(const YAML::Node& node);
    void parseObjectsNode(const YAML::Node& node);
    void parseBaseObjectNode(
        const YAML::Node& node,
        Object& obj,
        bool position2d=false,
        bool rotation2d=false
    );
    void parseStaticObjNode(const YAML::Node& node);
    void parseLightObjNode(const YAML::Node& node);

    void loadGround();

public:
    EdMap(const std::string& filename);
    ~EdMap();

    const std::string& getName() const { return _name; }
    const std::string& getFilename() const { return _filename; }

    bool isValid() const { return _valid; }

    int getWidth() const { return _width; }
    int getHeight() const { return _height; }

    void addObject(Object* obj);
    void removeObject(Object* obj);
    void deleteObject(Object* obj);
    const std::list<Object*>& getAllObjects() const { return _objects; }
};

}