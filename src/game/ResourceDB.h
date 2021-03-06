
#pragma once

#include "Geometry.h"
#include "GameMaterial.h"
#include "TemplateMesh.h"
#include "BuildingTemplate.h"
#include "TemplateSound.h"
#include "../core/TDataBase.h"
#include "../core/TSingleton.h"
#include "../particles/System.h"

using namespace Core;

class ResourceDB : public TSingleton<ResourceDB>
{
public:

	typedef TDataBase<Geometry>					GeometryDB;
	typedef TDataBase<GameMaterial>				MaterialDB;
	typedef TDataBase<TemplateMesh>				MeshDB;
	typedef TDataBase<TemplateSound>			SoundDB;
	typedef TDataBase<Particles::SystemTemplate>	ParticleDB;
	typedef TDataBase<BuildingTemplate>			BuildingDB;

protected:

	TDataBase<Geometry>					_geometry;
	TDataBase<GameMaterial>				_material;
	TDataBase<TemplateMesh>				_mesh;
	TDataBase<TemplateSound>			_sound;
	TDataBase<Particles::SystemTemplate>	_particles;
	TDataBase<BuildingTemplate>			_building;

public:

	ResourceDB()						{}
	~ResourceDB()						{}

	void									clear()
	{
		_building.clear();
		_particles.clear();
		_sound.clear();
		_mesh.clear();
		_material.clear();
		_geometry.clear();
	}

	GeometryDB&							getGeometryDB()			{ return _geometry; }
	MaterialDB&							getMaterialDB()			{ return _material; }
	MeshDB&								getMeshDB()				{ return _mesh; }
	SoundDB&							getSoundDB()			{ return _sound; }
	ParticleDB&							getParticleDB()			{ return _particles; }
	BuildingDB&							getBuildingDB()			{ return _building; }
};
