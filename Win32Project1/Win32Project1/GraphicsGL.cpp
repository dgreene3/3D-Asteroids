
#include "GraphicsGL.h"



float CalcFrustumScale(float fFovDeg) {
	float fFovRad = fFovDeg * degToRad;
	return 1.0f / tan(fFovRad / 2.0f);
}