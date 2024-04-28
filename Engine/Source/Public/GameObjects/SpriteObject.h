#pragma once
#include "GameObjects/GameObjects.h"
#include"Graphics/Animation.h"

class Sprite {
public:
	//default constructor 
	Sprite() : m_Sprite(nullptr), m_IsActive(true) {
		m_Offset.Scale = 0.0f;
	}
	//sprite assign constructor 
	Sprite(Animation* Anim) : m_Sprite(Anim), m_IsActive(true) {
		m_Offset.Scale = 0.0f;
	}

	~Sprite() {
		delete m_Sprite;
		m_Sprite = nullptr;
	}

	void SetActive(bool IsActive) {
		m_IsActive = IsActive;
		m_Sprite->SetVisible(IsActive);
	}
public:
	Animation* m_Sprite;
	EETransform m_Offset;


private:
	bool m_IsActive;

};
class SpriteObject : public GameObject {
public:

	SpriteObject() = default;


	virtual void Cleanup() override;


protected:
	//add a animation/sprite to the sprite stack 
	Sprite* AddSprite(const char* PathToFile, AnimationParams* Params = nullptr);

	virtual void OnPostUpdate(float DeletaTIme) override;

private:
	void SpriteFollowObject(Sprite* SpriteToFollow);
private:

	//store all sprites related to this object 
	TArray<Sprite*>m_SpriteStack;



};
