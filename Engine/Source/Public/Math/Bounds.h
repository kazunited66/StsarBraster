#pragma once
#include "Math/Vector2.h"
#include <cstdint>
#include <EngineTypes.h>
class GameObject;
struct EERect {
	//simple constructor 
	EERect() : Origin(0.0f), Extent(1.0f) {}
	//complex constructor 
	EERect(Vector2 Origin, Vector2 Extent) : Origin(Origin), Extent(Extent) {}

	// min value position of the relative to the screen 
	Vector2 Min() const { return Origin - Extent; }
	// max value position of the relative to the screen 
	Vector2 Max() const { return Origin + Extent; }
	//center of rectengle 	
	Vector2 Origin;
	//half size of the box 
	Vector2 Extent;
};

enum EECollisionType : unsigned int {
	CT_NONE = 0,
	CT_OVERLAP
};

struct STBoundsColour {
	STBoundsColour() : r(255), g(0), b(0) {}

	STBoundsColour(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}


	uint8_t r;
	uint8_t g;
	uint8_t b;


};

class Bounds {
public:
	Bounds(GameObject* Owner, Vector2 Origin = 0.0f, Vector2 Extent = 1.0f);

	Vector2 GetCenter() { return m_Rect.Origin + m_OriginalOffset; }

	static bool BoundsIntersect(const Bounds& BoundsA, const Bounds& BoundsB);

	GameObject* GetOwner() const { return m_Owner; }

	// mark the bounds for destroy 
	void DestroyBounds() { m_ShouldDestroy = true; }

	// check is the bounds is marked for destroy
	bool IsPendingDestroy() const { return m_ShouldDestroy; }

public:

	//this is the rectengle that defines the bound 
	EERect m_Rect;

	Vector2 m_OriginalOffset;

	EECollisionType m_CollisionType;

	const char* m_Tag;

	STBoundsColour m_RenderColour;

	TArray<Bounds*> m_Overlapped;

	bool m_Debug;

private:


	//this is the owner the bounds is attached to 
	GameObject* m_Owner;

	// flag to determine if the bounds is to be destroyed 
	bool m_ShouldDestroy;
};