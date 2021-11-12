#include "Settings.h"

bool Settings::WireFrameMode = false;
float Settings::WalkSpeed = .8f;


float Settings::CameraRadius = 2.0f;
float Settings::CameraTheta = 1.5f * XM_PI;
float Settings::CameraPhi = XM_PIDIV4;
float Settings::CameraSpeed = 15.0f;

void Settings::ChangeMode()
{
	WireFrameMode = !WireFrameMode;
}