#pragma once
#include "GameState.h"
#include"GameObjects/Player.h"	
#include"GameObjects/Missile.h"	
#include"GameObjects/Enemy.h"	

class TextObject;

class PlayState : public GameState {
public:
	PlayState();

protected:
	virtual void OnStart() override;

	virtual void OnUpdate(float DeltaTime) override;

	//	virtual void OnProcessInput(Input* GameInput) override;

	void EnemySpawner(float DeltaTime);

	void MissileSpawner(float DeltaTime);

	float GetRandomFloatRange(float min = 0.0f, float max = 1.0f) const;

	void UpdateScore();
	void UpdateFrequencyText();

	//	void CheckGameOver();

protected:

	TextObject* m_ScoreText;
	int m_Score;

	TextObject* m_FreqText;

	//	TextObject* m_GameOverText;

	Player* m_Player;

	Missile* m_Missile;

	float m_EnemyFrequency;

	float m_EnemySpawnTimer;

	float m_MissileSpawnTimer;

};