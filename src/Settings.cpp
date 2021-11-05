#include "Settings.h"

bool Settings::WireFrameMode = false;
float Settings::CameraRadius = 2.0f;
float Settings::CameraTheta = 1.5f * XM_PI;
float Settings::CameraPhi = XM_PIDIV4;

void Settings::ChangeMode()
{
	WireFrameMode = !WireFrameMode;
}

void Settings::OnWheelRotate(short delta)
{
	float r = delta < 0 ? 0.7f : -0.7f;
	Settings::CameraRadius += r;
	Settings::CameraRadius = Clamp(Settings::CameraRadius, 1.0f, 50.0f);
}

void Settings::OnMouseMove(USHORT btnFlag, int x, int y)
{
	float dx = XMConvertToRadians(0.025f * static_cast<float>(-x));
	float dy = XMConvertToRadians(0.025f * static_cast<float>(-y));

	Settings::CameraTheta += dx;
	Settings::CameraPhi += dy;

	Settings::CameraPhi = Clamp(Settings::CameraPhi, 0.1f, XM_PI - 0.1f);
}
