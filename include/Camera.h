#pragma once
#ifndef _CAMERA_H_

#include "stdafx.h"

class Camera
{
public:
	Camera(HWND h, float aspectRatio, XMVECTOR cPos);
	~Camera();

	XMFLOAT4X4 mWVPMat;

	void GetViewMat();

	void Reset();
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(USHORT btnFlag, int x, int y);
	void OnMouseWheelRotate(short delta);

	void SetCameraPositin(XMVECTOR camPos) { mEye = camPos; }
	void GetTransWVPMat();

private:
	HWND mHwnd;

	XMVECTOR mEye;
	XMVECTOR mAt;
	XMVECTOR mUp;

	XMFLOAT4X4 mProjMat;
	XMFLOAT4X4 mViewMat;
	XMFLOAT4X4 mWorldMat;

	float mTheta = 1.5f * XM_PI;
	float mPhi = XM_PIDIV4;
	float mRadius = 2.0f;

	POINT mMousePos;
};
#endif // !_CAMERA_H_