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
#include "GameObjects/TextObject.h"
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
	m_IsGameStart = false;
	m_IsGameOver = false;
	m_IsGameRestart = false;
	m_WindowRef = nullptr;
	m_RendererRef = nullptr;
	m_GameStateMachine = nullptr;
	m_MainMenuText = nullptr;
	m_GameOverText = nullptr;
	m_HitCount = 0;
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
	); //special flags to change the window settings

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
	};

	GameState* Default = new PlayState();
	auto mainMenuState = new MainMenuState();
	m_GameStateMachine = new GameStateMachine(Default);

	GameLoop();
}

void Game::GameLoop()
{
	// Display Main Menu
	DispMainMenu();

	// Wait for start or quit
	while (!m_IsGameStart && m_IsGameOpen) {
		CheckStart();
	}

	while (m_IsGameOpen) {

		CheckReStart();

		PreLoop();

		ProcessInput();

		CheckGameOver();

		if (!GetGameOver()) {	// Screen does not update when game over
			Update();
		}

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
	//recode the current frame time 
	double CurrentTickTime = (double)SDL_GetTicks64();

	//get the delta time - how much time has passed since the last frame
	double LongDelta = CurrentTickTime - m_LastTickTime;

	//convert from milliseconds 
	double DeltaTime = LongDelta / 1000.0;

	// set the last tick time 
	m_LastTickTime = CurrentTickTime;

	// for debug
//	printf("[DBG] DeltaTime=%f\n", DeltaTime);


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
	for (const auto TexRef : m_TextureStack) {
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

void Game::DispMainMenu()
{
	// create text object
	m_MainMenuText = new TextObject();
	m_MainMenuText->SetPosition({ 150.0f, 300.0f });
	m_MainMenuText->SetFontSize(60);
	m_MainMenuText->SetAlignment(AL_TOP_LEFT);
	std::string mainMenuString = "Press Enter/Return To Start Game";
	m_MainMenuText->SetText(mainMenuString.c_str());
	m_MainMenuText->Update(0);

	// render
	for (const auto Item : m_TextStack) {
		if (Item != nullptr) {
			Item->Draw();
		}
	}
	SDL_RenderPresent(m_RendererRef);
}

void Game::CheckStart()
{
	// check cross button is pressed on the window
	m_GameInput->ProcessInput();

	if (m_GameInput->IsKeyDown(EE_KEY_RETURN)) {
		m_IsGameStart = true;
		m_MainMenuText->SetText("");
		m_TextStack.clear();
		delete m_MainMenuText;
	}
}

void Game::CheckGameOver()
{
	if (GetGameOver()) {
		if (m_GameOverText == nullptr) {
			m_GameOverText = new TextObject();
			m_GameOverText->SetPosition({ 120.0f, 320.0f });
			m_GameOverText->SetFontSize(40);
			m_GameOverText->SetAlignment(AL_TOP_LEFT);
			std::string gemeoverString = "Game Over!   Press Enter/Return To Restart Game";
			m_GameOverText->SetText(gemeoverString.c_str());
			m_GameOverText->Update(0);
		}
	}
}

void Game::CheckReStart()
{
	if (GetGameOver()) {
		if (m_GameInput->IsKeyDown(EE_KEY_RETURN)) {
			SetRestart(true);
		}
	}

	if (GetRestart() == true) {
		SetGameOver(false);
		SetRestart(false);

		m_GameOverText->SetText("");

		ClearHitCount();

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

		delete m_GameOverText;
		m_GameOverText = nullptr;
		m_TextStack.clear();

		delete m_GameStateMachine;
		GameState* Default = new PlayState();
		m_GameStateMachine = new GameStateMachine(Default);

	}
}
void Game::SetLastTickTimeNow()
{
	m_LastTickTime = (double)SDL_GetTicks64();
}