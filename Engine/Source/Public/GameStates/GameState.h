#pragma once
#include "EngineTypes.h"
class Input;
struct SDL_Renderer;
class GameObject;

class GameState {
public: 
	GameState() : m_ShouldDestroy(false) {} 
	virtual ~GameState() = default;

    void Start();

	// clean up and deallocate memory for the state
	void Cleanup();

	//runs every frame at the start of a loop
	void PreLoop();

	//detects input od the game 
	void ProcessInput(Input* GameInput);

	//update each frame after input 
	void Update(float DeltaTime);

	void Render(SDL_Renderer* Renderer);

	void GarbageCollection();

	//add a game object to the game
	template<class T>
	T* AddGameObject() {
		T* NewObject = new T();

		//add the object to our pending apwan array 
		m_GameObjectPendingSpawn.push_back(NewObject);

		return NewObject;
	}

	void DestroyGameState() { m_ShouldDestroy = true;  }

	bool IsPendingDestroy() const { return m_ShouldDestroy; 
	}

protected: 
	virtual void OnStart() {}

	virtual void OnCleanup() {}

	virtual void OnPreLoop() {}

	virtual void OnProcessInput(Input* GameInput) {}

	virtual void OnUpdate(float DeltaTime) {}

    virtual void OnGarbageCollection() {}

private:
	TArray<GameObject*>m_GameObjectPendingSpawn;

	//store all game objects 
	TArray<GameObject*> m_GameObjectStack;

	bool m_ShouldDestroy;

};


