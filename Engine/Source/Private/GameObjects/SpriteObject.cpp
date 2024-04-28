#include "GameObjects/SpriteObject.h"
#include "Debug.h"
#define Super GameObject 

void SpriteObject::Cleanup()
{
	for (auto Sprite : m_SpriteStack) {
		delete Sprite;
		Sprite = nullptr;
	}
}

Sprite* SpriteObject::AddSprite(const char* PathToFile, AnimationParams* Params)
{

	Animation* NewAnim = new Animation();

	if (!NewAnim->CreateAnimation(PathToFile, Params)) {
		return nullptr;
	}
	Sprite* NewSprite = new Sprite(NewAnim);

	//set the sprite to the object transforms 
	SpriteFollowObject(NewSprite);

	//add it to the sprite stack 
	m_SpriteStack.push_back(NewSprite);

	return NewSprite;
}

void SpriteObject::OnPostUpdate(float DeletaTIme)
{
	Super::OnPostUpdate(DeletaTIme);

	// loop though each sprite and set it to match the object transform 
	for (auto Sprite : m_SpriteStack) {
		if (Sprite == nullptr) {
			continue;
		}


		SpriteFollowObject(Sprite);
		//update the sprites animation 
		Sprite->m_Sprite->Update(DeletaTIme);
	}
}

void SpriteObject::SpriteFollowObject(Sprite* SpriteToFollow)
{
	//set the aprites position to match the objects
	SpriteToFollow->m_Sprite->SetPosition(
		(int)GetTransform().Position.x + SpriteToFollow->m_Offset.Position.x,
		(int)GetTransform().Position.y + SpriteToFollow->m_Offset.Position.y);

	//set the sprite rotation to matc hthe object 
	SpriteToFollow->m_Sprite->SetRotation(GetTransform().Rotation + SpriteToFollow->m_Offset.Rotation);

	//set the sprite scale to match the object 
	SpriteToFollow->m_Sprite->SetScale(
		GetTransform().Scale.x + SpriteToFollow->m_Offset.Scale.x,
		GetTransform().Scale.y + SpriteToFollow->m_Offset.Scale.y);
}
