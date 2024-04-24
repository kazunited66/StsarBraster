#include"Input.h"
#include"SDL2/SDL.h"
#include"Game.h"
#include"Math/Vector2.h"
#include"SDL2/SDL_syswm.h"
#include"Menus/WinMenu.h"
#include"resource.h"

void Input::ProcessInput()
{
	//Data type that reads the SDL input events foe the windows
	SDL_Event InputEvent;

	//run through each input uin that frame
	while (SDL_PollEvent(&InputEvent)) {

		if (InputEvent.type == SDL_MOUSEBUTTONDOWN) {
			DetectMouseButtonState(InputEvent.button.button, true);

		}

		if (InputEvent.type == SDL_MOUSEBUTTONUP) {
			DetectMouseButtonState(InputEvent.button.button, false);
		}
		//if the cross button is pressed on the window, close the app
		if (InputEvent.type == SDL_QUIT) {
			Game::GetGame()->QuitApp();
		}

		if (InputEvent.type == SDL_SYSWMEVENT) {
			HandleWinMenuEvents(&InputEvent);
		}
	}
}

bool Input::IsKeyDown(EE_Key Key) {
	const Uint8* KeyStates = SDL_GetKeyboardState(NULL);

	if (KeyStates == nullptr) {
		return false;
	}

		return KeyStates[Key]; 
}

bool Input::IsMouseButtonDown(EE_MouseButton Button)
{
	if (Button >= EE_NUM_BUTTONS) {
		return false; 
	}

	return m_MouseState[Button];
}

void Input::DetectMouseButtonState(unsigned int Event, bool Value)
{
	switch (Event) {
	case SDL_BUTTON_LEFT :
		m_MouseState[EE_MOUSE_LEFT] = Value; 
		break;
	case SDL_BUTTON_MIDDLE :
		m_MouseState[EE_MOUSE_MIDDLE] = Value;
		break;
	case SDL_BUTTON_RIGHT :
		m_MouseState[EE_MOUSE_RIGHT] = Value;
		break;

	}
}

void Input::HandleWinMenuEvents(SDL_Event* Event)
{
	switch (Event->syswm.msg->msg.win.wParam) {
	case ID_FILE_EXITAPP :
	Game::GetGame()->QuitApp();
		break;
	case ID_FILE_RESTARTGAME : 
		Game::GetGame()->RestartGame();
		break;

	case ID_GAME_GAMECONTROL:
		Game::GetGame()->GetWinMenu()->ActivatePopup(
			"Game Controls",
			"WASD - Move Ship\nSpace - Fire Weapon"
		);

		break;

	case ID_ABOUT_ABOUTGAME :
		Game::GetGame()->GetWinMenu()->ActivatePopup(
			"Enter Eclips Game",
			"Enter Eclips Game is an SDL2-based C++ 2D Game engine creaated by Kazunori Masuda in 2024"

		);
		break;

	default:
		break;
	}
}

Vector2 Input::GetMousePos() const
{
	int x(0);
	int y(0);

	SDL_GetMouseState(&x, &y);

	return Vector2((float)x,(float)y);
}
