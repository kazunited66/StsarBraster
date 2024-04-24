#include"GameObjects/GameObjects.h"
#include"Debug.h"
#include"Math/Bounds.h"

void GameObject::Start()
{
	if (IsPendingDestroy()) {
		return;
	}
	EE_LOG("GameObject", "GameObject has spawned into the game");
	OnStart();

	BoundsMatchObjectPosition();

}

void GameObject::ProcessInput(Input* GameInput)
{
	if (IsPendingDestroy()) {
		return;
	}
	OnProcessInput(GameInput); 
} 

void GameObject::Update(float DeltaTime)
{
	if (IsPendingDestroy()) {
		return;
	}
	OnUpdate(DeltaTime);
}

void GameObject::PostUpdate(float DeltaTime)
{
	if (IsPendingDestroy()) {
		return;
	}
	OnPostUpdate(DeltaTime);
}

void GameObject::DestreyObject()
{
	if (m_ShouldDestroy) {
		OnDestroy();
	}
	 

	 m_ShouldDestroy = true;
}

void GameObject::Cleanup()
{
	for (auto TestBounds : m_BoundsStack) {
		delete TestBounds; 
	}
}

void GameObject::SetTransform(EETransform Transform)
{
	m_Transform = Transform;
}

void GameObject::SetPosition(Vector2 Position)
{
	EETransform NewTransform(
		Position,
		m_Transform.Rotation,
		m_Transform.Scale
	);

	SetTransform(NewTransform);

	BoundsMatchObjectPosition();
}

void GameObject::SetRotation(float Rotation)
{
	EETransform NewTransform(
		m_Transform.Position,
		Rotation,
		m_Transform.Scale
	);

	SetTransform(NewTransform);
}

void GameObject::SetScale(Vector2 Scale)
{
	EETransform NewTransform(
		m_Transform.Position,
		m_Transform.Rotation,
		Scale
	);

	SetTransform(NewTransform);
}

void GameObject::TestOverlapEvent(Bounds* OtherBounds)
{
	if (OtherBounds->m_CollisionType == CT_NONE) {
		return;
	}
	
	//loop to all of the collisition bounds
	for (auto TestBounds : m_BoundsStack) {


		if (TestBounds->m_CollisionType == CT_NONE) {
			continue;
		}

		if (TestBounds->m_Debug) {
			// change the colour if there is anything overlapping this bounds 
			if (TestBounds->m_Overlapped.size() > 0) {

				TestBounds->m_RenderColour = STBoundsColour(0, 255, 0);
			}
			else {
				TestBounds->m_RenderColour = STBoundsColour(255, 0, 0);
			}
		}

		if (OtherBounds->GetOwner() == this) {
			continue;

		}

	
		
		//search through the overlapped array to detect if me have already overlapped 
		auto it = std::find(TestBounds->m_Overlapped.begin(), TestBounds->m_Overlapped.end(), OtherBounds);
		bool IsIntersecting = Bounds::BoundsIntersect(*TestBounds, *OtherBounds);
		bool IsOverLapped = it != TestBounds->m_Overlapped.end();

		if (IsOverLapped) {
			if (!IsIntersecting) {
				//remove the other bounds from the overlapped array 
				TestBounds->m_Overlapped.erase(it);
				OnOverlapExit(OtherBounds, TestBounds);

				EE_LOG("Gameobject", "Exit.");
			}
		}
		else {
			if (IsIntersecting) {
				TestBounds->m_Overlapped.push_back(OtherBounds);

				OnOverlapEnter(OtherBounds, TestBounds);

				EE_LOG("GameObject", "Entered.");
			}
		}

	}
}

void GameObject::CollectGarbage()
{
	for (const auto TestBounds : m_BoundsStack) {
		size_t num = std::erase_if(TestBounds->m_Overlapped,
			[](Bounds* OBounds) { return OBounds->IsPendingDestroy(); }
			);
		if (num > 0) {
			EE_LOG("GameObject", "Bounds removed: " << num);
		}
		
	}

}

void GameObject::OnDestroy()
{
	EE_LOG("GameObject", "GameObject has been destroyed");
}

Bounds* GameObject::AddBounds(Vector2 Center, Vector2 Extent)
{
	Bounds* NewBounds = new Bounds(this, Center, Extent);
	m_BoundsStack.push_back(NewBounds);
	return NewBounds;
}

void GameObject::BoundsMatchObjectPosition()
{
	for (auto TestBounds : m_BoundsStack) {
		if (TestBounds == nullptr) {
			continue;
		}
		TestBounds->m_Rect.Origin = GetTransform().Position;
	}
}

 
