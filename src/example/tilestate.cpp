//
// Created by banoodle on 24.09.23.
//

#include "tilestate.h"

void Tilestate::Init()
{
	GameState::Init();
	loadTextures();

}

void Tilestate::loadTextures()
{
	tilesets[0] = loadTexture(BasePath "asset/graphic/pixel_platform_01.png");
	tilesets[1] = loadTexture(BasePath "asset/graphic/pixel_platform_02.png");
	tilesets[2] = loadTexture( BasePath "asset/graphic/pixel_platform_03.png");
	bg[0] = loadTexture(BasePath "asset/graphic/sky_solid_color.png");
	bg[1] = loadTexture( BasePath "asset/graphic/mountain_depth_z_1.png");
	bg[2] = loadTexture(BasePath "asset/graphic/mountain_depth_z_2.png");
	clouds = loadTexture(BasePath"assets/graphic/clouds.png");
}

Texture* Tilestate::loadTexture( std::string path )
{
	Texture *nt = nullptr;
	nt = IMG_LoadTexture( render, BasePath "asset/graphic/pixel_platform_01.png" );
	if(!nt){
		print("error loading %s image",path);
		return nullptr;
	}
	return nt;
}

void Tilestate::UnInit()
{
	GameState::UnInit();
	SDL_DestroyTexture(tilesets[0]);
	SDL_DestroyTexture(tilesets[1]);
	SDL_DestroyTexture(tilesets[2]);
	SDL_DestroyTexture(bg[0]);
	SDL_DestroyTexture(bg[1]);
	SDL_DestroyTexture(bg[2]);
	SDL_DestroyTexture(clouds);
}

void Tilestate::Events( const u32 frame, const u32 totalMSec, const float deltaT )
{
	SDL_PumpEvents();

	Event event;
	while( SDL_PollEvent( &event ) )
	{
		if( game.HandleEvent( event ) )
			continue;
	}
	key_array = SDL_GetKeyboardState( nullptr );
}

void Tilestate::Update( const u32 frame, const u32 totalMSec, const float deltaT )
{
	player.move( const_cast<u8 **>(&key_array) );
}

void Tilestate::Render( const u32 frame, const u32 totalMSec, const float deltaT )
{

}
