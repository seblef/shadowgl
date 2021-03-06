#pragma once

#include "GlobalLight.h"
#include "Light.h"
#include "LightRenderer.h"
#include "GBuffer.h"
#include "../core/TSingleton.h"


class LightSystem : public Core::TSingleton<LightSystem>
{
protected:

	IVideoDevice*		_device;

	LightGlobal			_global;
	LightRenderer*		_renderers[Light::LT_COUNT];

	IConstantBuffer*	_cLightBuffer;
	ShadowMapBuffer		_shadowBuffer;
	RenderState			_rsIn;
	RenderState			_rsOut;
	DepthStencilState	_dsIn;
	DepthStencilState	_dsOut;
	BlendState			_blendState;
	SamplerState		_samplerState;

	BlendState			_noBlendState;

public:

	LightSystem(IVideoDevice* device);
	~LightSystem();

	LightGlobal&		getGlobalLight()						{ return _global; }
	const LightGlobal&	getGlobalLight() const					{ return _global; }

	void				enqueueLight(Light* l);

	void				applyLights(const Camera& camera, GBuffer& gbuf);
};
