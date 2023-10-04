//
// Created by banoodle on 26.09.23.
//

#include "button.h"

void Button::set( Rect pos, u8 cr, u8 cg, u8 cb, std::string text, Texture * ascii )
{
	_pos = pos;
	_color[0] = cr;
	_color[1] = cg;
	_color[2] = cb;
	_color[3] = 255;
	_text = text;
	_ascii = ascii;
}

bool Button::is_clicked( Point * Cursor )
{
	if( Cursor->x >= _pos.x && Cursor->x <= _pos.x + _pos.w && Cursor->y >= _pos.y && Cursor->y <= _pos.y + _pos.h )
	{
		_pressed = !_pressed;
		return true;
	}
	return false;
}

void Button::draw( Renderer * render )
{
	if( _ascii == nullptr )
		return;
	SDL_SetRenderDrawBlendMode( render, SDL_BLENDMODE_BLEND );
	if( _pressed )
		SDL_SetRenderDrawColor( render, _color[0], _color[1], _color[2], _color[3] );
	else
		SDL_SetRenderDrawColor( render, _color[0], _color[1], _color[2], 50 );
	SDL_RenderFillRect( render, &_pos );
	SDL_SetRenderDrawBlendMode( render, SDL_BLENDMODE_NONE );
	Rect dest = { _pos.x + 5, _pos.y + _pos.h / 2 - 8, 16, 16 };
	Rect src = { 0, 0, 16, 16 };
	for( int i = 0; i < (int) _text.size(); i++ )
	{
		int ascii_value = _text[i];
		int row = ascii_value / 16 * 16;
		int col = ascii_value % 16 * 16;
		src.x = col;
		src.y = row;
		SDL_RenderCopy( render, _ascii, &src, &dest );
		dest.x += dest.w;
	}
}

void Button::set_pressed( bool pressed )
{
	_pressed = pressed;
}

bool Button::is_active()
{
	return _pressed;
}
