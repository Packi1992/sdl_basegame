#include "gamebase.h"

//
// Created by banoodle on 24.09.23.
//
#include "../gamebase.h"
#include "player.h"
#ifndef SDL_BASEGAME_TILESTATE_H
#define SDL_BASEGAME_TILESTATE_H


class Tilestate : public GameState
{
protected:
	Texture * tilesets[3]= { nullptr };
	Texture * bg[3];
	Texture * clouds;
	Point bgSize = {1024, 606};

	const u8 * key_array = SDL_GetKeyboardState( nullptr );
	Player player;
	FPoint cam{ .x = 0, .y = 0 };

	void loadTextures();
	Texture* loadTexture(std::string path);

public:
	// ctor
	using GameState::GameState;

	void Init() override;
	void UnInit() override;

	void Events( const u32 frame, const u32 totalMSec, const float deltaT ) override;
	void Update( const u32 frame, const u32 totalMSec, const float deltaT ) override;
	void Render( const u32 frame, const u32 totalMSec, const float deltaT ) override;

};


#endif //SDL_BASEGAME_TILESTATE_H
