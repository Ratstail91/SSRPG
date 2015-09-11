/* Copyright: (c) Kayne Ruse 2013-2015
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 
 * 3. This notice may not be removed or altered from any source
 * distribution.
*/
#pragma once

#include "image.hpp"

#include "SDL2/SDL_ttf.h"

#include <string>

constexpr SDL_Color COLOR_WHITE = {255, 255, 255, 255};
constexpr SDL_Color COLOR_RED = {255, 0, 0, 255};
constexpr SDL_Color COLOR_ORANGE = {255, 127, 0, 255};
constexpr SDL_Color COLOR_BLUE = {0, 0, 255, 255};

class Button {
public:
	enum State {
		IDLE = 0, HOVER = 1, PRESSED = 2, RELEASED = 3
	};

	//methods
	Button() = default;
	~Button() = default;

	void DrawTo(SDL_Renderer*);

	//setup
	void SetBackgroundTexture(SDL_Renderer*, SDL_Texture*);
	void SetText(SDL_Renderer*, TTF_Font*, std::string, SDL_Color);
	void SetX(int x);
	void SetY(int y);

	//capture input
	State MouseMotion(SDL_MouseMotionEvent const&);
	State MouseButtonDown(SDL_MouseButtonEvent const&);
	State MouseButtonUp(SDL_MouseButtonEvent const&);

	//states
	void SetState(State); //TODO: idle, busy or disabled
	State GetState();

protected:
	bool CheckBounds(int x, int y);

	Image image;
	int posX = 0, posY = 0;
	State state = State::IDLE;
};
