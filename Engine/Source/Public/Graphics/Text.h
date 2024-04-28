#pragma once
#include "Graphics/Texture.h"

typedef struct _TTF_Font TTF_Font;
struct SDL_Color;

class Text : public Texture {
public:
	Text(SDL_Renderer* Renderer);
	~Text();

	virtual bool ImportTexture(const char* PathToFile) override;

	virtual void Cleanup() override;

	void SetText(const char* NewText);

	void SetFontSize(int NewSize);

	void SetFontColour(SDL_Color Colour);

protected:
	void UpdateFont();

private:

	TTF_Font* m_Font;

	SDL_Color* m_TextColour;

	const char* m_Text;

	int m_FontSize;

};