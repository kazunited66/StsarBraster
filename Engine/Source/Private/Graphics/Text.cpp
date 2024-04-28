#include "Graphics/Text.h"
#include"SDL2/SDL_ttf.h"
#include"Debug.h"

#define Super Texture 

Text::Text(SDL_Renderer* Renderer) : Super(Renderer)
{
	m_Font = nullptr;
	m_TextColour = new SDL_Color({ 255,255,255,255 });
	m_Text = "Text";
	m_FontSize = 24;
}

Text::~Text()
{
	delete m_TextColour;
}

bool Text::ImportTexture(const char* PathToFile)
{
	//store the path to the file
	m_Path = PathToFile;

	m_Font = TTF_OpenFont(PathToFile, m_FontSize);

	if (m_Font == nullptr) {
		EE_LOG("Text", "Font failed to import: " << TTF_GetError());

		return false;
	}

	m_SurfaceData = TTF_RenderText_Solid(m_Font, m_Text, *m_TextColour);

	// did the import fail 
	if (m_SurfaceData == nullptr) {
		EE_LOG("Text", "Font failed to convert to suurface: " << TTF_GetError());

		return false;
	}

	//create the textture from the surface 
	m_TextureRef = SDL_CreateTextureFromSurface(m_RendererRef, m_SurfaceData);

	if (m_TextureRef == nullptr) {
		EE_LOG("Texture", "Image failed to convert to texture: " << SDL_GetError());
		Cleanup();
		return false;
	}

	EE_LOG("Text", "Successfully imported font: " << m_Path);

	return true;

}

void Text::Cleanup()
{
	TTF_CloseFont(m_Font);

	//    Super: Cleanup();
	Super::Cleanup();
}

void Text::SetText(const char* NewText)
{

	m_Text = NewText;
	UpdateFont();
}

void Text::SetFontSize(int NewSize)
{
	if (NewSize == m_FontSize) {
		return;
	}
	m_FontSize = NewSize;

	TTF_CloseFont(m_Font);

	m_Font = TTF_OpenFont(m_Path, m_FontSize);

	UpdateFont();
}

void Text::SetFontColour(SDL_Color Colour)
{
	if (m_TextColour == &Colour) {
		return;
	}

	m_TextColour->r = Colour.r;
	m_TextColour->g = Colour.g;
	m_TextColour->b = Colour.b;
	m_TextColour->a = Colour.a;

	UpdateFont();
}

void Text::UpdateFont()
{
	if (m_Font == nullptr) {
		return;
	}
	SDL_FreeSurface(m_SurfaceData);

	SDL_DestroyTexture(m_TextureRef);

	m_SurfaceData = TTF_RenderText_Solid(m_Font, m_Text, *m_TextColour);
	m_TextureRef = SDL_CreateTextureFromSurface(m_RendererRef, m_SurfaceData);
}
