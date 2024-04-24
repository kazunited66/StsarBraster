#include "Gamestates/MainMenuState.h"
#include "GameObjects/TextObject.h"
#include "Game.h"
#include "Input.h"
#include "GameStates/GameStateMachine.h"

void MainMenuState::OnStart()
{
	m_TitleText = AddGameObject<TextObject>();
	m_TitleText->SetAlignment(AL_CENTER);

	float HalfWidth = Game::GetGame()->WindowWidthF() / 2.0f;
	float HalfHeight = Game::GetGame()->WindowHeightF() / 2.0f;

	m_TitleText->SetPosition(Vector2(HalfWidth, HalfHeight));
	m_TitleText->SetFontSize(50);

	m_TitleText->SetText("Press Enter/Return To Start Game");


}

void MainMenuState::OnProcessInput(Input* GameInput)
{
	if (GameInput->IsKeyDown(EE_KEY_RETURN)) {
		auto NewSate = new PlayState();
		Game::GetGame()->GetGameStateMachine()->SetNewGameState(NewSate);

	}
}