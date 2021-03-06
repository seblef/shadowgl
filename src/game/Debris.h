
#pragma once

#include "Action.h"
#include "../core/Vector3.h"

class Mesh;
class MeshInstance;

using namespace Core;

class Debris : public Action
{
protected:

	class DebrisItem
	{
	public:

		MeshInstance*		_meshInstance;
		Vector3				_velocity;
		Vector3				_angleSpeed;
		Vector3				_position;
		Vector3				_rotation;
		float				_size;
	};

	int					_count;
	int					_alive;
	DebrisItem*			_items;

	void				updateItemMatrix(DebrisItem& d, Matrix4& w) const;

public:

	Debris(Mesh* mesh, int count, const Vector3& pos,const Vector3& normal, float maxAngle,
		float minVelocity, float maxVelocity, float maxAngleSpeed, float size);
	~Debris();

	void				update(float time);
};
