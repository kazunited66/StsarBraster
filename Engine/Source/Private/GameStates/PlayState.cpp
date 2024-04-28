#include "GameStates/PlayState.h"
#include"GameObjects/Enemy.h"
#include"GameObjects/Missile.h"
#include"GameObjects/Player.h"	
#include "Game.h"
#include "GameStates/GameStateMachine.h"
#include"Input.h"
#include"GameObjects/TextObject.h"



#include<string>
#include<iomanip>
#include<random>
#include<sstream>

#include "Debug.h"

#define Super GameState 

std::default_random_engine RandGenerator;

PlayState::PlayState() :
	m_ScoreText(nullptr),
	m_Score(0),
	m_FreqText(nullptr),
	//m_GameOverText(nullptr),
	m_Player(nullptr),
	m_Missile(nullptr),
	m_EnemyFrequency(3.0f),
	m_EnemySpawnTimer(5.0f),
	m_MissileSpawnTimer(0.0f)
{}

void PlayState::OnStart()
{
	Super::OnStart();

	m_Player = AddGameObject<Player>();
	m_Player->SetPosition({ Game::GetGame()->WindowWidth() / 2.0f, Game::GetGame()->WindowHeightF() - m_Player->ScaleHalfSize() });

	m_ScoreText = AddGameObject<TextObject>();
	m_ScoreText->SetPosition({ 25.0f, 25.0f });
	m_ScoreText->SetFontSize(50);
	m_ScoreText->SetAlignment(AL_TOP_LEFT);
	UpdateScore();

	m_FreqText = AddGameObject<TextObject>();
	m_FreqText->SetPosition({ 25.0f, 70.0f });
	m_FreqText->SetFontSize(25);
	m_FreqText->SetAlignment(AL_TOP_LEFT);
	UpdateFrequencyText();

	RandGenerator.seed(time(nullptr));
}


void PlayState::OnUpdate(float DeltaTime)
{
	Super::OnUpdate(DeltaTime);

	EnemySpawner(DeltaTime);

	MissileSpawner(DeltaTime);

	UpdateScore();
	UpdateFrequencyText();

	//	CheckGameOver();
}

/*
void PlayState::OnProcessInput(Input* GameInput)
{
	if (GameInput->IsKeyDown(EE_KEY_RETURN)) {
		if (Game::GetGame()->GetGameOver()) {
			Game::GetGame()->SetRestart();
		}
	}
}
*/

void PlayState::UpdateScore()
{
	m_Score = Game::GetGame()->GetHitCount() * 10;
	std::string scoreString = "Score: " + std::to_string(m_Score);
	m_ScoreText->SetText(scoreString.c_str());
}

void PlayState::UpdateFrequencyText()
{
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << m_EnemyFrequency;
	std::string freqString = "Enemy Spawn Time: " + stream.str();

	m_FreqText->SetText(freqString.c_str());
}

void PlayState::EnemySpawner(float DeltaTime)
{
	m_EnemySpawnTimer -= DeltaTime;

	if (m_EnemySpawnTimer <= 0.0f) {
		Enemy* E = AddGameObject<Enemy>();

		float PosX = GetRandomFloatRange(E->ScaleHalfSize(), Game::GetGame()->WindowWidthF() - E->ScaleHalfSize());
		E->SetPosition({ PosX, -E->ScaleHalfSize() });

		m_EnemySpawnTimer = m_EnemyFrequency;

		m_EnemyFrequency = std::max(0.1f, m_EnemyFrequency - 0.1f);

		UpdateFrequencyText();
	}
}

void PlayState::MissileSpawner(float DeltaTime)
{
	m_MissileSpawnTimer -= DeltaTime;

	if (m_Player->m_IsSpaceKeyPressed == true) {

		// Prevents firing until a predetermined time has elapsed
		if (m_MissileSpawnTimer <= 0.0f) {
			m_Missile = AddGameObject<Missile>();
			Vector2 MPos = { m_Player->GetTransform().Position.x, m_Player->GetTransform().Position.y - m_Player->ScaleSize() - 1 };
			m_Missile->SetPosition(MPos);
			m_MissileSpawnTimer = 0.5f;
		}
	}
	m_Player->m_IsSpaceKeyPressed = false;
}

float PlayState::GetRandomFloatRange(float min, float max) const
{
	std::uniform_real_distribution<float> RandNum(min, max);

	return RandNum(RandGenerator);
}

/*
void PlayState::CheckGameOver()
{
	if (Game::GetGame()->GetGameOver()) {
		m_GameOverText = AddGameObject<TextObject>();
		m_GameOverText->SetPosition({ 200.0f, 250.0f });
		m_GameOverText->SetFontSize(50);
		m_GameOverText->SetAlignment(AL_TOP_LEFT);
//		std::string gameoverString = "Game Over";
		std::string gameoverString = "Game Over! Press Enter/Return To Restart Game";
		m_GameOverText->SetText(gameoverString.c_str());
		m_GameOverText->Update(0);
	}

}
*/