#pragma once

#include "../MediaCommon.h"
#include "GLDefs.h"
#include <GL/glew.h>
#include "GL/gl.h"

class GLConstantBuffer : public IConstantBuffer
{
protected:

    GLuint          _cbId;

public:

    GLConstantBuffer(int size, int slot);
    ~GLConstantBuffer();

    void            set();
    void            fill(const void *data);
};
