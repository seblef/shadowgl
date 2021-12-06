
#pragma once

#include "../StdRenderer.h"
#include "../PhysicLib.h"
#include "Geometry.h"
#include "GameMaterial.h"

class GameEntity;

namespace YAML
{
	class Node;
}

class EntityTemplate
{
protected:

	string					_meshName;
	string					_materialName;

	Geometry*				_geometry;
	Mesh*					_rMesh;
	GameMaterial*			_rMaterial;
	PhysicGeometry*			_pGeometry;
	PhysicShape				_pShape;

	bool					_isTrigger;

public:

	EntityTemplate(bool isTrigger, const YAML::Node& node);
	virtual ~EntityTemplate()		 { unload(); }

	bool					isTrigger() const					{ return _isTrigger; }

	virtual void			load();
	virtual void			unload();

	Mesh*					getMesh() const						{ return _rMesh; }
	GameMaterial*			getMaterial() const					{ return _rMaterial; }
	Geometry*				getGeometry() const					{ return _geometry; }
	PhysicGeometry*			getPhysicGeometry() const			{ return _pGeometry; }

	virtual GameEntity*		createInstance(const Matrix4& world) = 0;
};
