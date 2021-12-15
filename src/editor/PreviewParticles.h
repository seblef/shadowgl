#pragma once

#include "PreviewFrame.h"


class IConstantBuffer;

namespace Particles
{
class System;
class SystemTemplate;
}


namespace Editor
{

class PreviewParticles : public PreviewFrame
{
protected:
    std::unique_ptr<Particles::System> _particles;
    std::unique_ptr<IConstantBuffer> _sceneBuffer;
    Core::BBox3 _currentBBox;
    float _prevTime;

    void render(float time);

public:

    PreviewParticles(
        Particles::SystemTemplate* particles,
        IVideoDevice* device,
        PreviewResources* resources,
        int width,
        int height
    );
    ~PreviewParticles();
};

}