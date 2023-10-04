//
// Created by banoodle on 25.09.23.
//

#include <iostream>
#include "player.h"

void Player::draw( Point offset, const u32 frame )
{
	Rect p = { (int) (pos.x - _size.x / 2 - (float) offset.x), (int) (pos.y - _size.y + (float) offset.y / 2),
	           (int) _size.x, (int) _size.y };
	if( _render == nullptr )
	{
		std::cerr << "you need to ini Player first";
		SDL_SetRenderDrawColor( _render, _color[0], _color[1], _color[2], _color[3] );
		// do calculations to get proper pos on screen
		SDL_RenderDrawRect( _render, &p );
	}
	Rect src = { 0, 0, 48, 48 };
	if( _rollCounter < 6 ){
		_rollCounter = ((int) (frame - _startRollFrame) / 6) % 7;
		src.x = _rollCounter * 48;
		SDL_RenderCopy( _render, _roll, &src, &p );
	}
	else{
		if( _speed.x > 0.0 ){
			src.x = (int) (((frame) / 6) % 8) * 48;
			SDL_RenderCopy( _render, _run, &src, &p );
		}
		if( _speed.x < 0.0 ){
			src.x = (int) ((frame / 6) % 8) * 48;
			SDL_RenderCopyEx( _render, _run, &src, &p, 180, nullptr, SDL_RendererFlip::SDL_FLIP_VERTICAL );
		}
		if( _speed.x == 0 ){
			src.x = (int) ((frame / 6) % 8) * 48;
			SDL_RenderCopy( _render, _idle, &src, &p );
		}
		if( _speed.y != 0 ){
			if( _speed.y < -20 )
				src.x = 0;
			if( _speed.y > -5 || _speed.y < 5 )
				src.x = 1 * 48;
			if( _speed.y > 5 )
			{
				src.x = 2 * 48;
			}
			src.x = (int) ((frame / 6) % 3) * 48;
			SDL_RenderCopy( _render, _jump, &src, &p );
		}
	}
}

void Player::ini( Renderer * render )
{
	_render = render;
	_idle = loadTexture( BasePath"asset/graphic/Character Idle 48x48.png" );
	_run = loadTexture( BasePath"asset/graphic/PlayerWalk 48x48.png" );
	_roll = loadTexture( BasePath"asset/graphic/Player Roll 48x48.png" );
	_jump = loadTexture( BasePath"asset/graphic/player jump 48x48.png" );
}

Texture * Player::loadTexture( std::string path )
{
	if( _render == nullptr )
	{
		std::cerr << "you need to ini Player first";
		return nullptr;
	}

	Texture * nt{};

	nt = IMG_LoadTexture( _render, path.c_str() );
	if( !nt )
	{
		print( "error loading %s image", path );
		return nullptr;
	}
	return nt;
}
