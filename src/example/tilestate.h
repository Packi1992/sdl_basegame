#include "gamebase.h"

//
// Created by banoodle on 24.09.23.
//
#ifndef SDL_BASEGAME_TILESTATE_H
#define SDL_BASEGAME_TILESTATE_H

#include "player.h"
#include "button.h"

#define MAPWIDTH 40
#define MAPHEIGHT 14
#define RENDERSIZE 64
#define TILESIZE 32
#define SCREENWIDTH 1280
#define SCREENHEIGHT 960

class TileState : public GameState
{
protected:
	// rendering
	Texture * _tilesets[3] = { nullptr };
	Texture * _bg[3] = { nullptr };
	Texture * _clouds = nullptr;
	Texture * _font = nullptr;
	Rect _bg0 = { 0, 0, 1280, 806 };
	Rect _bg1 = { 0, 200, 1280, 806 };
	Rect _bg2 = { 0, 250, 1280, 806 };
	Rect _rClouds = { 0, 0, 1280, 250 };
	Rect _tilesetDst = { 250, 10, 1024, 640 };
	Rect _selTileDst = { 5, 440, 200, 200 };

	// input handling
	Point cursur{};
	bool click = false;
	const u8 * key_array = SDL_GetKeyboardState( nullptr );
	bool _showGrid = false;
	bool _showTileNumbers = false;
	bool _drawColRect = false;
	bool _drawRenderRect = false;

	// update and data
	int _selTile = 0;
	Player _player;
	Point cam{ 0, 0 };
	Button t0, t1, t2, t3;
	Button btn_show, btn_save, btn_hint;
	u8 map[MAPWIDTH][MAPHEIGHT] = {};
	Vector<Point> touchedTiles{};

	void loadTextures();
	void buildMap();

	Texture * loadTexture( std::string path );
	void loadMap();
	void saveMap();
	void movePlayer( const u32 frame );
	void updateCam();
	void handleButtons();

	void drawTileMap();
	void drawMap();
	void drawText(int x, int y, std::string text);
	void drawCenterText( int x, int y, Rect * dst, const std::string& text );
	void drawNumber( int x, int y, Rect * dst, int number );
	void drawHint();
	void drawUi();

public:
	// ctor
	using GameState::GameState;

	void Init() override;
	void UnInit() override;

	void Events( const u32 frame, const u32 totalMSec, const float deltaT ) override;
	void Update( const u32 frame, const u32 totalMSec, const float deltaT ) override;
	void Render( const u32 frame, const u32 totalMSec, const float deltaT ) override;

	void drawBackground();
	void handleCollision();
	static bool tileCollideAble( u8 i );
	void drawCollisions();
};


#endif //SDL_BASEGAME_TILESTATE_H
