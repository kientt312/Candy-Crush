#include "Texture.h"

SDL_Renderer *Texture::renderer = NULL;

Texture::Texture(){
	m_texture = NULL;
}

Texture::~Texture(){
	free();
}

bool Texture::loadImage(std::string path){
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == NULL){
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL){
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		SDL_FreeSurface(loadedSurface);
	}
	m_texture = newTexture;

	return m_texture != NULL;
}

void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip){
	SDL_Rect render_quad = {x, y, clip->w, clip->h};

	SDL_RenderCopyEx(renderer, m_texture, clip, &render_quad, angle, center, flip);
}

void Texture::free(){
	if (m_texture != NULL){
		SDL_DestroyTexture(m_texture);
		m_texture = NULL;
	}
}
