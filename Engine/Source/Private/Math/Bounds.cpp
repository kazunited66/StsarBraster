#include "Math/Bounds.h"

Bounds::Bounds(GameObject* Owner, Vector2 Origin, Vector2 Extent)
{
	m_Rect.Origin = Origin;
	m_Rect.Extent = Extent;
	m_Owner = Owner;
	m_CollisionType = CT_OVERLAP;
	m_Tag = "ALL";
	m_ShouldDestroy = false;

#if 1		//org
	m_Debug = false;
#else
	m_Debug = true;
#endif

}

bool Bounds::BoundsIntersect(const Bounds& BoundsA, const Bounds& BoundsB)
{
	Vector2 BoundsAMin = BoundsA.m_Rect.Min() - BoundsA.m_OriginalOffset;
	Vector2 BoundsAMax = BoundsA.m_Rect.Max() + BoundsA.m_OriginalOffset;
	Vector2 BoundsBMin = BoundsB.m_Rect.Min() - BoundsB.m_OriginalOffset;
	Vector2 BoundsBMax = BoundsB.m_Rect.Max() + BoundsB.m_OriginalOffset;

	bool MinX = BoundsBMin.x <= BoundsAMax.x;
	bool MaxX = BoundsBMax.x >= BoundsAMin.x;
	bool MinY = BoundsBMin.y <= BoundsAMax.y;
	bool MaxY = BoundsBMax.y >= BoundsAMin.y;

	return MinX && MaxX && MinY && MaxY;
}
