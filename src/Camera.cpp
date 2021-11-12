#include "Camera.h"

Camera::Camera(HWND h, float aspectRatio, XMVECTOR cPos) :
	mEye(cPos),
	mFocus(XMVectorSet(0.0f, 200.0f, 0.0f, 0.0f))
{
	XMStoreFloat4x4(&mProjMat, XMMatrixPerspectiveFovLH(45.0f*(3.14f / 180.0f), aspectRatio, 0.1f, 1000.0f));
	mTarget = XMVector3Normalize(XMVectorSubtract(mFocus, mEye));
	mRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), mTarget));
	mUp = XMVector3Cross(mTarget, mRight);
	mMVPChanged = true;
}

void Camera::GetViewMat()
{
	XMStoreFloat4x4(&mViewMat, XMMatrixLookAtLH(mEye, mFocus, mUp));
}

XMMATRIX Camera::GetTransWVPMat()
{
	if (mMVPChanged) {
		/*GetViewMat();

		mWVPMat = XMLoadFloat4x4(&mViewMat) * XMLoadFloat4x4(&mProjMat);
		mMVPChanged = false;*/



		// Keep camera's axes orthogonal to each other and of unit length.
		mTarget = XMVector3Normalize(mTarget);
		mUp = XMVector3Normalize(XMVector3Cross(mTarget, mRight));
		mRight = XMVector3Cross(mUp, mTarget);

		// Fill in the view matrix entries.
		float x = -XMVectorGetX(XMVector3Dot(mEye, mRight));
		float y = -XMVectorGetX(XMVector3Dot(mEye, mUp));
		float z = -XMVectorGetX(XMVector3Dot(mEye, mTarget));

		XMFLOAT4X4 mView;
		mView(0, 0) = XMVectorGetX(mRight);
		mView(1, 0) = XMVectorGetY(mRight);
		mView(2, 0) = XMVectorGetZ(mRight);
		mView(3, 0) = x;

		mView(0, 1) = XMVectorGetX(mUp);
		mView(1, 1) = XMVectorGetY(mUp);
		mView(2, 1) = XMVectorGetZ(mUp);
		mView(3, 1) = y;

		mView(0, 2) = XMVectorGetX(mTarget);
		mView(1, 2) = XMVectorGetY(mTarget);
		mView(2, 2) = XMVectorGetZ(mTarget);
		mView(3, 2) = z;

		mView(0, 3) = 0.0f;
		mView(1, 3) = 0.0f;
		mView(2, 3) = 0.0f;
		mView(3, 3) = 1.0f;

		mWVPMat = XMLoadFloat4x4(&mView);
		mMVPChanged = false;
	}
	return mWVPMat;
}

void Camera::Walk(float t)
{
	XMVECTOR c = XMVectorReplicate(t);
	mEye = XMVectorMultiplyAdd(c, mTarget, mEye);
	mMVPChanged = true;
}

void Camera::Strafe(float t)
{
	XMVECTOR c = XMVectorReplicate(t);
	mEye = XMVectorMultiplyAdd(c, mRight, mEye);
	mMVPChanged = true;
}

void Camera::Pitch(float angle)
{
	XMMATRIX p = XMMatrixRotationAxis(mRight, angle);
	mUp = XMVector3TransformNormal(mUp, p);
	mTarget = XMVector3TransformNormal(mTarget, p);
	mMVPChanged = true;
}

void Camera::Yaw(float angle)
{
	XMMATRIX y = XMMatrixRotationY(angle);
	mRight = XMVector3TransformNormal(mRight, y);
	mUp = XMVector3TransformNormal(mUp, y);
	mTarget = XMVector3TransformNormal(mTarget, y);
	mMVPChanged = true;
}

void Camera::OnMouseMove(USHORT btnFlag, int x, int y)
{
	//mAt = XMVectorAdd(mAt, XMVectorSet(sin(atan(x / 20.0f)) * 2.0, y/40.f, 0.0f, 0.0f));
	Pitch(XMConvertToRadians(y * .25f));
	Yaw(XMConvertToRadians(x * .25f));
	//char str[20];	
	// mAt.m128_f32[0] += sin(atan(x / 20.0f)) * 2.0f;
	//mAt.m128_f32[1] += y/20.f;
	/*sprintf_s(str, "%f %f\n", mAt.m128_f32[0], t);
	OutputDebugStringA(str);*/
}

void Camera::OnMouseWheelRotate(short delta)
{
	//float r = delta < 0 ? 0.7f : -0.7f;
	//Settings::CameraRadius += r;
	//Settings::CameraRadius = Clamp(Settings::CameraRadius, 1.0f, 50.0f);

	mEye = XMVectorAdd(mEye, XMVectorReplicate(delta));
	mMVPChanged = true;
}