#include"GameObjects/Missile.h"
#include "GameObjects/Enemy.h"

#define Super Charactor 

Missile::Missile()
{
//	m_MaxSpeed = 100.0f;
	m_MaxSpeed = 50;
	m_Scale = 3.0f;
	m_Size = 48.0f - 16.0f;
	m_hitEnemy = false;

	m_MainSprite = AddSprite(
		//		"Content/Sprites/Main Ship/MAin Ship - Bases/PNGs/Main Ship - Base - Very damaged.png");
		"Content/Sprites/Main Ship/Main Ship - Engines/PNGs/Main Ship - Engines - Big Pulse Engine.png");

	//change the scale
	SetScale(m_Scale);

	Bounds* MissileBounds = AddBounds(0.0f, ScaleSize());
	MissileBounds->m_OriginalOffset = -ScaleHalfSize();
	MissileBounds->m_Tag = "MISSILE";
}


void Missile::OnUpdate(float DeltTime)
{
	Super::OnPostUpdate(DeltTime);

	AddMovementInput(Vector2(0.0f, -1.0f));

	if (GetTransform().Position.y + ScaleHalfSize() < 0) {
		DestreyObject();
	}
}

void Missile::OnOverlapEnter(Bounds* OverlapBoudns, Bounds* HitBounds)
{
#if 0
	if (dynamic_cast<Enemy*>(OverlapBoudns->GetOwner())) {
		OverlapBoudns->GetOwner()->DestreyObject();
	}
	if (dynamic_cast<Missile*>(HitBounds->GetOwner())) {
		HitBounds->GetOwner()->DestreyObject();
		m_hitEnemy = true;
	}
#else
	if ( (dynamic_cast<Enemy*>(OverlapBoudns->GetOwner())) &&
		 (dynamic_cast<Missile*>(HitBounds->GetOwner()))){ 
			OverlapBoudns->GetOwner()->DestreyObject();
			HitBounds->GetOwner()->DestreyObject();
			m_hitEnemy = true;
	}
#endif

}
