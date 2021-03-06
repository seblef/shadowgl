#pragma once

#include "../mediacommon/IShadowMap.h"
#include <GL/glew.h>


class GLShadowMap : public IShadowMap
{
protected:

    GLuint          _fbId;
    GLuint          _mapId;

public:

    GLShadowMap(int w, int h);
    ~GLShadowMap();

    void            setAsRenderTarget();
    void            set(int stage);
};
