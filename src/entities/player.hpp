/* Copyright: (c) Kayne Ruse 2015
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

#include "sprite_sheet.hpp"
#include "vector2.hpp"

#include "SDL2/SDL.h"

#include <cmath>

//the speeds that the character moves
constexpr double CHARACTER_WALKING_SPEED = 2.24;
constexpr double CHARACTER_DIAGONAL = 1.0/sqrt(2.0);

class Player {
public:
	Player();
	virtual ~Player();

	void Update();
	void DrawTo(SDL_Renderer*, int camX, int camY);

	//movement controls
	void PressUp();
	void PressDown();
	void PressLeft();
	void PressRight();

	void ReleaseUp();
	void ReleaseDown();
	void ReleaseLeft();
	void ReleaseRight();

	//accessors & mutators
	SpriteSheet* const GetSprite() { return &spriteSheet; }

	Vector2 GetOrigin() { return origin; }
	Vector2 GetMotion() { return motion; }

	double SetOriginX(double x) { origin.x = x; }
	double SetOriginY(double y) { origin.y = y; }
	double SetMotionX(double x) { motion.x = x; }
	double SetMotionY(double y) { motion.y = y; }

	double GetOriginX() { return origin.x; }
	double GetOriginY() { return origin.y; }
	double GetMotionX() { return motion.x; }
	double GetMotionY() { return motion.y; }

protected:
	void CorrectSprite();
	void CorrectMotion();

	SpriteSheet spriteSheet;
	Vector2 origin = {0, 0};
	Vector2 motion = {0, 0};
};