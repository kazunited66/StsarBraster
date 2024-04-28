#include "Graphics/Texture.h"
#include"SDL2/SDL.h"
#include"SDL2/SDL_image.h"
#include"Debug.h"


Texture::Texture(SDL_Renderer* Renderer)
{
	m_RendererRef = Renderer;
	m_Angle = 0.0f;
	m_Path = "";
	m_PosX = m_PosY = 0;
	m_SurfaceData = nullptr;
	m_TextureRef = nullptr;
	m_ScaleX = m_ScaleY = 1.0f;
	m_ClipRect = nullptr;
	m_IsVisible = true;
	m_Alignment = AL_CENTER;
}

Texture::~Texture()
{
	if (m_ClipRect != nullptr) {
		delete m_ClipRect;
	}
}

bool Texture::ImportTexture(const char* PathToFile)
{

	//store the path to the file
	m_Path = PathToFile;

	//import the image and convert it to a surface 
	//fill in the object with the data for the image 
	m_SurfaceData = IMG_Load(PathToFile);

	// did the import fail 
	if (m_SurfaceData == nullptr) {
		EE_LOG("Texture", "Image failed to import: " << SDL_GetError());

		return false;
	}

	//create the textture from the surface 
	m_TextureRef = SDL_CreateTextureFromSurface(m_RendererRef, m_SurfaceData);

	if (m_TextureRef == nullptr) {
		EE_LOG("Texture", "Image failed to convert to texture: " << SDL_GetError());
		Cleanup();
	}

	EE_LOG("Texture", "Successfully imported texture: " << m_Path);

	return true;
}

void Texture::CopyTexture(Texture* CopyTexture)
{
	m_Path = CopyTexture->m_Path;
	m_SurfaceData = CopyTexture->m_SurfaceData;
	m_TextureRef = CopyTexture->m_TextureRef;
}

void Texture::Draw()
{
	if (!m_IsVisible) {
		return;
	}

	float ImageWidth = (float)m_SurfaceData->w;
	float ImageHeight = (float)m_SurfaceData->h;

	SDL_FRect DestRect = {
		(float)m_PosX,(float)m_PosY,
		ImageWidth * m_ScaleX, ImageHeight * m_ScaleY

	};
	if (m_ClipRect != nullptr) {
		DestRect.w = m_ClipRect->w * m_ScaleX;
		DestRect.h = m_ClipRect->h * m_ScaleY;

	}
	switch (m_Alignment)
	{
	case AL_CENTER:
		// move the texture to be center at the middle point of the image 
		DestRect.x -= DestRect.w / 2;
		DestRect.y -= DestRect.h / 2;
		break;
	case AL_LEFT:
		DestRect.y -= DestRect.h / 2;
		break;
	case AL_RIGHT:
		DestRect.x -= DestRect.w;
		DestRect.y -= DestRect.h / 2;
		break;
	case AL_TOP_LEFT:
		break;
	case AL_TOP_RIGHT:
		DestRect.x -= DestRect.w;
		break;
	case AL_TOP_CENTER:
		DestRect.x -= DestRect.w / 2;
		break;
	case AL_BOTTOM_LEFT:
		DestRect.y -= DestRect.h;
	case AL_BOTTOM_RIGHT:
		DestRect.y -= DestRect.h;
		DestRect.x -= DestRect.w;
		break;
	case AL_BOTTOM_CENTER:
		DestRect.y -= DestRect.h;
		DestRect.x -= DestRect.w;

		break;
	default:
		break;
	}


	SDL_FPoint Center{
		DestRect.w / 2,
		DestRect.h / 2
	};

	SDL_RenderCopyExF(
		m_RendererRef,//Render to draw to 
		m_TextureRef,//The texture to draw to the renderer
		m_ClipRect, //Clip rect 
		&DestRect, // position and scale on the screen 
		m_Angle, // rotation of the texture 
		&Center, // center point for the rotation 
		SDL_FLIP_NONE // flip the texture vertical or horizontal 
	);
}

void Texture::Cleanup()
{
	//clean up the surface 
	if (m_SurfaceData != nullptr) {
		SDL_FreeSurface(m_SurfaceData);
	}
	//clean up the texture 
	if (m_TextureRef != nullptr) {
		SDL_DestroyTexture(m_TextureRef);
	}
	EE_LOG("Texture", "Successfully destroyed image : " << m_Path);
}

void Texture::SetClip(int x, int y, int w, int h)
{
	//creaate the sdl rect object
	m_ClipRect = new SDL_Rect();

	//set the rect values
	m_ClipRect->x = x;
	m_ClipRect->y = y;
	m_ClipRect->w = w;
	m_ClipRect->h = h;


}

