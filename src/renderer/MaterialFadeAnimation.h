#pragma once

#include "MaterialAnimation.h"
#include "MaterialCreate.h"

class MaterialFadeAnimation : public MaterialAnimation
{
protected:

	float		_start;
	float		_end;

public:

	MaterialFadeAnimation(Material *m) : MaterialAnimation(m), _start(0), _end(1)			{}
	MaterialFadeAnimation(Material *m, float start, float end) : MaterialAnimation(m), _start(start), _end(end)	{}
	MaterialFadeAnimation(const MaterialFadeAnimation& a) : MaterialAnimation(a._material), _start(a._start), _end(a._end)	{}
	MaterialFadeAnimation(const MaterialCreate& mc, Material* m) : MaterialAnimation(m), _start(mc._fadeAnim._startTime), _end(mc._fadeAnim._endTime)			{}

	const char*	getAnimationName() const				{ return "fade"; }

	void		set(float time);

	float		getStartTime() const					{ return _start; }
	float		getEndTime() const						{ return _end; }

	void		setStartTime(float st)					{ _start=st; }
	void		setEndTime(float et)					{ _end=et; }
};
