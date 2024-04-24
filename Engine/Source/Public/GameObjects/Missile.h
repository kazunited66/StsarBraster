#pragma once
#include"Character.h"

class Missile : public Charactor {
public: 
	Missile();
protected: 
	
	virtual void OnUpdate(float DeltTime) override;

	virtual void OnOverlapEnter(Bounds* OverlapBoudns, Bounds* HitBounds) override;

protected:
	
public:
	bool m_hitEnemy;
};