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
#include "map_scene.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

//utility contanst
constexpr double minZoom = 0.25;
constexpr double maxZoom = 2.00;

//utility functions
static int dofile_ext(lua_State* L, const char* filename, void* arg1) {
	int ret = luaL_loadfile(L, filename);
	if (ret) return ret;
	lua_pushlightuserdata(L, arg1);
	ret |= lua_pcall(L, 1, LUA_MULTRET, 0);
	return ret;
}

//method definitions
MapScene::MapScene(lua_State* L) {
	//initial state
	luaState = L;
	regionPager.SetLuaState(luaState);
	tileSheet.Load(GetRenderer(), "rsc\\overworld.png", 32, 32);

	//run startup.lua
	if (dofile_ext(luaState, "scripts\\startup.lua", &regionPager)) {
		std::ostringstream msg;
		msg << "Failed to run scripts\\startup.lua; " << lua_tostring(luaState, -1);
		throw(std::runtime_error(msg.str()));
	}

	font = TTF_OpenFont("C:/Windows/Fonts/arialbd.ttf", 12);

	//check that the font loaded
	if (!font) {
		std::ostringstream msg;
		msg << "Failed to load a font file; " << SDL_GetError();
		throw(std::runtime_error(msg.str()));
	}
}

MapScene::~MapScene() {
	//run shutdown.lua
	if (dofile_ext(luaState, "scripts\\shutdown.lua", &regionPager)) {
		std::ostringstream msg;
		msg << "Failed to run scripts\\shutdown.lua; " << lua_tostring(luaState, -1);
		throw(std::runtime_error(msg.str()));
	}
}

//-------------------------
//frame phases
//-------------------------

void MapScene::FrameStart() {
	//
}

void MapScene::Update() {
	//
}

void MapScene::FrameEnd() {
	//TODO: use a proper bounds check to create & cull regions, possibly maxZoom?
	//pull the regions into existance
	int screenWidth = 0, screenHeight = 0;
	SDL_RenderGetLogicalSize(GetRenderer(), &screenWidth, &screenHeight);
	for (int i = snapToBase(REGION_WIDTH, camera.x / tileSheet.GetTileW()); i <= snapToBase(REGION_WIDTH, (camera.x + screenWidth / camera.zoom) / tileSheet.GetTileW()); i += REGION_WIDTH) {
		for (int j = snapToBase(REGION_HEIGHT, camera.y / tileSheet.GetTileH()); j <= snapToBase(REGION_HEIGHT, (camera.y + screenHeight / camera.zoom) / tileSheet.GetTileH()); j += REGION_HEIGHT) {
			regionPager.GetRegion(i, j);
		}
	}

	//cull distant regions
	regionPager.UnloadIf([&](Region const& r) -> bool {
		//rough boundry for unloading
		return (r.GetX() * tileSheet.GetTileW() - camera.x > screenWidth / minZoom) || (r.GetY() * tileSheet.GetTileH() - camera.y > screenHeight / minZoom);
	});
}

void MapScene::RenderFrame(SDL_Renderer* renderer) {
	//draw the ground
	std::for_each(regionPager.GetContainer()->begin(), regionPager.GetContainer()->end(), [&](Region& r) {
		tileSheet.DrawRegionTo(renderer, &r, camera.x, camera.y, camera.zoom, camera.zoom);

		//quick and dirty
		std::ostringstream msg;
		msg << r.GetX() << ", " << r.GetY();
		TextLine textLine(renderer, font, msg.str(), SDL_Color{255, 255, 255, 255});
		textLine.DrawTo(renderer, (r.GetX() * tileSheet.GetTileW() - camera.x) * camera.zoom, (r.GetY() * tileSheet.GetTileH() - camera.y) * camera.zoom);
	});
}

//-------------------------
//input events
//-------------------------

void MapScene::MouseMotion(SDL_MouseMotionEvent const& event) {
	if (event.state & SDL_BUTTON_RMASK) {
		camera.x -= event.xrel / camera.zoom;
		camera.y -= event.yrel / camera.zoom;
	}
}

void MapScene::MouseButtonDown(SDL_MouseButtonEvent const& event) {
	//
}

void MapScene::MouseButtonUp(SDL_MouseButtonEvent const& event) {
	//
}

void MapScene::MouseWheel(SDL_MouseWheelEvent const& event) {
	//zoom
	if (event.y < 0) camera.zoom *= 1.1;
	if (event.y > 0) camera.zoom /= 1.1;

	//min & max zom
	if (camera.zoom < minZoom) camera.zoom = minZoom;
	if (camera.zoom > maxZoom) camera.zoom = maxZoom;
}

void MapScene::KeyDown(SDL_KeyboardEvent const& event) {
	//hotkeys (LCTRL is broken)
	if (event.keysym.mod & (KMOD_SHIFT|KMOD_CTRL) && !event.repeat) {
		switch(event.keysym.sym) {
			case SDLK_1:
				dofile_ext(luaState, "scripts\\hotkey_1.lua", &regionPager);
			break;

			case SDLK_2:
				dofile_ext(luaState, "scripts\\hotkey_2.lua", &regionPager);
			break;

			case SDLK_3:
				dofile_ext(luaState, "scripts\\hotkey_3.lua", &regionPager);
			break;

			case SDLK_4:
				dofile_ext(luaState, "scripts\\hotkey_4.lua", &regionPager);
			break;

			case SDLK_5:
				dofile_ext(luaState, "scripts\\hotkey_5.lua", &regionPager);
			break;
		}

		//skip other checks
		return;
	}

	switch(event.keysym.sym) {
		//preference as a default
		case SDLK_ESCAPE:
			QuitEvent();
		break;

		case SDLK_SPACE:
			camera.zoom = 1.0;
		break;
	}
}

void MapScene::KeyUp(SDL_KeyboardEvent const& event) {
	//
}
