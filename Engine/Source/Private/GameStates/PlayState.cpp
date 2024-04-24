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
//	Player* P = AddGameObject<Player>();
//	P->SetPosition({ Game::GetGame()->WindowWidth()/ 2.0f, Game::GetGame()->WindowHeightF() - P->ScaleHalfSize()});
	m_Player->SetPosition({ Game::GetGame()->WindowWidth()/ 2.0f, Game::GetGame()->WindowHeightF() - m_Player->ScaleHalfSize()});


#if 0	// Test
	// Playerの真上にミサイルを表示させてみる
	 
	// テスト的にEnemyクラスでやってみる
	Missile* E = AddGameObject<Missile>();
	// テスト的にPlayerの真上に出す
//	M->SetPosition({ P->GetTransform().Position.x, P->GetTransform().Position.y - P->ScaleSize()});
//	E->SetPosition({ P->GetTransform().Position.x, P->GetTransform().Position.y - 100});
	Vector2 EPos = { P->GetTransform().Position.x, P->GetTransform().Position.y - P->ScaleSize() };
//	E->SetPosition({ P->GetTransform().Position.x, P->GetTransform().Position.y - P->ScaleSize() });
	E->SetPosition(EPos);

#endif

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
	
}

void PlayState::UpdateScore()
{
	if(m_Missile){
		if (m_Missile->m_hitEnemy == true) {
			m_Score += 10;
			// Stop counting when the upper limit is reached
			if (m_Score >= 99990) {
				m_Score = 99990;		
			}
			m_Missile->m_hitEnemy = false;
		}
	}
//	std::string scoreString = "Score: " + std::to_string(0);
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
		E->SetPosition({ PosX, -E->ScaleHalfSize()});

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
			//			Missile* M = AddGameObject<Missile>();

			// Spawns a missile above the player. Add 1 above to avoid duplication.
			Vector2 MPos = { m_Player->GetTransform().Position.x, m_Player->GetTransform().Position.y - m_Player->ScaleSize() - 1};
//			M->SetPosition(MPos);
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
