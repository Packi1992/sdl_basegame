//
// Created by banoodle on 25.09.23.
//

#ifndef SDL_BASEGAME_PLAYER_H
#define SDL_BASEGAME_PLAYER_H

#include "../gamebase.h"
class Player
{
private:
	Texture * loadTexture( std::string path ) const;
public:
	enum dir{
		LEFT,
		RIGHT
	};
	FPoint _pos{ 3, 6 };
	FPoint _size{ 100, 100 };
	FPoint _mapSize {};
	FPoint _speed{ 0, 0 };
	FPoint _maxSpeed{ 0.08, 0.3 };
	FPoint _acceleration{ 0.03, 0.1 };

	Renderer * _render{};

	Texture * _idle{};
	Texture * _run{};
	Texture * _roll{};
	Texture * _jump{};

	dir _lastDir = LEFT;
	bool _onGround = true;

	int _rollCountdown = 0;
	int _rollCounter = 8;
	int _startRollFrame = 0;

	u8 _color[4] = { 0, 255, 0, 255 };//krasses grün

	void draw( Point offset, const u32 frame, bool drawRect, bool drawColRect );
	void ini( Renderer * render, int RenderSize );
	FRect getCollisionRect(float RenderSize) const;
};


#endif //SDL_BASEGAME_PLAYER_H
