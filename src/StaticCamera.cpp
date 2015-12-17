#include "StaticCamera.h"

StaticCamera::StaticCamera(void)
    : StaticCamera(0.0f, 0.0f, 0.0f)
{
    /* Nothing to do. */
}

StaticCamera::StaticCamera(float x, float y, float z)
    : Camera(x, y, z)
{
    /* Nothing to do. */
}

StaticCamera::~StaticCamera(void)
{
    /* Nothing to do. */
}

void StaticCamera::Update(float dt)
{
    /* Nothing to do. */
}
