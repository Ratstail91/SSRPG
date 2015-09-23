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
#include "player.hpp"

Player::Player() {
	//
}

Player::~Player() {
	//
}

void Player::Update() {
	origin += motion;
	spriteSheet.Update(.016);
}

void Player::DrawTo(SDL_Renderer* renderer, int camX, int camY) {
	spriteSheet.DrawTo(renderer, origin.x - camX, origin.y - camY);
}

//-------------------------
//movement controls
//-------------------------

void Player::PressUp() {
	motion.y = std::max(-CHARACTER_WALKING_SPEED, motion.y - CHARACTER_WALKING_SPEED);
	CorrectSprite();
	CorrectMotion();
}

void Player::PressDown() {
	motion.y = std::min(CHARACTER_WALKING_SPEED, motion.y + CHARACTER_WALKING_SPEED);
	CorrectSprite();
	CorrectMotion();
}

void Player::PressLeft() {
	motion.x = std::max(-CHARACTER_WALKING_SPEED, motion.x - CHARACTER_WALKING_SPEED);
	CorrectSprite();
	CorrectMotion();
}

void Player::PressRight() {
	motion.x = std::min(CHARACTER_WALKING_SPEED, motion.x + CHARACTER_WALKING_SPEED);
	CorrectSprite();
	CorrectMotion();
}

void Player::ReleaseUp() {
	motion.y = std::min(0.0, motion.y + CHARACTER_WALKING_SPEED);
	CorrectSprite();
	CorrectMotion();
}

void Player::ReleaseDown() {
	motion.y = std::max(0.0, motion.y - CHARACTER_WALKING_SPEED);
	CorrectSprite();
	CorrectMotion();
}

void Player::ReleaseLeft() {
	motion.x = std::min(0.0, motion.x + CHARACTER_WALKING_SPEED);
	CorrectSprite();
	CorrectMotion();
}

void Player::ReleaseRight() {
	motion.x = std::max(0.0, motion.x - CHARACTER_WALKING_SPEED);
	CorrectSprite();
	CorrectMotion();
}

void Player::CorrectSprite() {
	//NOTE: This must correspond to the sprite sheet in use
	if (motion.y > 0) {
		spriteSheet.SetIndexY(0);
	}
	else if (motion.y < 0) {
		spriteSheet.SetIndexY(1);
	}
	else if (motion.x > 0) {
		spriteSheet.SetIndexY(3);
	}
	else if (motion.x < 0) {
		spriteSheet.SetIndexY(2);
	}

	//animation
	if (motion.x || motion.y) {
		if (!spriteSheet.GetDelay()) {
			spriteSheet.SetDelay(0.1);
		}
	}
	else {
		spriteSheet.SetDelay(0);
		spriteSheet.SetIndexX(0);
	}
}

void Player::CorrectMotion() {
	//defensive book keeping code
	if (motion.x < 0) {
		motion.x = -CHARACTER_WALKING_SPEED;
	}
	if (motion.x > 0) {
		motion.x = CHARACTER_WALKING_SPEED;
	}
	if (motion.y < 0) {
		motion.y = -CHARACTER_WALKING_SPEED;
	}
	if (motion.y > 0) {
		motion.y = CHARACTER_WALKING_SPEED;
	}
	if (motion.x && motion.y) {
		motion *= CHARACTER_DIAGONAL;
	}
}