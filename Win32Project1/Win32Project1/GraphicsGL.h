#ifndef GRAPHICSGL_H
#define GRAPHICSGL_H

#include "Precompiled.h"



//get current camera position and return
const float degToRad = (float)3.14159 * 2.0f / 360.0f;

//get frustum scale for proj matrix
float CalcFrustumScale(float fFovDeg);


#endif
