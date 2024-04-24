#include "Game.h"
#include <iostream>
#include "SDL2/SDL.h"
#include "Debug.h"
#include "Graphics/Texture.h"
#include <vector>
#include "Input.h"
#include"GameStates/GameStateMachine.h"
#include"SDL2/SDL_ttf.h"
#include "Graphics/Text.h"
#include "Graphics/Texture.h"
#include "SDL2/SDL_syswm.h"
#include "Menus/WinMenu.h"

using namespace std;
Game* Game::GetGame()
{

	//static - only run initilisation once
	// that means this line will only run once
	//this is completely thread safe 
	static Game* Gamesinglton = new Game();
	return Gamesinglton;
}

void Game::DestroyGame()
{
	delete Game::GetGame();
}

Texture* Game::ImportTexture(const char* PathToFile)
{
	Texture* NewTexture = new Texture(m_RendererRef);

	// loop through all of the texture in the game 
	for (Texture* TexRef : m_TextureStack) {
		//check if the texture has already been imported 
		if (strcmp(TexRef->GetPath(), PathToFile) == 0) {
			// if there was a matching path 
			//Copy the successfully mached element 
			NewTexture->CopyTexture(TexRef);
			//add it to the texture stack
			m_TextureStack.push_back(NewTexture);
			//return the new texture here to ignore the rest of the fnction 
			return NewTexture;
		}
	}

	//attempt to import the texture 
	if (!NewTexture->ImportTexture(PathToFile)) {
		//if it failed then delete and update new texture to nullptr 
		delete NewTexture;
		NewTexture = nullptr;
	}
	else {
		m_TextureStack.push_back(NewTexture);
	}
	return NewTexture;
}

void Game::DestroyTexture(Texture* TextureToDestroy)
{
	int TexturesFound = 0;
	//loop through all of the textures 
	for (Texture* TexRef : m_TextureStack) {
		//if the texture has a matching path
		if (strcmp(TextureToDestroy->GetPath(), TexRef->GetPath()) == 0) {
			++TexturesFound;

			if (TexturesFound > 1) {
				break;
			}

		}
	}

	if (TexturesFound <= 1) {
		TextureToDestroy->Cleanup();
	}

	auto it = find(m_TextureStack.begin(), m_TextureStack.end(), TextureToDestroy);

	// if we find the teture 
	if (it != m_TextureStack.end()) {
		m_TextureStack.erase(it);
	}
	

	delete TextureToDestroy;
	TextureToDestroy = nullptr;

	EE_LOG("Game", "Texture has been destroyed.");
}

Text* Game::ImportText(const char* PathToFiile)
{
	Text* NewText = new Text(m_RendererRef);

	if (!NewText->ImportTexture(PathToFiile)) {
		delete NewText;
		return nullptr;
	}

	m_TextStack.push_back(NewText);
	return NewText;
}

void Game::DestroyText(Text* TextToDestroy)
{

    std:erase_if(m_TextStack, [TextToDestroy](const auto Item) {return Item == TextToDestroy;  });
	TextToDestroy->Cleanup();
	delete TextToDestroy; 
}

int Game::WindowWidth() const
{
	int w = 0;
	SDL_GetWindowSize(m_WindowRef, &w, NULL);
	return w;
}

int Game::WindowHeight() const
{
	int h = 0;
	SDL_GetWindowSize(m_WindowRef, NULL, &h);
	return h;
}

float Game::WindowWidthF() const
{
	return static_cast<float>(WindowWidth());
}

float Game::WindowHeightF() const
{
	return static_cast<float>(WindowHeight());
}

void Game::RestartGame()
{
	auto NewState = new MainMenuState();
	GetGameStateMachine()->SetNewGameState(NewState);
}


Game::Game()
{
	printf("Game created.\n");
	m_IsGameOpen = true;
	m_WindowRef = nullptr;
	m_RendererRef = nullptr;
	m_GameStateMachine = nullptr;
	m_WinMenu = nullptr;
	

}

Game::~Game()
{
	printf("Game destroyed.\n");


}

void Game::Initialise()
{

	//Initialise SDL and the game if it fail
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		EE_LOG("Game", "SDL failed to init: " << SDL_GetError());
		return;
	}

	if (TTF_Init() == -1) {
		EE_LOG("Game", "TTF failed to init: " << TTF_GetError());
		Cleanup();
		return;
	}

	EE_LOG("Game", "Game successfully initialised all libraries");

	Start();
}

void Game::Start()
{
	m_WindowRef = SDL_CreateWindow("Ember Eclipse", // Tittle of window
		SDL_WINDOWPOS_CENTERED_DISPLAY(1), SDL_WINDOWPOS_CENTERED_DISPLAY(1),//start position on the window 
		1280, 720,//resolution of the window
		0
	//		SDL_WINDOW_POPUP_MENU
	); //special flags to change the window settings

#if 1	// メニューの追加 https://stackoverflow.com/questions/51250046/sdl2-win32-api-menubar-click-event-not-working

	SDL_SysWMinfo infoWindow;
	SDL_VERSION(&infoWindow.version);
	SDL_GetWindowWMInfo(m_WindowRef, &infoWindow);
	HWND hwnd = infoWindow.info.win.window;

	#define ID_LOADROM 1
	#define ID_ABOUT 2
	#define ID_EXIT 3
	#define ID_CONTROLS 4
	static HMENU hHelp;
	static HMENU hEdit;
	static HMENU hFile;
	static HMENU hMenuBar;

	hMenuBar = CreateMenu();
	hFile = CreateMenu();
	hEdit = CreateMenu();
	hHelp = CreateMenu();

	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFile, TEXT("File"));
//	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hEdit, TEXT("Edit"));
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hEdit, TEXT("Game"));
//	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hHelp, TEXT("Help"));
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hHelp, TEXT("About"));

//	AppendMenu(hFile, MF_STRING, ID_LOADROM, TEXT("Load ROM"));
//	AppendMenu(hFile, MF_STRING, ID_EXIT, TEXT("Exit"));

//	AppendMenu(hEdit, MF_STRING, ID_CONTROLS, TEXT("Configure Controls"));

//	AppendMenu(hHelp, MF_STRING, ID_ABOUT, TEXT("About"));

	SetMenu(hwnd, hMenuBar);

#endif


	//did the window successfully create 
	if (m_WindowRef == nullptr) {
		EE_LOG("Game", "SDL window failed to create: " << SDL_GetError());

		//deallocate all the stuff that has been allocated prior to this point 
		Cleanup();
		return;
	}

	//create randerer and check if it failed 
	m_RendererRef = SDL_CreateRenderer(m_WindowRef, -1, 0);


	//did the rendere fail
	if (m_RendererRef == nullptr) {
		EE_LOG("Game", "SDL renderer failed to create: " << SDL_GetError());

		Cleanup();
		return;
	}

	//create the game input 

	m_GameInput = new Input();

	m_WinMenu = new WinMenu(m_WindowRef);

	if (!m_WinMenu->InitialiseMenu()) {
		Cleanup();
		return;
	}

	auto Default = new MainMenuState();
	m_GameStateMachine = new GameStateMachine(Default);
 
	GameLoop();
}

void Game::GameLoop()
{
	while (m_IsGameOpen) {
		PreLoop();

		ProcessInput();
		
		Update();

		Render();

		CollectGarbage();

	}

	Cleanup();
}

void Game::Cleanup()
{
	//run the cleanup 
	m_GameStateMachine->Cleanup();

	for (const auto Item : m_TextStack) {
		Item->Cleanup();
		delete Item;
	}

	// cleanup and remove all textures from the texture stack 
	for (int i = m_TextureStack.size() - 1; i > -1; --i) {
		DestroyTexture(m_TextureStack[i]);
	}

	if (m_WinMenu != nullptr) {
		delete m_WinMenu;

	}

	//does the renderer exist
	if (m_RendererRef != nullptr) {
		//deallocate the render from memory
		SDL_DestroyRenderer(m_RendererRef);
	}
	//does the window exist
	if (m_WindowRef != nullptr) {
		//deallocate the window from memory
		SDL_DestroyWindow(m_WindowRef);
	}
	TTF_Quit();
	SDL_Quit();

	EE_LOG("Game", "Gamer has dewallocated all memory");
}

void Game::PreLoop()
{
	//process the input for the game 
	m_GameStateMachine->PreLoop();
}



void Game::ProcessInput()
{
	//process the input foe the game
	m_GameInput->ProcessInput();

	m_GameStateMachine->ProcessInput(m_GameInput);

}

void Game::Update()
{
	//recode the previous frame time 
	static double LastTickTime = 0.0;
	//recode the current frame time 
	double CurrentTickTime = (double)SDL_GetTicks64();
	//get the delta time - how much time has passed since the last frame
	double LongDelta = CurrentTickTime - LastTickTime;
	//convert from milliseconds 
	double DeltaTime = LongDelta / 1000.0;
	// set the last tick time 
	LastTickTime = CurrentTickTime;

	//run the active game state update
	m_GameStateMachine->Update(static_cast<float>(DeltaTime));
	
	//caps the frame rate 
	int FrameDuration = 1000 / 240;

	if ((double)FrameDuration > LongDelta) {
		FrameDuration = (int)LongDelta;
	}
	// is the frame rate is greater than 240 delay the frame 
	SDL_Delay((Uint32)FrameDuration);
}

void Game::Render()
{
	
	SDL_SetRenderDrawColor(m_RendererRef, 50, 50, 50, 255);
	//User the color just started to clear the previous frame and fill in with that colour 
	SDL_RenderClear(m_RendererRef);

	//TODO: Render custom graphics
	for(const auto TexRef : m_TextureStack) {
		if (TexRef != nullptr) {
			TexRef->Draw();
		}
	}
	
	for (const auto Item : m_TextStack) {
		if (Item != nullptr) {
			Item->Draw();

		}
	}
	m_GameStateMachine->Render(m_RendererRef);

	
	//Present the graphics to the renderer 
	SDL_RenderPresent(m_RendererRef);
}

void Game::CollectGarbage()
{
	m_GameStateMachine->GarbageCollection();
}
