#include "Button.h"

Button::Button(){
}


Button::~Button(){
}

void Button::setPosition(int x, int y){
	position.x = x;
	position.y = y;
}

SDL_Point Button::getPosition(){
	return position;
}

void Button::setDimension(int w, int h){
	width = w;
	height = h;
}

void Button::setID(int ID){
	id = ID;
}

int Button::getID(){
	return id;
}

void Button::setClip(int x, int y, int w, int h){
    clip = {x, y, w, h};
}

bool Button::load(std::string path){
    return texture.loadImage(path);
}

void Button::render(){
    texture.render(position.x, position.y, &clip);
}

bool Button::handleEvent(SDL_Event* e){
	bool buttonPressed = false;
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP){
		int x, y;
		SDL_GetMouseState(&x, &y);

		bool inside = true;

		/// Check whether mouse is inside the candy
		if (x < position.x || x > position.x + width || y < position.y || y > position.y + height){
			inside = false;
		}

		if (inside){
			if (e->type == SDL_MOUSEBUTTONDOWN){
                musicPlayer.play(SELECT);
				buttonPressed = true;
			}
		}
	}
	return buttonPressed;
}

void Button::free(){
    texture.free();
}
