
#include "SoundVirtualSource.h"
#include "SoundPlayingSource.h"
#include "../mediacommon/ISoundVirtualSource.h"
#include "../mediacommon/ISoundDevice.h"


SoundVirtualSource::SoundVirtualSource(
	ISoundDevice *device,
	int priority,
	float gain,
	bool loop,
	const Vector3& position,
	const Vector3& velocity,
	const Vector3& direction,
	float refDist, float maxDist,
	float rollOff,
	float inAngle, float outAngle,
	bool managed
) :
	_playingSource(0),
	_priority(priority),
	_loop(loop),
	_managed(managed),
	_sound(0),
	_stopped(false),
	_gain(gain)
{
	_source = device->createVirtualSource(
		position,
		velocity,
		direction,
		refDist, maxDist,
		rollOff,
		inAngle, outAngle
	);
	_source->setGain(gain);
}

SoundVirtualSource::~SoundVirtualSource()
{
    delete _source;
}

void SoundVirtualSource::updateDistance(const Vector3& listener)
{
    _distance = (_source->getPosition() - listener).getSquareLength();
}