#include "GameObjects/PhysicsObject.h"


#define Super SpriteObject 

PhysicsObject::PhysicsObject() :
	m_Deceleration(0.0f),
	m_Drag(1.0f),
	m_Mass(1.0f),
	m_MaxSpeed(600.0f) {}



void PhysicsObject::AddForce(Vector2 Direction, float Force)
{
	m_AccelerationForce = Direction * Force;
}

void PhysicsObject::OnPostUpdate(float DeletaTime)
{

	// the constant wind force against the object
	Vector2 DragForce(m_Velocity * -m_Drag);
	//Combine force for the velocity 
	Vector2 FullForce(DragForce + m_AccelerationForce);
	// physics force algorithm F + ma (a = F / m)
	m_Acceleration = FullForce / std::max(m_Mass, 0.00001f);
	//applying acceleration and multiply it my time 
	m_Velocity += m_Acceleration * DeletaTime;

	//cap the velocity at the maxmum value 
	if (m_Velocity.Length() > m_MaxSpeed) {
		m_Velocity = m_Velocity.Normalise() * m_MaxSpeed;
	}


	// the force t hat should stop the object faster 
	Vector2 DecelForce;

	//setdeceleration force only if we need decelerate
	if (m_Velocity.Length() < m_LastTickVelocity.Length()) {
		DecelForce = m_Velocity * -m_Deceleration;
	}

	//apply deceleration
	m_Velocity += DecelForce * DeletaTime;

	Vector2 TimeVelocity(m_Velocity * DeletaTime);

	SetPosition(GetTransform().Position + TimeVelocity);

	//reset out push force 
	m_AccelerationForce = Vector2();

	//update the last tick velocity 
	m_LastTickVelocity = m_Velocity;
	//runs the sprite stuff after physics has applied 
	Super::OnPostUpdate(DeletaTime);
}
