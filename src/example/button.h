//
// Created by banoodle on 26.09.23.
//

#ifndef SDL_BASEGAME_BUTTON_H
#define SDL_BASEGAME_BUTTON_H

#include "../gamebase.h"

class Button
{
protected:
	bool _pressed = false;
	u8 _color[4] = { 0, 0, 0, 255 };
	Rect _pos{};
	std::string _text{};
	Texture * _ascii{};
public:
	void set( Rect pos, u8 cr, u8 cg, u8 cb, std::string text, Texture * ascii );
	bool is_clicked( Point * Cursor );
	void draw( Renderer * render );
	void set_pressed( bool pressed );
	bool is_active();
};


#endif //SDL_BASEGAME_BUTTON_H
