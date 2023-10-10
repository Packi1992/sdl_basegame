//
// Created by banoodle on 24.09.23.
//

#include <iostream>
#include <fstream>
#include <filesystem>
#include "tilestate.h"
#include <cmath>

void TileState::Init()
{
	GameState::Init();
	loadTextures();
	loadMap();
	_player.ini( render, RENDERSIZE );
	t1.set_pressed( true );
	// Setup Buttons
	Rect pos = { 5, 5, 202, 50 };
	t0.set( pos, 200, 200, 200, " NO TILESET", _font );
	pos.y += pos.h + 10;
	t1.set( pos, 200, 200, 200, "  TILESET 1", _font );
	pos.y += pos.h + 10;
	t2.set( pos, 200, 200, 200, "  TILESET 2", _font );
	pos.y += pos.h + 10;
	t3.set( pos, 200, 200, 200, "  TILESET 3", _font );
	pos.y += pos.h + 10;
	btn_show.set( pos, 200, 200, 200, "SHOW TILESET", _font );
	pos.y += pos.h + 10;
	btn_save.set( pos, 200, 200, 200, "    Save    ", _font );
	pos.y += pos.h + 10;
	btn_hint.set( pos, 200, 200, 200, "      ?     ", _font );
}

void TileState::Events( const u32 frame, const u32 totalMSec, const float deltaT )
{
	SDL_PumpEvents();

	Event event;
	while( SDL_PollEvent( &event ) )
	{
		if( event.type == SDL_MOUSEBUTTONDOWN )
			click = true;
		if( event.type == SDL_KEYDOWN )
		{
			switch( event.key.keysym.scancode )
			{
				case SDL_SCANCODE_C:
					_drawColRect = !_drawColRect;
					break;
				case SDL_SCANCODE_R:
					_drawRenderRect = !_drawRenderRect;
					break;
				case SDL_SCANCODE_N:
					_showTileNumbers = !_showTileNumbers;
					break;
				case SDL_SCANCODE_G:
					_showGrid = !_showGrid;
					break;
				default:
					break;
			}
		}
		if( game.HandleEvent( event ) )
			continue;
	}
	key_array = SDL_GetKeyboardState( nullptr );
	SDL_GetMouseState( &cursur.x, &cursur.y );
}

void TileState::Update( const u32 frame, const u32 totalMSec, const float deltaT )
{
	handleCollision();
	movePlayer( frame );
	(_player._rollCountdown - 1 > 0) ? _player._rollCountdown-- : _player._rollCountdown = 0;
	updateCam(); // Viewport!

	// close overlay with ESCAPE
	if( key_array[SDL_SCANCODE_ESCAPE] && btn_show.is_active() )
		btn_show.set_pressed( false );
	if( key_array[SDL_SCANCODE_ESCAPE] && btn_hint.is_active() )
		btn_hint.set_pressed( false );

	handleButtons();

	// tile selection (evaluate if cursor inside Tile Selection)
	if( btn_show.is_active() && click &&
	    cursur.x > _tilesetDst.x &&
	    cursur.x < _tilesetDst.x + _tilesetDst.w &&
	    cursur.y > _tilesetDst.y &&
	    cursur.y < _tilesetDst.y + _tilesetDst.h )
	{
		_selTile = (cursur.x - _tilesetDst.x) / 64 + 16 * ((cursur.y - _tilesetDst.y) / 64);
		click = false;
	}
	// map manipulation
	if( key_array[SDL_SCANCODE_RCTRL] && click )
	{
		// get relative position on map
		Point sel_map_entry = { (cursur.x + (int) cam.x) / RENDERSIZE, (960 - cursur.y) / RENDERSIZE };
		// evaluate if inside map Bounds
		if( sel_map_entry.x >= 0 && sel_map_entry.x < MAPWIDTH &&
		    sel_map_entry.y >= 0 && sel_map_entry.y < MAPHEIGHT )
			// actually change map data
			map[sel_map_entry.x][sel_map_entry.y] = _selTile;
		click = false;
	}
	// 60 frames per sec -> every 8 frames
	if( frame % 8 == 0 )
	{
		for( int x = 0; x < MAPWIDTH; x++ )
		{
			for( int y = 0; y < MAPHEIGHT; y++ )
			{
				// animate coin -> next image after 9 frames
				if( map[x][y] >= 5 && map[x][y] < 10 )
					map[x][y]++;
				else if( map[x][y] == 10 )
					map[x][y] = 5;
				if( frame % 16 == 0 )
				{
					if( map[x][y] >= 90 && map[x][y] < 94 )
						map[x][y]++;
					else if( map[x][y] == 94 )
						map[x][y] = 90;
				}
			}
		}
	}

	click = false;
}

void TileState::Render( const u32 frame, const u32 totalMSec, const float deltaT )
{
	drawBackground();

	drawMap();
	_player.draw( cam, frame, _drawRenderRect, _drawColRect );

	drawUi();

	// draw overlay
	drawTileMap();
	drawCollisions();
	drawHint();
}

void TileState::loadTextures()
{
	_tilesets[0] = loadTexture( BasePath "asset/graphic/pixel_platform_01.png" );
	_tilesets[1] = loadTexture( BasePath "asset/graphic/pixel_platform_02.png" );
	_tilesets[2] = loadTexture( BasePath "asset/graphic/pixel_platform_03.png" );
	_font = loadTexture( BasePath "asset/font/16x16_sm.png" );
	_bg[0] = loadTexture( BasePath "asset/graphic/sky_solid_color.png" );
	_bg[1] = loadTexture( BasePath "asset/graphic/mountain_depth_z_1.png" );
	_bg[2] = loadTexture( BasePath "asset/graphic/mountain_depth_z_2.png" );
	_clouds = loadTexture( BasePath "asset/graphic/clouds.png" );
}

Texture * TileState::loadTexture( std::string path )
{
	Texture * nt;

	nt = IMG_LoadTexture( render, path.c_str() );
	if( !nt )
	{
		print( "error loading %s image", path );
		return nullptr;
	}
	return nt;
}

void TileState::UnInit()
{
	GameState::UnInit();
	SDL_DestroyTexture( _tilesets[0] );
	SDL_DestroyTexture( _tilesets[1] );
	SDL_DestroyTexture( _tilesets[2] );
	SDL_DestroyTexture( _bg[0] );
	SDL_DestroyTexture( _bg[1] );
	SDL_DestroyTexture( _bg[2] );
	SDL_DestroyTexture( _clouds );
	SDL_DestroyTexture( _font );
}


void TileState::buildMap()
{
	for( int x = 0; x < MAPWIDTH; x++ )
	{
		for( int y = 0; y < MAPHEIGHT; y++ )
		{
			map[x][y] = 0;
			if( y < 3 )
			{
				map[x][y] = 100;
				if( (x == 0 || x == 39) )
					map[x][y] = 99;
			}
			if( y == 3 )
			{
				map[x][y] = 84;
				if( x == 0 )
					map[x][y] = 83;
				if( x == 39 )
					map[x][y] = 85;
			}
		}
	}
}

void TileState::loadMap()
{
	std::string line;
	std::ifstream iStream;
	if( std::filesystem::exists( BasePath "map.txt" ) )
	{
		iStream.open( BasePath "map.txt" );
		if( iStream.is_open() )
		{
			int y = MAPHEIGHT - 1;
			while( getline( iStream, line ) )
			{
				unsigned long token = line.find( ';' );
				for( int x = 0; x < MAPWIDTH; x++ )
				{
					if( line.empty() )
						map[x][y] = 0;
					int tile = (int) strtol( line.substr( 0, token ).c_str(), nullptr, 10 );
					map[x][y] = tile;
					line.erase( 0, token + 1 );
					token = line.find( ';' );
				}
				y--;
			}
			while( y >= 0 )
			{
				for( int x = 0; x < MAPWIDTH; x++ )
					map[x][y] = 0;
				y--;
			}
		}
	}
	else
	{
		buildMap();
		saveMap();
	}
}

void TileState::saveMap()
{
	std::ofstream oStream;
	oStream.open( BasePath "map.txt" );
	for( int y = MAPHEIGHT - 1; y >= 0; y-- )
	{
		for( int x = 0; x < MAPWIDTH; x++ )
		{
			oStream << (int) map[x][y] << ";";
		}
		oStream << "\n";
	}
	oStream.close();
}

void TileState::handleCollision()
{
	touchedTiles.clear();
	FRect colRect = _player.getCollisionRect( RENDERSIZE );
	// one of error
	touchedTiles.push_back({(int) colRect.x,(int) colRect.y-1});
	touchedTiles.push_back({(int) (colRect.x + colRect.w),(int) colRect.y-1});
	touchedTiles.push_back({(int) colRect.x,(int) (colRect.y + colRect.h)-1});
	touchedTiles.push_back({(int) (colRect.x + colRect.w),(int) (colRect.y + colRect.h)-1});
	for( Point t : touchedTiles )
	{
		if( map[t.x][t.y] >= 5 && map[t.x][t.y] <= 10 ) // Collision with Coin
		{
			map[t.x][t.y] = 0; // delete coin
		}
	}

}

void TileState::movePlayer( const u32 frame )
{
	// change acceleration
	if( key_array[SDL_SCANCODE_D] && !key_array[SDL_SCANCODE_A] )
		this->_player._speed.x < _player._maxSpeed.x ?
			this->_player._speed.x += _player._acceleration.x :
			this->_player._speed.x = _player._maxSpeed.x;
	if( key_array[SDL_SCANCODE_A] && !key_array[SDL_SCANCODE_D] )
		this->_player._speed.x < -_player._maxSpeed.x ? this->_player._speed.x -= _player._acceleration.y
		                                              : this->_player._speed.x = -_player._maxSpeed.x;
	// stop, if no button is pressed
	if( _player._speed.x != 0 && !key_array[SDL_SCANCODE_D] && !key_array[SDL_SCANCODE_A] )
		_player._speed.x * _player._speed.x > 0.3 ? _player._speed.x /= 2.0 : _player._speed.x = 0;
	// actually move
	// collect position data
	FRect collisionRect = _player.getCollisionRect( RENDERSIZE );
	if( _player._speed.x > 0 )
	{
		// moving right
		float xam = collisionRect.x + collisionRect.w + _player._speed.x; // rightmost x after movement
		int xamMap = (int) xam;
		bool top = tileCollideAble( map[xamMap][(int) collisionRect.y] );
		bool bot = tileCollideAble( map[xamMap][(int) (collisionRect.y - collisionRect.h)] );
		if( top || bot )
		{
			_player._pos.x = (float) xamMap - collisionRect.w * 1.5f;
			_player._speed.x = 0;
			std::cout << "Collision-Right" << std::endl;
		}
		else
			_player._pos.x += _player._speed.x;
	}
	if( _player._speed.x < 0 )
	{
		// moving left
		float xam = collisionRect.x + _player._speed.x; // leftmost x after movement
		int xamMap = (int) xam;
		bool top = tileCollideAble( map[xamMap][(int) collisionRect.y] );
		bool bot = tileCollideAble( map[xamMap][(int) (collisionRect.y - collisionRect.h)] );
		if( top || bot )
		{
			_player._pos.x = (float) xamMap + 1 - collisionRect.w * 0.5f;
			_player._speed.x = 0;
			std::cout << "Collision-Left" << std::endl;
		}
		else
			_player._pos.x += _player._speed.x;

	}
	if( _player._speed.y > 0 )
	{
		// moving up
		_player._onGround = false;
		float yam = collisionRect.y + _player._speed.y; // topmost y after movement
		int yamMap = (int) yam;
		bool left = tileCollideAble( map[(int) collisionRect.x][yamMap] );
		bool right = tileCollideAble( map[(int) (collisionRect.y - collisionRect.h)][yamMap] );
		if( left || right )
		{
			_player._pos.y = (float) yamMap - collisionRect.h * 0.5f;
			_player._speed.y = 0;
			std::cout << "Collision-Top" << std::endl;
		}
		else
			_player._pos.y += _player._speed.y;
	}
	if( _player._speed.y <= 0 )
	{
		// moving down
		float yam = collisionRect.y - collisionRect.h - 0.9f + _player._speed.y; // bot most y after movement
		int yamMap = (int) yam;
		bool left = tileCollideAble( map[(int) collisionRect.x][yamMap] );
		bool right = tileCollideAble( map[(int) (collisionRect.x + collisionRect.w)][yamMap] );
		if( left || right )
		{
			_player._pos.y = (float) yamMap + (float) _player._size.y / RENDERSIZE * 0.89f;
			_player._speed.y = 0;
			std::cout << "Collision-Bot" << std::endl;
			// hitting Ground
			_player._onGround = true;
		}
		else
		{
			_player._onGround = false;
			_player._pos.y += _player._speed.y;
		}
	}
	if( !_player._onGround )
	{
		// Accurately calculate the gravitational influence
		_player._speed.y -= _player._acceleration.y / 8;
		if( _player._speed.y < -_player._maxSpeed.y )
			_player._speed.y = -_player._maxSpeed.y;
	}
	else if( key_array[SDL_SCANCODE_SPACE] )
	{
		_player._speed.y = _player._maxSpeed.y;
		_player._onGround = false;
	}

	// pos.x 0 is left most position
	if( _player._pos.x < 0 )
	{
		_player._pos.x = 0;
		_player._speed.x = 0;
	}
	if( _player._rollCountdown == 0 && key_array[SDL_SCANCODE_LCTRL] )
	{
		_player._rollCounter = 0;
		_player._startRollFrame = (int) frame;
		_player._rollCountdown = 400;
	}
	if( _player._speed.x > 0 )
		_player._lastDir = Player::RIGHT;
	if( _player._speed.x < 0 )
		_player._lastDir = Player::LEFT;

}

void TileState::updateCam() // need more fixes
{
	if( ((int) (_player._pos.x * RENDERSIZE) - cam.x) > SCREENWIDTH - 150 )
	{
		if( _player._speed.x > 0 )
			cam.x += (int) (RENDERSIZE * _player._speed.x);
		else
			cam.x -= (int) (RENDERSIZE * _player._speed.x);
	}
	if( (int) (_player._pos.x * RENDERSIZE) - cam.x < 150 )
	{
		if( _player._speed.x < 0 )
			cam.x += (int) (RENDERSIZE * _player._speed.x);
		if( cam.x < 0 )
			cam.x = 0;
	}
}

void TileState::handleButtons()
{
	if( click && t0.is_clicked( &cursur ) )
	{
		t1.set_pressed( false );
		t2.set_pressed( false );
		t3.set_pressed( false );
		click = false;
	}
	if( click && t1.is_clicked( &cursur ) )
	{
		t0.set_pressed( false );
		t2.set_pressed( false );
		t3.set_pressed( false );
		click = false;
	}
	if( click && t2.is_clicked( &cursur ) )
	{
		t0.set_pressed( false );
		t1.set_pressed( false );
		t3.set_pressed( false );
		click = false;
	}
	if( click && t3.is_clicked( &cursur ) )
	{
		t0.set_pressed( false );
		t1.set_pressed( false );
		t2.set_pressed( false );
		click = false;
	}
	if( click && btn_show.is_clicked( &cursur ) )
	{
		click = false;
	}
	if( click && btn_save.is_clicked( &cursur ) )
	{
		saveMap();
		btn_save.set_pressed( false );
		click = false;
	}
	if( click && btn_hint.is_clicked( &cursur ) )
	{
		click = false;
	}
}

void TileState::drawNumber( int x, int y, Rect * dst, int number )
{
	std::string text = std::to_string( number );
	drawCenterText( x, y, dst, text );
}

void TileState::drawCenterText( int x, int y, Rect * dst, const std::string & text )
{
	int nx = x + (dst->w - (int) text.size() * 16) / 2;
	int ny = y + (dst->h - 16) / 2;
	drawText( nx, ny, text );
}

void TileState::drawText( int x, int y, std::string text )
{
	Rect dest = { x, y, 16, 16 };
	Rect src = { 0, 0, 16, 16 };
	for( int i = 0; i < (int) text.size(); i++ )
	{
		int ascii_value = (u_char) text[i];
		int row = ascii_value / 16 * 16;
		int col = ascii_value % 16 * 16;
		src.x = col;
		src.y = row;
		SDL_RenderCopy( render, _font, &src, &dest );
		dest.x += dest.w;
	}
}


void TileState::drawHint()
{
	if( !btn_hint.is_active() )
		return;

	SDL_SetRenderDrawColor( render, 0, 0, 0, 255 ); // Black
	Rect rHint = { 250, 50, SCREENWIDTH - 300, SCREENHEIGHT - 100 };
	SDL_RenderFillRect( render, &rHint );
	SDL_SetRenderDrawColor( render, 255, 255, 255, 255 ); // White
	SDL_RenderDrawRect( render, &rHint );
	std::string line;
	std::ifstream iStream;
	int x_start = rHint.x + 10;
	int y = rHint.y + 20;

	if( std::filesystem::exists( BasePath "hint.txt" ) )
	{
		iStream.open( BasePath "hint.txt" );
		if( iStream.is_open() )
		{
			while( getline( iStream, line ) )
			{
				drawText( x_start, y, line );
				y += 20;
			}
		}
	}
}

void TileState::drawBackground()
{
	// Draw Sky - No Viewport induced Movement
	SDL_RenderCopy( render, _bg[0], nullptr, &_bg0 );
	// Draw Mountains 1 far away (1/10) Movement
	// Draw mid BG
	_bg1.x = (_bg1.w - (cam.x / 10)) % _bg1.w;
	SDL_RenderCopy( render, _bg[1], nullptr, &_bg1 );
	// Draw left BG
	_bg1.x -= _bg1.w;
	SDL_RenderCopy( render, _bg[1], nullptr, &_bg1 );
	// Draw right BG
	_bg1.x += _bg1.w + _bg1.w;
	SDL_RenderCopy( render, _bg[1], nullptr, &_bg1 );
	// Draw Mountains 2 (1/5) Movement
	_bg2.x = (_bg2.w - (cam.x / 5)) % _bg2.w;
	SDL_RenderCopy( render, _bg[2], nullptr, &_bg2 );
	_bg2.x -= _bg2.w;
	SDL_RenderCopy( render, _bg[2], nullptr, &_bg2 );
	_bg2.x += _bg2.w + _bg2.w;
	SDL_RenderCopy( render, _bg[2], nullptr, &_bg2 );
	// Draw Clouds (1/3) Movement
	_rClouds.x = (_rClouds.w - (cam.x / 3)) % _rClouds.w;
	SDL_RenderCopy( render, _clouds, nullptr, &_rClouds );
	_rClouds.x -= _rClouds.w;
	SDL_RenderCopy( render, _clouds, nullptr, &_rClouds );
	_rClouds.x += _rClouds.w + _rClouds.w;
	SDL_RenderCopy( render, _clouds, nullptr, &_rClouds );

}

void TileState::drawMap()
{
	Texture * tileset = nullptr;
	if( t1.is_active() )
		tileset = _tilesets[0];
	if( t2.is_active() )
		tileset = _tilesets[1];
	if( t3.is_active() )
		tileset = _tilesets[2];

	Rect dst = { 0, 0, RENDERSIZE, RENDERSIZE };
	Rect src = { 0, 0, TILESIZE, TILESIZE };
	for( int x = 0; x < MAPWIDTH; x++ )
	{
		for( int y = 0; y < MAPHEIGHT; y++ )
		{
			int tile = (int) map[x][y];
			src.x = tile % 16 * TILESIZE;
			src.y = tile / 16 * TILESIZE;
			dst.x = x * RENDERSIZE - (int) cam.x;
			dst.y = SCREENHEIGHT - ((y + 1) * RENDERSIZE);
			if( (dst.x > (-RENDERSIZE)) && dst.x < SCREENWIDTH && tile != 0 )
			{
				if( tileset == nullptr )
				{
					SDL_SetRenderDrawColor( render, 0, 0, 0, 255 );
					SDL_RenderFillRect( render, &dst );
				}
				else
					SDL_RenderCopy( render, tileset, &src, &dst );
				if( _showTileNumbers )
				{
					SDL_SetRenderDrawColor( render, 255, 255, 255, 255 );
					drawNumber( dst.x, dst.y, &dst, tile );
				}
			}
		}
	}
	if( _showGrid )
	{
		SDL_SetRenderDrawColor( render, 0, 255, 0, 255 );
		for( int x = 0; x < MAPWIDTH; x++ )
		{
			for( int y = 0; y < MAPHEIGHT; y++ )
			{
				dst.x = x * 64 - (int) cam.x;
				dst.y = 960 - ((y + 1) * 64);
				if( (dst.x > (-64)) && dst.x < 1280 )
				{
					SDL_RenderDrawRect( render, &dst );
				}
			}
		}
	}
}

void TileState::drawTileMap()
{
	if( !btn_show.is_active() )
		return;

	SDL_SetRenderDrawColor( render, 0, 0, 0, 255 );
	SDL_RenderFillRect( render, &_tilesetDst );
	SDL_RenderCopy( render, _tilesets[0], nullptr, &_tilesetDst );
	SDL_SetRenderDrawColor( render, 0, 0, 0, 255 );
	SDL_RenderFillRect( render, &_selTileDst );
	Rect src = { 0, 0, 32, 32 };
	src.x = _selTile % 16 * 32;
	src.y = _selTile / 16 * 32;
	SDL_RenderCopy( render, _tilesets[0], &src, &_selTileDst );
}

void TileState::drawUi()
{
	t0.draw( render );
	t1.draw( render );
	t2.draw( render );
	t3.draw( render );
	btn_show.draw( render );
	btn_save.draw( render );
	btn_hint.draw( render );
}

bool TileState::tileCollideAble( u8 i )
{
	if( i >= 5 && i <= 10 ) // coins
		return false;
	if( (i >= 23 && i <= 26) || i == 41 || i == 42 )
		return false; // greens
	if( i == 39 ) // mushroom
		return false;
	if( i == 124 || i == 126 || i == 141 || i == 143 ) // signs
		return false;
	return i != 0;
}

void TileState::drawCollisions()
{
	if(!_drawColRect)
		return;
	// draw Rects with possible collisions
	Rect colRect= {0,0,RENDERSIZE,RENDERSIZE};
	SDL_SetRenderDrawColor(render,255,0,0,255);
	for(Point p : touchedTiles){
		colRect.x = p.x*RENDERSIZE-cam.x;
		colRect.y = SCREENHEIGHT-(p.y*RENDERSIZE+1);
		SDL_RenderDrawRect(render,&colRect);
	}
}
