#pragma once
#ifndef _INPUT_H_
#define _INPUT_H_

#include "stdafx.h"

class Input
{
public:
	Input();
	~Input();

	UINT ParseInputData(LPARAM lParam);
	RAWINPUT* const GetRawInput();
private:
	RAWINPUTDEVICE m_rawInputDevices[2];
	LPBYTE m_rawInput;
};

#endif // !_INPUT_H_