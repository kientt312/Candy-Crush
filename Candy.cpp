#include "Candy.h"
#include "Appconfig.h"

SDL_Rect Candy::candyClips[CANDY_TYPES][CANDY_SPRITES];
Texture Candy::candySpriteSheetTexture;

Candy::Candy(){
	position.x = TOP_X;
	position.y = TOP_Y;
	currentSprite = CANDY_SPRITE_MOUSE_OUT;
	type = 0;
	removed = false;
	updatePosition = false;
	destY = 0;
	destX = 0;
}

void Candy::setPosition(int x, int y){
	position.x = x;
	position.y = y;
}

SDL_Point Candy::getPosition(){
	return position;
}

void Candy::setType(int tmpType){
	type = tmpType;
}

int Candy::getType(){
	return type;
}

void Candy::setPressed(bool isPressed){
	pressed = isPressed;
}

void Candy::setRemoved(bool isRemoved){
	removed = isRemoved;
}

bool Candy::isRemoved(){
	return removed;
}

void Candy::setToUpdate(bool toUpdate){
	updatePosition = toUpdate;
}

bool Candy::toUpdate(){
	return updatePosition;
}

bool Candy::load(std::string path){
    return candySpriteSheetTexture.loadImage(path);
}

void Candy::setClip(int i, int j, int x, int y, int w, int h){
    candyClips[i][j] = {x, y, w, h};
}

void Candy::render(){
	if (!removed)
        candySpriteSheetTexture.render(position.x, position.y, &candyClips[type][currentSprite]);
}

void Candy::unselectTexture(){
	currentSprite = CANDY_SPRITE_MOUSE_OUT;
}

void Candy::handleEvent(SDL_Event* e, Candy* pressedCandys[2], int &pressedCount){
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP){
		int x, y;
		SDL_GetMouseState(&x, &y);

		bool inside = true;

		if (x < position.x || x > position.x + CANDY_EDGE || y < position.y || y > position.y + CANDY_EDGE){
			inside = false;
		}

		if (!inside){
			if (pressed){
				currentSprite = CANDY_SPRITE_MOUSE_DOWN;
			}
			else{
				currentSprite = CANDY_SPRITE_MOUSE_OUT;
			}
		}
		else{
			switch (e->type){
			case SDL_MOUSEMOTION:
				if (!pressed){
					currentSprite = CANDY_SPRITE_MOUSE_OVER_MOTION;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
			    musicPlayer.play(SELECT);
				currentSprite = CANDY_SPRITE_MOUSE_DOWN;
				if (pressed){
					setPressed(false);
					pressedCandys[0] = NULL;
					--pressedCount;
					break;
				}
				else{
					setPressed(true);
					pressedCandys[pressedCount] = this;
					++pressedCount;
					break;
				}
			case SDL_MOUSEBUTTONUP:
				currentSprite = CANDY_SPRITE_MOUSE_DOWN;
				if (pressedCount == 1 && pressedCandys[0] != this){
					setPressed(true);
					pressedCandys[pressedCount] = this;
					++pressedCount;
					break;
				}
			}
		}
	}
}

void Candy::free(){
    candySpriteSheetTexture.free();
}
