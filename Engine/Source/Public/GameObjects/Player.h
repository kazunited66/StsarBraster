#pragma once
#include"GameObjects/Character.h"


class Player : public Charactor {
public: 
	Player(); 
protected: 


	virtual void OnProcessInput(Input* GameInput) override;

	virtual void OnUpdate(float DeltaTime) override; 
	void SetPoweredEngines(bool Powered);

	virtual void OnOverlapEnter(Bounds* OverlapBoudns, Bounds* HitBounds) override;
protected: 
	//store the diffrent effect for the engine effects 
	TArray<Sprite*>m_EngineEffects;
public: 
	bool	m_IsSpaceKeyPressed;
};