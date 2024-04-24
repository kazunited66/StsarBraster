#pragma once
#include "GameObjects/GameObjects.h"
#include <cstdint>
#include "Graphics/TextureTypes.h"

class Text;

struct SDL_Color;
class TextObject : public GameObject {
public: 
	TextObject(const char* FilePath = "Content/Fonts/Pixelify/PixelifySans-Regular.ttf");

	void SetText(const char* NewText);

	void SetFontSize(int NewSize);

	void SetFontColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a );

	void SetAlignment(EAligment NewAlignment);
protected: 
	virtual void OnUpdate(float DeltaTime) override;

	virtual void Cleanup() override;

private: 
	Text* m_Text;




};