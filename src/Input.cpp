#include "Input.h"

Input::Input()
{
	m_rawInputDevices[0].usUsagePage = 0x01;
	m_rawInputDevices[0].usUsage = 0x02;
	m_rawInputDevices[0].dwFlags = 0;
	m_rawInputDevices[0].hwndTarget = 0;

	m_rawInputDevices[1].usUsagePage = 0x01;
	m_rawInputDevices[1].usUsage = 0x06;
	m_rawInputDevices[1].dwFlags = 0;
	m_rawInputDevices[1].hwndTarget = 0;

	//m_rawInput = reinterpret_cast<RAWINPUT*>(new UINT8[256]);
	//m_rawInput = new BYTE[48];
	ThrowIfFailed(RegisterRawInputDevices(m_rawInputDevices, 2, sizeof(m_rawInputDevices[0])));
}

Input::~Input()
{
	delete m_rawInput;
}

UINT Input::ParseInputData(LPARAM lParam)
{
	UINT size{ 0 };
	GetRawInputData((HRAWINPUT)(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
	if (m_rawInput) delete m_rawInput;
	m_rawInput = new BYTE[size];
	GetRawInputData((HRAWINPUT)(lParam), RID_INPUT, m_rawInput, &size, sizeof(RAWINPUTHEADER));

	return reinterpret_cast<RAWINPUT*>(m_rawInput)->header.dwType;
}

RAWINPUT* const Input::GetRawInput()
{
	return reinterpret_cast<RAWINPUT*>(m_rawInput);
}
