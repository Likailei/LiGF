#pragma once
#ifndef _FIRST_PERSON_CAMERA_H

#include "stdafx.h"

class ThirdPersonCamera {
public:
	ThirdPersonCamera(float aspectRatio, XMFLOAT3 pos);
	
	void Update(XMFLOAT4X4& viewProjMat);
	void Strafe(float t);
	void Walk(float t);
	void Shake(float x, float y);

private:
	bool ViewMatChanged = true;

	XMVECTOR EyePos;
	XMVECTOR LookDirection;
	XMVECTOR Right;
	XMVECTOR Up;

	XMMATRIX ViewMat;
	XMMATRIX ProjMat;
	XMMATRIX ViewProjMat;

	void UpdateViewMat();
	void Yaw(float angle);
	void Pitch(float angle);
};
#endif // !_FIRST_PERSON_CAMERA_H

