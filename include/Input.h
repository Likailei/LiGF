#pragma once
#ifndef _INPUT_H_
#define _INPUT_H_

#include "stdafx.h"
#include "Settings.h"

class Input
{
public:
	Input();
	~Input();
	
	struct MouseFlags {
		bool MOUSE_LBUTTON_DOWN = false;
		bool MOUSE_LBUTTON_UP = false;
	};

	MouseFlags m_mouseFlags;

	RAWINPUT* const GetRawInput();
	void DispatchInput(LPARAM lParam);
private:
	RAWINPUTDEVICE m_rawInputDevices[2];
	LPBYTE m_rawInput;

	RAWINPUT* ParseInputData(LPARAM lParam);
};

#endif // !_INPUT_H_