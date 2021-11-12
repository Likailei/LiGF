#include "Camera.h"

Camera::Camera(HWND h, float aspectRatio, XMVECTOR cPos) :
	mHwnd(h),
	mEye(cPos),
	mAt(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)),
	mUp(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))
{
	XMStoreFloat4x4(&mProjMat, DirectX::XMMatrixPerspectiveFovLH(45.0f * (3.14f / 180.0f), aspectRatio, 0.1f, 1000.0f));
}

Camera::~Camera()
{
	mHwnd = nullptr;
}

void Camera::GetViewMat()
{
	float x = mRadius * sinf(mPhi) * cosf(mTheta);
	float z = mRadius * sinf(mPhi) * sinf(mTheta);
	float y = mRadius * cosf(mPhi);

	mEye = XMVectorSet(x, y, z, 1.0f);
	mAt = XMVectorZero();
	mUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMStoreFloat4x4(&mViewMat, XMMatrixLookAtLH(mEye, mAt, mUp));
}

void Camera::GetTransWVPMat()
{
	GetViewMat();

	XMMATRIX tempWVPMat = XMLoadFloat4x4(&mViewMat) * XMLoadFloat4x4(&mProjMat);
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
	/*float detaX = mMousePos.x - x;
	float detaY = mMousePos.y - y;*/
	// Make each pixel correspond to a quarter of a degree.
	float dx = XMConvertToRadians(0.025f * static_cast<float>(-x));
	float dy = XMConvertToRadians(0.025f * static_cast<float>(-y));

	// Update angles based on input to orbit camera around box.
	mTheta += dx;
	mPhi += dy;

	// Restrict the angle mPhi.
	mPhi = Clamp(mPhi, 0.1f, XM_PI - 0.1f);

	mMousePos.x = x;
	mMousePos.y = y;

}

void Camera::OnMouseWheelRotate(short delta)
{
	float r = delta < 0 ? 0.7f : -0.7f;
	mRadius += r;
	mRadius = Clamp(mRadius, 1.0f, 50.0f);
}