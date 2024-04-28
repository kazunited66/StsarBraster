#include"GameObjects/Enemy.h"
#include "Game.h"

#define Super Charactor 

Enemy::Enemy()
{
	m_MaxSpeed = 100.0f;
	m_Scale = 3.0f;
	m_Size = 48.0f - 16.0f;
	m_ReachedBottom = false;

	m_MainSprite = AddSprite(
		"Content/Sprites/Main Ship/MAin Ship - Bases/PNGs/Main Ship - Base - Very damaged.png");

	//change the scale
	SetScale(m_Scale);

	//flip it 180 to look downward
	SetRotation(180.0f);

	Bounds* EnemyBounds = AddBounds(0.0f, ScaleSize());
	EnemyBounds->m_OriginalOffset = -ScaleHalfSize();
	EnemyBounds->m_Tag = "ENEMY";
}


void Enemy::OnUpdate(float DeltTime)
{
	Super::OnPostUpdate(DeltTime);

	AddMovementInput(Vector2(0.0f, 1.0f));

	if (GetTransform().Position.y - ScaleHalfSize() > 720.0f) {
		DestreyObject();
		Game::GetGame()->SetGameOver(true);
	}
}
