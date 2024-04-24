#pragma once
#include<EngineTypes.h>
//forward decleation
struct SDL_Window;
struct SDL_Renderer;
class Texture;
class Input;
class GameStateMachine;
class Text;
class WinMenu;




class Game {
public:
	static Game* GetGame();

	static void DestroyGame();

	//Run the Game
	void Run()  {Initialise(); }
	//Exit APP 
	void QuitApp() { m_IsGameOpen = false; }

	//Import a texture to the game 
	Texture* ImportTexture(const char* PathToFile);

	//safe destory the texture
	void DestroyTexture(Texture* TextureToDestroy);

	Text* ImportText(const char* PathToFiile);

	void DestroyText(Text* TextToDestroy);

	GameStateMachine* GetGameStateMachine() const { return m_GameStateMachine; }
	
	int WindowWidth() const;

	int WindowHeight() const;

	float WindowWidthF() const;

	float WindowHeightF() const;

	void RestartGame();

	WinMenu* GetWinMenu() const { return m_WinMenu;  }



private:
	//Constructor runs when the class is created
	//You also need it to create a new object 
	Game();

	// Destructor runs when the class is destroyed 

	~Game();

	//Core game Functions 
	//Initilise the dependecies/external libraries
	//This will exit the game if any fail
	void Initialise();

	//This will run the post-initialise function that rely on dependencies                                                                                                
	//Load the window and any start game functions 
	void Start();

	// Run the looop of the game until the app close 
	void GameLoop();



	// Game Loop 

	//check if any object need to be spwned 
	void PreLoop();

	void Cleanup();

	//Listen for user inout and process it 
	void ProcessInput();


	//Update the game logic based on player inputs
	//No rendering here, logic should update first then we display the results 
	void Update();


	//Display the results and render graphics  to the screen based on the logic 
	void Render();

	//Any object that are marked for delete will be deallocated from memory here 
	void CollectGarbage();



	private :
		// flag that decides when the game loop ends 
		bool m_IsGameOpen;


		//stores the window for the app/game
		SDL_Window* m_WindowRef;
		//stores the renderer for the sdl window
		SDL_Renderer* m_RendererRef;

		//store all of the texture in the game 
		TArray<Texture*> m_TextureStack;
		 
		TArray<Text*> m_TextStack;

		//store the input for the game 
		Input* m_GameInput; 

		GameStateMachine* m_GameStateMachine;

		WinMenu* m_WinMenu;
	
};
