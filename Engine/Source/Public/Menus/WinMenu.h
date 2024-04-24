#pragma once

#include "SDL2/SDL_syswm.h"

class WinMenu {
public: 
	WinMenu(SDL_Window* Window);
	~WinMenu() = default;


	//create a menu and add it to the SDL window 
	bool InitialiseMenu();


	void ActivatePopup(const char* Title, const char* Msg);

private: 
	SDL_Window* m_Window;
    
	SDL_SysWMinfo m_WindowInfo;


};