#include "Input.h"

Input::Input(HWND h, ThirdPersonCamera* c): m_hwnd(h),m_camera(c)
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

RAWINPUT* Input::ParseInputData(LPARAM lParam)
{
	UINT size{ 0 };
	GetRawInputData((HRAWINPUT)(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
	if (m_rawInput) delete m_rawInput;
	m_rawInput = new BYTE[size];
	GetRawInputData((HRAWINPUT)(lParam), RID_INPUT, m_rawInput, &size, sizeof(RAWINPUTHEADER));

	//return reinterpret_cast<RAWINPUT*>(m_rawInput)->header.dwType;
	return reinterpret_cast<RAWINPUT*>(m_rawInput);
}

RAWINPUT* const Input::GetRawInput()
{
	return reinterpret_cast<RAWINPUT*>(m_rawInput);
}

void Input::DispatchInput(LPARAM lParam)
{
    char str[200];
    RAWINPUT* raw = ParseInputData(lParam);
    switch (raw->header.dwType)
    {
    case RIM_TYPEKEYBOARD:
        /*sprintf_s(str, "Kbd: make=%04x Flags:%04x Reserved:%04x ExtraInformation:%08x, msg=%04x VK=%04x \n",
            raw->data.keyboard.MakeCode,
            raw->data.keyboard.Flags,
            raw->data.keyboard.Reserved,
            raw->data.keyboard.ExtraInformation,
            raw->data.keyboard.Message,
            raw->data.keyboard.VKey);*/
        //OutputDebugStringA(str);
        if (raw->data.keyboard.Flags == RI_KEY_MAKE) {
            switch (raw->data.keyboard.VKey)
            {
            case VK_F3:
                Settings::ChangeMode();
                break;
            case 0x57:
                m_camera->Walk(Settings::WalkSpeed);
                break;
            case 0x53:
                m_camera->Walk(-Settings::WalkSpeed);
                break;
            case 0x41:
                m_camera->Strafe(-Settings::WalkSpeed);
                break;
            case 0x44:
                m_camera->Strafe(Settings::WalkSpeed);
                break;
            default:
                break;
            }
        }
        break;

    case RIM_TYPEMOUSE:
        /*sprintf_s(str, "Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%d ulRawButtons=%04x lLastX=%ld lLastY=%ld ulExtraInformation=%04x\r\n",
            raw->data.mouse.usFlags,
            raw->data.mouse.ulButtons,
            raw->data.mouse.usButtonFlags,
            (short)raw->data.mouse.usButtonData,
            raw->data.mouse.ulRawButtons,
            (long)raw->data.mouse.lLastX,
            (long)raw->data.mouse.lLastY,
            raw->data.mouse.ulExtraInformation);
        OutputDebugStringA(str);*/

        if (raw->data.mouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_DOWN) m_mouseFlags.MOUSE_LBUTTON_DOWN = true;
        if (raw->data.mouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_UP) m_mouseFlags.MOUSE_LBUTTON_DOWN = false;
        if (raw->data.mouse.usButtonFlags == RI_MOUSE_WHEEL) {
            short delta = raw->data.mouse.usButtonData;
            m_camera->Walk(delta*.2f);
        }
        if (m_mouseFlags.MOUSE_LBUTTON_DOWN) {
            //SetCapture(m_hwnd);
            m_camera->Shake((float)raw->data.mouse.lLastX, (float)raw->data.mouse.lLastY);
        }
        if (m_mouseFlags.MOUSE_LBUTTON_UP) {
            //ReleaseCapture();
        }
        break;
    }
}
