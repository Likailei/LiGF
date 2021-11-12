#include "ThirdPersonCamera.h"

ThirdPersonCamera::ThirdPersonCamera(float aspectRatio, XMFLOAT3 pos)
{
	EyePos = XMLoadFloat3(&pos);
	ProjMat = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI * .25f, aspectRatio, 0.1f, 1000.f);
	LookDirection = XMVectorSet(.0f, .0f, 1.f, .0f);
	Up = XMVectorSet(.0f, 1.f, .0f, .0f);
	Right = XMVectorSet(1.f, .0f, .0f, .0f);
}

void ThirdPersonCamera::Update(XMFLOAT4X4& viewProjMat)
{
	if (ViewMatChanged) {
		UpdateViewMat();
		ViewProjMat = DirectX::XMMatrixMultiply(ViewMat, ProjMat);
		ViewProjMat = XMMatrixTranspose(ViewProjMat);
		ViewMatChanged = false;
		/*char str[200];
		sprintf_s(str, "%.6f %.6f %.6f %.6f\n%.6f %.6f %.6f %.6f\n%.6f %.6f %.6f %.6f\n%.6f %.6f %.6f %.6f\n",
			XMVectorGetX(ViewProjMat.r[0]), XMVectorGetY(ViewProjMat.r[0]), XMVectorGetZ(ViewProjMat.r[0]), XMVectorGetW(ViewProjMat.r[0]),
			XMVectorGetX(ViewProjMat.r[1]), XMVectorGetY(ViewProjMat.r[1]), XMVectorGetZ(ViewProjMat.r[1]), XMVectorGetW(ViewProjMat.r[1]),
			XMVectorGetX(ViewProjMat.r[2]), XMVectorGetY(ViewProjMat.r[2]), XMVectorGetZ(ViewProjMat.r[2]), XMVectorGetW(ViewProjMat.r[2]),
			XMVectorGetX(ViewProjMat.r[3]), XMVectorGetY(ViewProjMat.r[3]), XMVectorGetZ(ViewProjMat.r[3]), XMVectorGetW(ViewProjMat.r[3]));
		OutputDebugStringA(str);*/
	}
	XMStoreFloat4x4(&viewProjMat, ViewProjMat);
}

void ThirdPersonCamera::Strafe(float t)
{
	auto delta = XMVectorReplicate(t);
	EyePos = XMVectorMultiplyAdd(delta, Right, EyePos);
	ViewMatChanged = true;
}

void ThirdPersonCamera::Walk(float t)
{
	auto delta = XMVectorReplicate(t);
	EyePos = XMVectorMultiplyAdd(delta, LookDirection, EyePos);
	ViewMatChanged = true;
}

void ThirdPersonCamera::Shake(float x, float y)
{
	x = XMConvertToRadians(x / 10.f);
	y = XMConvertToRadians(y / 10.f);
	Yaw(x);
	Pitch(y);
}

void ThirdPersonCamera::Yaw(float angle)
{
	auto y = XMMatrixRotationY(angle);
	Right =	XMVector3TransformNormal(Right, y);
	Up = XMVector3TransformNormal(Up, y);
	LookDirection = XMVector3TransformNormal(LookDirection, y);

	ViewMatChanged = true;
}

void ThirdPersonCamera::Pitch(float angle)
{
	auto p = XMMatrixRotationAxis(Right, angle);
	Up = XMVector3TransformNormal(Up, p);
	LookDirection = XMVector3TransformNormal(LookDirection, p);

	ViewMatChanged = true;
}

void ThirdPersonCamera::UpdateViewMat()
{
	ViewMat = DirectX::XMMatrixLookToLH(EyePos, LookDirection, Up);
}
