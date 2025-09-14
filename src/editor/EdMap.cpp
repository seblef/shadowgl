#include "EdMap.h"
#include "EdGround.h"
#include "EdMaterial.h"
#include "EdStatic.h"
#include "LightObject.h"
#include "Resources.h"
#include "../core/FileSystemFactory.h"
#include "../core/YAMLCore.h"
#include "../game/Ground.h"
#include "../game/GroundFileFormat.h"
#include "../renderer/LightSystem.h"
#include "../renderer/MaterialSystem.h"
#include "../renderer/Renderer.h"
#include "../loguru.hpp"
#include <memory>

namespace Editor
{

EdMap::EdMap(const std::string& filename) :
    _filename(filename),
    _ground(0),
    _width(0),
    _height(0),
    _startPosition(Core::Vector2::NullVector),
    _valid(false)
{
    YAML::Node root;
    try
    {
        root = YAML::LoadFile(filename);
    }
    catch(const std::exception& e)
    {
        LOG_S(ERROR) << "Failed loading map " << filename << " - " << e.what();
        return;
    }

    YAML::Node mapNode = root["map"];
    parseMapNode(mapNode);

    YAML::Node globalLightNode = root["global_light"];
    if(globalLightNode)
        parseGlobalLightNode(globalLightNode);
    else
        LightSystem::getSingletonRef().getGlobalLight().disableGlobalLight();

    YAML::Node resources = root["resources"];
    parseResourcesNode(resources);

    YAML::Node objects = root["objects"];
    parseObjectsNode(objects);

    loadGround();
}

EdMap::~EdMap()
{
    if(_ground)
        delete _ground;
    for(auto& obj : _objects)
    {
        obj->onRemFromScene();
        delete obj;
    }
}

void EdMap::parseMapNode(const YAML::Node& node)
{
    _width = node["width"].as<int>();
    _height = node["height"].as<int>();
    _startPosition =node["start_position"].as<Core::Vector2>();
    _groundFile = node["ground"].as<string>();
    Core::Color ambientLight = node["ambient_light"].as<Core::Color>();

    LightSystem::getSingletonRef().getGlobalLight().setAmbientLight(ambientLight);
}

void EdMap::parseGlobalLightNode(const YAML::Node& node)
{
    Core::Vector3 dir = node["direction"].as<Core::Vector3>();
    Core::Color color = node["color"].as<Core::Color>();

    LightSystem::getSingletonRef().getGlobalLight().enableGlobalLight(
        color, dir
    );
}

void EdMap::parseResourcesNode(const YAML::Node& node)
{
    YAML::Node materials = node["materials"];
    parseMaterialsNode(materials);

    YAML::Node geometries = node["geometries"];
    parseGeometriesNode(geometries);

    YAML::Node statics = node["meshes"];
    parseStaticsNode(statics);
}

void EdMap::parseMaterialsNode(const YAML::Node& node)
{
    for(YAML::const_iterator mat = node.begin(); mat!=node.end(); ++mat)
    {
        const std::string& name(mat->first.as<std::string>());
        EdMaterial* material = new EdMaterial(
            name,
            mat->second
        );
        if(material->isValid())
            Resources::getSingletonRef().add(RES_MATERIAL, material, name);
    }
}

void EdMap::parseGeometriesNode(const YAML::Node& node)
{
    for(auto geo=node.begin(); geo!=node.end(); ++geo)
        Resources::getSingletonRef().load(RES_GEOMETRY, geo->as<std::string>());
}

void EdMap::parseStaticsNode(const YAML::Node& node)
{
    for(auto stat=node.begin(); stat!=node.end(); ++stat)
    {
        const std::string& name(stat->first.as<string>());
        EdStaticTemplate* statTemplate = new EdStaticTemplate(
            name, stat->second
        );
        Resources::getSingletonRef().add(
            RES_STATIC,
            statTemplate,
            name
        );
    }
}

void EdMap::parseObjectsNode(const YAML::Node& node)
{
    for(auto obj=node.begin(); obj!=node.end(); ++obj)
    {
        const std::string& objClass((*obj)["class"].as<string>());
        if(objClass == "mesh")
            parseStaticObjNode(*obj);
        else if(objClass == "light")
            parseLightObjNode(*obj);
    }
}

void EdMap::parseBaseObjectNode(
    const YAML::Node& node,
    Object& obj,
    bool position2d,
    bool rotation2d
)
{
    obj.setName(node["name"].as<string>(""));
    Core::Vector3 pos3d, rot3d;
    if(position2d)
    {
        Core::Vector2 pos2d = node["position"].as<Core::Vector2>();
        float height = node["height"].as<float>(.0f);

        if(height == .0f)
        {
            const Core::BBox3& localBBox(obj.getLocalBBox());
            height = -localBBox.getMin().y;
        }

        pos3d = Core::Vector3(pos2d.x, height, pos2d.y);
    }
    else
        pos3d = node["position"].as<Core::Vector3>();

    if(rotation2d)
    {
        float rot = node["rotation"].as<float>(.0f);
        rot3d = Core::Vector3(0.f, rot, 0.f);
    }
    else
        rot3d = node["rotation"].as<Core::Vector3>(Core::Vector3::NullVector);
    
    obj.setPosition(pos3d);
    obj.setRotation(rot3d);
}

void EdMap::parseStaticObjNode(const YAML::Node& node)
{
    const std::string& templateName(node["template"].as<std::string>());
    EdStaticTemplate* staticTemplate = (EdStaticTemplate*)Resources::getSingletonRef().get(
        RES_STATIC,
        templateName,
        false
    );
    if(!staticTemplate)
    {
        LOG_S(WARNING) << "No static template " << templateName << " found";
        return;
    }

    StaticObject* obj = new StaticObject(staticTemplate);
    parseBaseObjectNode(node, *obj, true);
    addObject(obj);
}

void EdMap::parseLightObjNode(const YAML::Node& node)
{
    LightObject *light = new LightObject(node);
    parseBaseObjectNode(node, *light);
    addObject(light);
}

void EdMap::loadGround()
{
    std::unique_ptr<Core::IFile> f(
        Core::FileSystemFactory::getFileSystem()->openFile(_groundFile, Core::FA_READ)
    );
	File::GroundFileHeaderEx h;
	File::GroundFileMaterial m;
	f->read(&h, sizeof(File::GroundFileHeader));

    if(h._header != GROUND_HEADER)
    {
        LOG_S(ERROR) << "Wrong header in map file";
        return;
    }
    if(h._width != _width || h._height != _height)
    {
        LOG_S(ERROR) << "Mismatch between ground and map sizes";
        return;
    }

	unsigned int* flags = new unsigned int[h._width*h._height];
    EdMaterial** mats = new EdMaterial*[h._matCount];
	std::unique_ptr<Material*[]> mm(new Material*[h._matCount]);
    std::unique_ptr<int[]> matIds(new int[_width * _height]);

	for(int i=0;i<h._matCount;++i)
	{
		f->read(&m,sizeof(File::GroundFileMaterial));
		mats[i] = (EdMaterial*)Resources::getSingletonRef().get(RES_MATERIAL, m._name);
        mm[i] = mats[i]->getMaterial();
	}

	for (int i = 0; i < h._gameMatCount; ++i)
		f->read(&m, sizeof(File::GroundFileMaterial));

	f->read(flags, sizeof(unsigned int) * h._width * h._height);

    unsigned int *flag_ptr = flags;
    int *mat_ptr = matIds.get();
    for(int i=0;i<_width * _height;++i, ++flag_ptr, ++mat_ptr)
        *mat_ptr = ((int)(*flag_ptr & GMATERIAL_MASK)) - 1;

    _ground = new EdGround(
        _width,
        _height,
        h._matCount,
        mats,
        mm.get(),
        flags,
        matIds.get()
    );
}

void EdMap::addObject(Object* obj)
{
    obj->onAddToScene();
    _objects.push_back(obj);
}

void EdMap::removeObject(Object* obj)
{
    obj->onRemFromScene();
    _objects.remove(obj);
}

void EdMap::deleteObject(Object* obj)
{
    removeObject(obj);
    delete obj;
}

}
