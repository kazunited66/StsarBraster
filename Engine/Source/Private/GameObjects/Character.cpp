#include"GameObjects/Character.h"
#define Super PhysicsObject
Charactor::Charactor() :
	m_MainSprite(nullptr),
	m_AccelerationSpeed(2000.0f),
	m_Scale(1.0f),
	m_Size(1.0f)
	

{}

void Charactor::AddMovementInput(Vector2 Direction, float Scale)
{
	//increese out movement direction based on a direction and scale 
	m_MoveDirection += Direction.Normalise()* Scale;
}

void Charactor::OnPostUpdate(float DeltaTime)
{
	//if we have a movemnet direction set then move the object using physics 
	if (m_MoveDirection.Length() > 0.0f) {
		AddForce(m_MoveDirection, m_AccelerationSpeed);
	}

	m_MoveDirection = Vector2();
	Super::OnPostUpdate(DeltaTime); 
}
