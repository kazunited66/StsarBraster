#pragma once
#include "PhysicsObject.h"
#include"Math/Bounds.h"

class Charactor : public PhysicsObject {
public:
	Charactor();
	//move the charactor in the direction based on the accleration speed and max speed 
	void AddMovementInput(Vector2 Direction, float Scale = 1.0f);

	float HalfSize() const { return m_Size / 2.0f; };
	float ScaleHalfSize() const { return ScaleSize() / 2.0f; };
	float ScaleSize() const { return m_Size * m_Scale; }

public: 
	float m_Scale, m_Size;

protected: 
	virtual void OnPostUpdate(float DeltaTime) override;
protected:
	//main sprite that represent the charactor 
	Sprite* m_MainSprite;
	
	//speed the charactor will move to max speed 
	float m_AccelerationSpeed;
 
	Vector2 m_MoveDirection;
};
