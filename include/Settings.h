#pragma once
#ifndef _SETTINGS_H_

#include "stdafx.h"

class Settings {
public:
	static bool WireFrameMode;

	static float CameraRadius;
	static float CameraTheta;
	static float CameraPhi;

	static void ChangeMode();
	static void OnWheelRotate(short delta);
	static void OnMouseMove(USHORT btnFlag, int x, int y);
};
#endif // !_SETTINGS_H_
