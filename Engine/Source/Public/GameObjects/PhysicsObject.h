#pragma once
#include "SpriteObject.h"
#include "Math/Vector2.h"

class PhysicsObject : public SpriteObject {
public : 
	PhysicsObject();
	virtual ~PhysicsObject()override = default;

	//add force to the object
	//@param 1 - Direction for the force to be applied
	//@param 2 - The power/strenght of the force to apply 
	void AddForce(Vector2 Direction, float Force);

protected: 

	virtual void OnPostUpdate(float DeletaTime) override;
protected:  
	//after stopped moving then slow down 
	float m_Deceleration;
	// wind wind force against the physics 
	float m_Drag;
	//how heavy the object is 
	float m_Mass;
	//maximum velocity speed that we can travel 
	float m_MaxSpeed;
	//direction and speed of the physics object 
	Vector2 m_Velocity; 
private:
	//the hidden force changing velocity
	Vector2 m_Acceleration;
	//custom force that pushes our objects in physics 
	Vector2 m_AccelerationForce;
	//previous velocity on the last tick 
	Vector2 m_LastTickVelocity;




};
