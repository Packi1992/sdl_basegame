//
// Created by banoodle on 25.09.23.
//

#ifndef SDL_BASEGAME_PLAYER_H
#define SDL_BASEGAME_PLAYER_H

#include "../gamebase.h"

class Player
{
private:
	Texture * loadTexture( std::string path );
public:
	FPoint pos{ 50, 600 };
	FPoint _size{ 100, 100 };
	FPoint _speed{ 0, 0 };
	FPoint _maxSpeed{ 5, 30 };
	FPoint _acceleration{ 1, 30 };
	Renderer * _render{};
	Texture * _idle{};
	Texture * _run{};
	Texture * _roll{};
	Texture * _jump{};
	int _rollCountdown = 0;
	int _rollCounter = 8;
	int _startRollFrame = 0;
	u8 _color[4] = { 0, 255, 0, 255 };//krasses grün
	void draw( Point offset, const u32 frame );
	void ini( Renderer * render );
};


#endif //SDL_BASEGAME_PLAYER_H
