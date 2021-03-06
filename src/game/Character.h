
#pragma once

#include <map>
#include <string>

class Actor;
class GameMaterial;

using namespace std;


class Character
{
public:

	typedef std::map<std::string, std::string> AnimMap;

protected:

	string				_name;
	string				_actor;
	string				_material;
	string				_weapon;
	string				_aiMachine;
	string				_aiState;
	float				_fov;
	float				_visDistance;
	float				_forwardSpeed;
	float				_backwardSpeed;
	float				_lateralSpeed;
	float				_rotationSpeed;
	float				_lifePoints;
	AnimMap				_animations;

	GameMaterial*		_rMat;
	Actor*				_rActor;

public:

	Character(
		const string& name,
		const string& actor,
		const string& material,
		float forwardSpeed,
		float backwardSpeed,
		float lateralSpeed,
		float rotationSpeed,
		const string& weapon,
		const string& aiMachine,
		const string& aiState,
		float fov,
		float visDistance,
		float life,
		const AnimMap& anims
	);
	~Character()							{ unload(); }

	const string&							getName() const							{ return _name; }
	const string&							getWeapon() const						{ return _weapon; }
	const string&							getAIMachine() const					{ return _aiMachine; }
	const string&							getAIState() const						{ return _aiState; }
	float									getForwardSpeed() const					{ return _forwardSpeed; }
	float									getBackwardSpeed() const				{ return _backwardSpeed; }
	float									getLateralSpeed() const					{ return _lateralSpeed; }
	float									getRotationSpeed() const				{ return _rotationSpeed; }
	float									getFOV() const							{ return _fov; }
	float									getVisibilityDistance() const			{ return _visDistance; }
	float									getLifePoints() const					{ return _lifePoints; }

	GameMaterial*							getMaterial() const						{ return _rMat; }
	Actor*									getActor() const						{ return _rActor; }

	void									load();
	void									unload();
};
