#include "GameStates/GameState.h"
#include "GameObjects/GameObjects.h"
#include "Math/Bounds.h"
#include "SDL2/SDL_render.h"

#include "Debug.h"

 void GameState::Start()
{
	 OnStart();
	 EE_LOG("GameState", "start game state");


}

void GameState::Cleanup()
{
	 OnCleanup();

	 //desdtrtoy any object pending spwam 
	 for (auto GO : m_GameObjectPendingSpawn) {
		 GO->Cleanup();
		 delete GO;
		 GO = nullptr;
	 }
	 //destroy any remaining  game object
	 for (auto GO : m_GameObjectStack) {
		 GO->Cleanup();
		 delete GO;
		 GO = nullptr;
	 }
}

void GameState::PreLoop()
{

	//add all game object pending spwam to the game object stack 
	for (auto GO : m_GameObjectPendingSpawn) {
		m_GameObjectStack.push_back(GO);
		GO->Start();
	}

	m_GameObjectPendingSpawn.clear();

	OnPreLoop();
}

 void GameState::ProcessInput(Input* GameInput)
{
	 //run the input listner functiom for all game object 
	 for (auto GO : m_GameObjectStack) {
		 if (GO != nullptr) {
			 GO->ProcessInput(GameInput);
		 }
	 }

	 OnProcessInput(GameInput);

	

}

 void GameState::Update(float DeltaTime)
{
	 //run the update logic for all game object 
	 for (auto GO : m_GameObjectStack) {
		 if (GO != nullptr) {
			 GO->Update(DeltaTime);
			 GO->PostUpdate(DeltaTime);

			 for (auto OtherGo : m_GameObjectStack) {
				 for (auto OtherBounds : OtherGo->GetAllBounds()) {
					 // testing the bounds for overlap events
					 GO->TestOverlapEvent(OtherBounds);
				 }
			 }
		 }
	 }

	 OnUpdate(DeltaTime);
}

void GameState::Render(SDL_Renderer* Renderer)
{
	for (auto GO : m_GameObjectStack) {
		if (GO == nullptr) {
			continue;

		}
		//loop through all the game object bounds
		for (auto TestBounds : GO->GetAllBounds()) {
			if (!TestBounds->m_Debug) {
				continue;
			}
			SDL_SetRenderDrawColor(
				Renderer,

				TestBounds->m_RenderColour.r,
				TestBounds->m_RenderColour.g,
				TestBounds->m_RenderColour.b,
				255


			);
			SDL_FRect BoundsRect{
				TestBounds->GetCenter().x,
				TestBounds->GetCenter().y,
				TestBounds->m_Rect.Extent.x,
				TestBounds->m_Rect.Extent.y
			};
			SDL_RenderDrawRectF(Renderer, &BoundsRect);
		}
	}

}

void GameState::GarbageCollection()
{
	OnGarbageCollection();

	for (const auto GO : m_GameObjectStack) {
		GO->CollectGarbage();
	}
	//TODO: Delete object at the end of each frame
	for (int i = m_GameObjectStack.size() - 1; i >= 0; --i) {
		if (!m_GameObjectStack[i]->IsPendingDestroy()) {
			continue;
		}

		//make sure the game object is not nullptr 
		if (m_GameObjectStack[i] != nullptr) {
			m_GameObjectStack[i]->Cleanup();
			delete m_GameObjectStack[i];
		}

		//remove from and resize the array 
		m_GameObjectStack.erase(m_GameObjectStack.begin() + i);
	}

}

