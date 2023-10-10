//
// Created by banoodle on 25.09.23.
//

#include <iostream>
#include "player.h"

void Player::draw( Point offset, const u32 frame, bool drawRect = false , bool drawColRect=false)
{
	Rect p = { (int)(_pos.x * 64 - (float)offset.x), (int) (960 - ((_pos.y + 1) * 64)),
	           (int) _size.x, (int) _size.y };
	if( _render == nullptr || drawRect)
	{
		if(_render == nullptr)
			std::cerr << "you need to ini Player first";
		SDL_SetRenderDrawColor( _render, _color[0], _color[1], _color[2], _color[3] );
		// do calculations to get proper pos on screen
		SDL_RenderDrawRect( _render, &p );
	}
	if( drawColRect){
		SDL_SetRenderDrawColor( _render, _color[0], _color[1], _color[2], _color[3] );
		FRect cRect = getCollisionRect( 64);
		cRect.x = cRect.x * 64 - (float)offset.x;
		cRect.y = (960 - ((cRect.y) * 64));
		cRect.h *= 64;
		cRect.w *= 64;
		SDL_RenderDrawRectF(_render,&cRect);
	}
	Rect src = { 0, 0, 48, 48 };
	if( _rollCounter < 6 ){
		_rollCounter = ((int) (frame - _startRollFrame) / 6) % 7;
		src.x = _rollCounter * 48;
		SDL_RenderCopyEx( _render, _roll, &src, &p,0, nullptr,
		                  _lastDir==RIGHT?SDL_RendererFlip::SDL_FLIP_NONE:SDL_RendererFlip::SDL_FLIP_HORIZONTAL);

	}
	else{
		if( _speed.y != 0 ){
			if( _speed.y < - 0.5*_maxSpeed.y )
				src.x = 0;
			else
				src.x = 2 * 48;
			SDL_RenderCopyEx( _render, _jump, &src, &p,0, nullptr,
							  _lastDir==RIGHT?SDL_RendererFlip::SDL_FLIP_NONE:SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
		}
		else{
			if( _speed.x != 0 ){
				src.x = (int) ((frame / 6) % 8) * 48;
				SDL_RenderCopyEx( _render, _run, &src, &p,0, nullptr,
				                  _lastDir==RIGHT?SDL_RendererFlip::SDL_FLIP_NONE:SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
			}
			else{
				src.x = (int) ((frame / 6) % 8) * 48;
				SDL_RenderCopyEx( _render, _idle, &src, &p,0, nullptr,
				                  _lastDir==RIGHT?SDL_RendererFlip::SDL_FLIP_NONE:SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
			}
		}


	}
}

void Player::ini( Renderer * render, int RenderSize )
{
	_render = render;
	_idle = loadTexture( BasePath"asset/graphic/Character Idle 48x48.png" );
	_run = loadTexture( BasePath"asset/graphic/PlayerWalk 48x48.png" );
	_roll = loadTexture( BasePath"asset/graphic/Player Roll 48x48.png" );
	_jump = loadTexture( BasePath"asset/graphic/player jump 48x48.png" );
	_mapSize.x = (float)(_size.x*0.6) / (float)RenderSize;
	_mapSize.y = (float)(_size.y*0.6) / (float)RenderSize;
}

Texture * Player::loadTexture( std::string path ) const
{
	if( _render == nullptr )
	{
		std::cerr << "you need to ini Player first";
		return nullptr;
	}

	Texture * nt;

	nt = IMG_LoadTexture( _render, path.c_str() );
	if( !nt )
	{
		print( "error loading %s image", path );
		return nullptr;
	}
	return nt;
}

FRect Player::getCollisionRect(float RenderSize) const
{
	float x = _pos.x + (_size.x * 0.25f) / RenderSize;
	float y = _pos.y + (_size.y * 0.4f) / RenderSize;
	float w = (_size.x * 0.5f)/RenderSize;
	float h = (_size.y * 0.65f)/RenderSize;
	return {x,y,w,h};
}
