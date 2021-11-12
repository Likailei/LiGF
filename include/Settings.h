#pragma once
#ifndef _SETTINGS_H_

#include "stdafx.h"

class Settings {
public:
	static bool WireFrameMode;
	static float WalkSpeed;



	static float CameraRadius;
	static float CameraTheta;
	static float CameraPhi;
	static float CameraSpeed;

	static void ChangeMode();
};
#endif // !_SETTINGS_H_
