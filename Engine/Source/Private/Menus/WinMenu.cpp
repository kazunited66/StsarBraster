#include "Menus/WinMenu.h"


#include <Windows.h>
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_messagebox.h"
#include "resource.h"
#include "Debug.h"

WinMenu::WinMenu(SDL_Window* Window)
{
	m_Window = Window;

	SDL_VERSION(&m_WindowInfo.version);

	SDL_GetWindowWMInfo(m_Window, &m_WindowInfo);
}

bool WinMenu::InitialiseMenu()
{
	HWND SysWMInfo = m_WindowInfo.info.win.window;


	HINSTANCE WindowInstance = GetModuleHandle(0);

	LPCWSTR VSMenuStr = MAKEINTRESOURCE(IDR_MENU1);

	HMENU SysWMenu = LoadMenu(WindowInstance, VSMenuStr);

	if (!SetMenu(SysWMInfo, SysWMenu)) {
		EE_LOG("WinMenu", "Windows could not assign menu.");
		return false;

	}

	SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);

	return true;
}

void WinMenu::ActivatePopup(const char* Title, const char* Msg)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, Title, Msg, m_Window);
}
