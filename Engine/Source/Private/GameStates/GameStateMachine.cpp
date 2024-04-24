#include "GameStates/GameStateMachine.h"

GameStateMachine::GameStateMachine(GameState* DefaultGameState)
{
	SetNewGameState(DefaultGameState);

}

GameState* GameStateMachine::GetActiveGameState() const
{
	return m_ActiveGameStateStack.back();
}

void GameStateMachine::Cleanup()
{
	for (const auto GS : m_PandingGameStateStack) {
		GS->Cleanup();
		delete GS;
	}

	for (const auto GS : m_ActiveGameStateStack) {
		GS->Cleanup();
		delete GS;

	}
}

void GameStateMachine::SetNewGameState(GameState* NewGameState, bool IsAdditive)
{
	if (IsAdditive) {
		return;
	}

	for (const auto GS : m_ActiveGameStateStack) {
		GS->DestroyGameState();
	}

	if (NewGameState != nullptr) {
		m_PandingGameStateStack.push_back(NewGameState);
	}

	

}

void GameStateMachine::GarbageCollection()
{

	GetActiveGameState()->GarbageCollection();
	if (m_ActiveGameStateStack.size() < 1) {
		return;
	
	}

	std::erase_if(m_ActiveGameStateStack,
		[](GameState* GS) {
			if (!GS->IsPendingDestroy()) {
				return false;
			}

			GS->Cleanup();
			delete GS;

			return true;
		}

	);
}


void GameStateMachine::PreLoop()
{
	for (const auto GS : m_PandingGameStateStack) {
		m_ActiveGameStateStack.push_back(GS);
		GS->Start();

	}
	m_PandingGameStateStack.clear();
	GetActiveGameState()->PreLoop();

}

void GameStateMachine::ProcessInput(Input* GameInput)
{
	GetActiveGameState()->ProcessInput(GameInput);
}

void GameStateMachine::Update(float DeltaTime)
{
	GetActiveGameState()->Update(DeltaTime);
}

void GameStateMachine::Render(SDL_Renderer* Renderer)
{
	GetActiveGameState()->Render(Renderer);
}
