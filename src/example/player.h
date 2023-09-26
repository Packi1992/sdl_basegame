//
// Created by banoodle on 25.09.23.
//

#ifndef SDL_BASEGAME_PLAYER_H
#define SDL_BASEGAME_PLAYER_H
#include "../gamebase.h"

class Player
{
	void draw(Renderer *render);
public:
	void move(u8 **key_array);
};


#endif //SDL_BASEGAME_PLAYER_H
