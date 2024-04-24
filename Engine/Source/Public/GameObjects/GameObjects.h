#pragma once
#include "Math/Vector2.h"
#include<EngineTypes.h>
class Input;
class Bounds; 
struct EETransform {
	//default structor 
	EETransform() : Position(0.0f), Rotation(0.0f), Scale(1.0f) {}
	//assignment constructor
	EETransform(Vector2 Position, float Rotation, Vector2 Scale) :  
	          Position(Position), Rotation(Rotation), Scale(Scale){}
	//transform constructor
	EETransform(const EETransform& Other) :
		Position(Other.Position), Rotation(Other.Rotation), Scale(Other.Scale) {}


	Vector2 Position;
	float Rotation;
	Vector2 Scale;


};

class GameObject {

public:
	GameObject(): m_ShouldDestroy(false){

	}

	virtual ~GameObject() = default;

	//runs when this object spaqns into the game
	virtual void Start();

	//listen for input events 
	void ProcessInput(Input* GameInput);

	//run evry frame
	virtual void Update(float DeltaTime);

	//runs every frame after the update 
	void PostUpdate(float DeltaTime);

	// destroy the game 
	void DestreyObject();


	bool IsPendingDestroy() const { return m_ShouldDestroy; }

	//remove any memory reference 
	virtual void Cleanup();


	//Position, Rotation, Scale

	EETransform GetTransform() const { return m_Transform;  }

	//set the transform of the object 
	void SetTransform(EETransform Transform);

	void SetPosition(Vector2 Position);
	void SetRotation(float Rotation);
	void SetScale(Vector2 Scale);
	//test which bounds event needs to run 
	void TestOverlapEvent(Bounds* OtherBounds);

	TArray<Bounds*>GetAllBounds() const { return m_BoundsStack; }

	void CollectGarbage();

protected:
	//run when the game object has been marked for destroy 
	virtual void OnDestroy();
	//runs when the game object has been marked for destory 
	virtual void OnStart() {};
	//run on the game object has been marked for destroy 
	virtual void OnUpdate(float DeltaTime) {}
	//run on the game object process input 
	virtual void OnProcessInput(Input* GameInput){}
	//run on the game object post update (each frame after the update)
	virtual void OnPostUpdate(float DeltaTime) {}

	virtual void OnOverlapEnter(Bounds* OverlapBounds, Bounds* HitBounds) {}

	virtual void OnOverlapExit(Bounds* OverlapBounds, Bounds* HitBounds) {}

	Bounds* AddBounds(Vector2 Center = 0.0f, Vector2 Extent = 1.0f);

private:
	//set all bounds to match the object position 
	void BoundsMatchObjectPosition();
private:
	//determine is the object should be destroyed at the end of thr loop 
	bool m_ShouldDestroy;

	EETransform m_Transform; 

	TArray<Bounds*> m_BoundsStack;

};