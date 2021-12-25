#ifndef _PONG_LOGIC_H
#define _PONG_LOGIC_H

// write bounce info to stderr (use 2>log.txt)
#define _DEBUG_COLLISION

void   player_up(Game* game, short p);
void player_down(Game* game, short p);
void move_ball(Game* game);

#endif
