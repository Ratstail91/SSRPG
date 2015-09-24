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
#include "example_scene.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

ExampleScene::ExampleScene(lua_State* L) {
	luaState = L;
	regionPager.SetLuaState(luaState);
	tileSheet.Load(GetRenderer(), "rsc\\terrain.png", 32, 32);

	player.GetSprite()->Load(GetRenderer(), "rsc\\character1.png", 4, 4);

	//DEBUG: basic map data
	for (int i = -20; i < 20; i++) {
		for (int j = -20; j < 20; j++) {
			regionPager.SetTile(i, j, 0, 50);
		}
	}

	if (luaL_dofile(luaState, "scripts\\startup.lua")) {
		std::ostringstream msg;
		msg << "Failed to run scripts\\startup.lua; " << lua_tostring(luaState, -1);
		throw(std::runtime_error(msg.str()));
	}
}

ExampleScene::~ExampleScene() {
	if (luaL_dofile(luaState, "scripts\\shutdown.lua")) {
		std::ostringstream msg;
		msg << "Failed to run scripts\\shutdown.lua; " << lua_tostring(luaState, -1);
		throw(std::runtime_error(msg.str()));
	}
}

//-------------------------
//frame phases
//-------------------------

void ExampleScene::FrameStart() {
	//
}

void ExampleScene::Update() {
	//framework is calibrated to 16 milliseconds per frame
	player.Update();

	//DEBUG: output: (player.x, player.y) (camera.x, camera.y)
//	std::ostringstream os;

	//(player.x, player.y)
//	os << "(" << player.GetOriginX() << ", " << player.GetOriginY() << ")";
//	std::cout << std::left << std::setw(20) << os.str();
//	os.str("");

	//(player.x, player.y)
//	os << "(" << camera.x << ", " << camera.y << ")" << std::endl;
//	std::cout << std::left << std::setw(20) << os.str();
//	os.str("");

	//(player.sprite.delay, player.sprite.tick)
//	os << "(" << player.GetSprite()->GetDelay() << ", " << player.GetSprite()->GetTick() << ")";
//	std::cout << std::left << std::setw(15) << os.str();
//	os.str("");

//	os << "(" << player.GetSprite()->GetCountX() << ", " << player.GetSprite()->GetCountY() << ")";
//	std::cout << std::left << std::setw(10) << os.str();
//	os.str("");

//	os << "(" << player.GetSprite()->GetIndexX() << ", " << player.GetSprite()->GetIndexY() << ")";
//	std::cout << std::left << std::setw(10) << os.str();
//	os.str("");

	//speed
//	os << "(" << player.GetMotion().Length() << ")";
//	std::cout << std::left << std::setw(10) << os.str();
//	os.str("");

//	std::cout << std::endl;
}

void ExampleScene::FrameEnd() {
	//update the camera position
	int screenWidth = 0, screenHeight = 0;
	SDL_RenderGetLogicalSize(GetRenderer(), &screenWidth, &screenHeight);
	//determine the "margin" (distance from top-left of the screen to the player's intended draw position)
	//subtract the margin from the player position to get the correct camera position
	//camera position is used for drawing all other objects relative to the player character
	camera.x = player.GetOriginX() - ((screenWidth  - player.GetSprite()->GetClipW()) / 2);
	camera.y = player.GetOriginY() - ((screenHeight - player.GetSprite()->GetClipH()) / 2);
}

void ExampleScene::RenderFrame(SDL_Renderer* renderer) {
	//draw the ground
	std::for_each(regionPager.GetContainer()->begin(), regionPager.GetContainer()->end(), [&](Region& r) {
		tileSheet.DrawRegionTo(GetRenderer(), &r, camera.x, camera.y, camera.zoom, camera.zoom);
	});

	//draw the player
	player.DrawTo(renderer, camera.x, camera.y);
}

//-------------------------
//input events
//-------------------------

void ExampleScene::MouseMotion(SDL_MouseMotionEvent const& event) {
	//
}

void ExampleScene::MouseButtonDown(SDL_MouseButtonEvent const& event) {
	//
}

void ExampleScene::MouseButtonUp(SDL_MouseButtonEvent const& event) {
	//
}

void ExampleScene::MouseWheel(SDL_MouseWheelEvent const& event) {
	//
}

void ExampleScene::KeyDown(SDL_KeyboardEvent const& event) {
	/* DOCS: hotkeys 1-9 call lua 
	*/
	//hotkeys (LCTRL is broken)
	if (event.keysym.mod & KMOD_SHIFT|KMOD_CTRL) {
		switch(event.keysym.sym) {
			case SDLK_1:
				//
			break;

			//...
		}

		//skip other checks
		return;
	}

	switch(event.keysym.sym) {
		//preference as a default
		case SDLK_ESCAPE:
			QuitEvent();
		break;

		case SDLK_w:
			player.PressUp();
		break;

		case SDLK_a:
			player.PressLeft();
		break;

		case SDLK_s:
			player.PressDown();
		break;

		case SDLK_d:
			player.PressRight();
		break;
	}
}

void ExampleScene::KeyUp(SDL_KeyboardEvent const& event) {
	//stop
	switch(event.keysym.sym) {
		case SDLK_w:
			player.ReleaseUp();
		break;

		case SDLK_a:
			player.ReleaseLeft();
		break;

		case SDLK_s:
			player.ReleaseDown();
		break;

		case SDLK_d:
			player.ReleaseRight();
		break;
	}
}
