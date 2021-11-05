#include "Camera.h"

Camera::Camera(HWND h, float aspectRatio, XMVECTOR cPos) :
	mHwnd(h),
	mEye(cPos),
	mAt(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)),
	mUp(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)),
	mObjPos(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)),
	mRotXYZ(XMFLOAT3(0.000f, 0.000f, 0.000f))
{
	XMStoreFloat4x4(&mProjMat, DirectX::XMMatrixPerspectiveFovLH(45.0f*(3.14f / 180.0f), aspectRatio, 0.1f, 1000.0f));

	XMStoreFloat4x4(&mRotMat, DirectX::XMMatrixIdentity());
}

Camera::~Camera()
{
	mHwnd = nullptr;
}

void Camera::GetViewMat()
{
	float x = Settings::CameraRadius * sinf(Settings::CameraPhi)*cosf(Settings::CameraTheta);
	float z = Settings::CameraRadius * sinf(Settings::CameraPhi)*sinf(Settings::CameraTheta);
	float y = Settings::CameraRadius * cosf(Settings::CameraPhi);

	mEye = XMVectorSet(x, y, z, 1.0f);
	mAt = XMVectorZero();
	mUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMStoreFloat4x4(&mViewMat, XMMatrixLookAtLH(mEye, mAt, mUp));
}

void Camera::GetWorldMat()
{
	XMStoreFloat4x4(&mWorldMat, XMLoadFloat4x4(&mRotMat) * XMMatrixTranslationFromVector(mObjPos));
}

void Camera::GetRotMat(XMFLOAT3 xyz)
{
	XMMATRIX rotXMat = XMMatrixRotationX(xyz.x);
	XMMATRIX rotYMat = XMMatrixRotationY(xyz.y);
	XMMATRIX rotZMat = XMMatrixRotationZ(xyz.z);

	XMStoreFloat4x4(&mRotMat, XMLoadFloat4x4(&mRotMat) * rotXMat * rotYMat * rotZMat);
}

void Camera::GetTransWVPMat()
{
	GetViewMat();
	GetRotMat(mRotXYZ);
	GetWorldMat();
	
	XMMATRIX tempWVPMat = XMLoadFloat4x4(&mWorldMat) *  XMLoadFloat4x4(&mViewMat) *  XMLoadFloat4x4(&mProjMat);
	XMStoreFloat4x4(&mWVPMat, XMMatrixTranspose(tempWVPMat));
}

void Camera::Reset()
{
	mEye = XMVectorSet(0.0f, 1.0f, -10.0f, 0.0f);
	mAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	mUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
}

void Camera::OnMouseDown(WPARAM btnState, int x, int y)
{
	mMousePos.x = x;
	mMousePos.y = y;
	SetCapture(mHwnd);
}

void Camera::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void Camera::OnMouseMove(USHORT btnFlag, int x, int y)
{
	mMousePos.x = x;
	mMousePos.y = y;

}

void Camera::OnMouseWheelRotate(short delta)
{
	float r = delta < 0 ? 0.7f : -0.7f;
	Settings::CameraRadius += r;
	Settings::CameraRadius = Clamp(Settings::CameraRadius, 1.0f, 50.0f);
}