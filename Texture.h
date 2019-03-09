#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <ctime>

class Texture
{
public:
	Texture();
	~Texture();

	bool loadImage(std::string path);

	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void free();

	static SDL_Renderer* renderer;

private:
	SDL_Texture* m_texture;
};

#endif // TEXTURE_H
