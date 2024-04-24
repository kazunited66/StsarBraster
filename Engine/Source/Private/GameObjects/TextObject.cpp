#include "GameObjects/TextObject.h"
#include "Game.h"
#include "Graphics/Text.h"
#include "SDL2/SDL.h"


#define Super GameObject

TextObject::TextObject(const char* FilePath)
{
	m_Text = Game::GetGame()->ImportText(FilePath);
}

void TextObject::SetText(const char* NewText)
{
	m_Text->SetText(NewText);
}

void TextObject::SetFontSize(int NewSize)
{
	m_Text->SetFontSize(NewSize);
}

void TextObject::SetFontColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	m_Text->SetFontColour({r,g,b,a });
}

void TextObject::SetAlignment(EAligment NewAlignment)
{
	m_Text->m_Alignment = NewAlignment; 
}

void TextObject::OnUpdate(float DeltaTime)
{
	Super::OnUpdate(DeltaTime);

	m_Text->m_PosX = GetTransform().Position.x;
	m_Text->m_PosY = GetTransform().Position.y;

	m_Text->m_Angle = GetTransform().Rotation; 


}

void TextObject::Cleanup()
{

	Game::GetGame()->DestroyText(m_Text);

	Super::Cleanup();
}
