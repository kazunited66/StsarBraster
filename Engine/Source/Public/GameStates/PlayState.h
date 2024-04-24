#pragma once
#include "GameState.h"
#include"GameObjects/Player.h"	
#include"GameObjects/Missile.h"	

class TextObject;

class PlayState : public GameState {
public: 
	PlayState() ;

protected:
	virtual void OnStart() override;

	virtual void OnUpdate(float DeltaTime) override; 

	
	void EnemySpawner(float DeltaTime);

	void MissileSpawner(float DeltaTime);

	float GetRandomFloatRange(float min = 0.0f, float max = 1.0f) const;

	void UpdateScore();
	void UpdateFrequencyText();


protected:

	TextObject* m_ScoreText; 
	int m_Score;

	TextObject* m_FreqText;

	Player* m_Player;

	Missile* m_Missile;

	float m_EnemyFrequency; 

	float m_EnemySpawnTimer;

	float m_MissileSpawnTimer;

};