#ifndef STATIC_CAMERA_H
#define STATIC_CAMERA_H

#include "Camera.h"

class StaticCamera : public Camera
{
private:
    virtual void Update(float dt) override;

public:
    StaticCamera(void);
    StaticCamera(float x, float y, float z);
    virtual ~StaticCamera(void) override;
};

#endif
