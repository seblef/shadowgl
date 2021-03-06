#pragma once

#include "PhysicDefs.h"
#include <PxPhysicsAPI.h>

using namespace physx;

class PhysicGeometry
{
protected:

	PxGeometry*					_geo;
	bool						_needPreTransform;
	Matrix4						_preTransform;

	void						makeBox(const PhysicGeometryCreate_t& c, PxPhysics* p);
	void						makeSphere(const PhysicGeometryCreate_t& c, PxPhysics* p);
	void						makeCapsule(const PhysicGeometryCreate_t& c, PxPhysics* p);
	void						makeMesh(const PhysicGeometryCreate_t& c, PxPhysics* p, PxCooking *cook);
	void						makeConvexHull(const PhysicGeometryCreate_t& c, PxPhysics* p, PxCooking *cook);

	void						makeOffsetPreTransform(const Vector3& center);

public:

	PhysicGeometry(const PhysicGeometryCreate_t& c, PxPhysics* p, PxCooking *cook);
	~PhysicGeometry();

	const PxGeometry&			getGeometry() const					{ return *_geo; }

	bool						needPreTransform() const			{ return _needPreTransform; }
	const Matrix4&				getPreTransform() const				{ return _preTransform; }
};
