#pragma once
#include "EngineTypes.h"
#include "GameStates/GameState.h"
#include "GameStates/PlayState.h"
#include "GameStates/MainMenuState.h"
class GameState;

class GameStateMachine {
public:
	GameStateMachine(GameState*DefaultGameState = new GameState());
	~GameStateMachine() = default;

	void Start();
	//return current active game state  
	GameState* GetActiveGameState() const; 

	void Cleanup();

	void SetNewGameState(GameState* NewGameState = new GameState(), bool IsAdditive = false);

	void GarbageCollection();
	 
	void PreLoop();

	//detects input od the game 
	void ProcessInput(Input* GameInput);

	//update each frame after input 
	void Update(float DeltaTime);

	void Render(SDL_Renderer* Renderer);


private: 

	TArray<GameState*> m_ActiveGameStateStack; 
	TArray<GameState*> m_PandingGameStateStack;


};
