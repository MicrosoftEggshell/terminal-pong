#include <stdio.h>
#include <stdlib.h>
#include "pong_platform.h"
#include "pong.h"
#include "pong_draw.h"
#include "pong_logic.h"

void player_up(Game* game, short p){ // moves and draws!
    // assume 0 <= p < 2
    if(game->players[p].pos <= 0)
        return;

    short cx;
    switch(p){
        case 0:
            cx = 2;
            break;
        case 1:
            cx = CONSOLE_W - 1;
            break;
        default:
            fwprintf(stderr, L"idiot");
            exit(-10);
    }

    // ALL MORTAL FOOLS WHO QUESTIONS THE MAGIC CONSTANTS SHALL PERISH
    // they just work, ok? trial and error never lies.
    CPOS(cx, HEADER_HEIGHT + 2 + game->players[p].pos + game->players[p].len);
    putwchar(L' '); // delete bottom character of pad
    --game->players[p].pos;
    CPOS(cx, HEADER_HEIGHT + 3 + game->players[p].pos);
    putwchar(CH_VPIPE);
}

void player_down(Game* game, short p){ // moves and draws!
    // assume 0 <= p < 2
    if(game->players[p].pos + game->players[p].len >= GAME_HEIGHT)
        return;

    short cx;
    switch(p){
        case 1:
            cx = CONSOLE_W - 1;
            break;
        case 0:
            cx = 2;
            break;
        default:
            fwprintf(stderr, L"idiot");
            exit(-10);
    }

    CPOS(cx, HEADER_HEIGHT + 3 + game->players[p].pos);
    putwchar(L' '); // delete top character of pad
    ++game->players[p].pos;
    CPOS(cx, HEADER_HEIGHT + 2 + game->players[p].pos + game->players[p].len);
    putwchar(CH_VPIPE);
}

static bool can_bounce_to(Game game, short raw_newx, short raw_newy){
    /**
      * only side bounces, no top or bottom
      * new x and y should come before bounce has been calculated (e.g. offsceen positions)
      * TODO: left and right side bounces should be treated separately throughout all of the program, I think
      **/

    short pad;       // index of player on the same side as the ball
    double bounce_y; // weighted average of old and new y's (collision point)

    if(raw_newx <= 0){ // left side bounce
        pad = 0;
        //                                          ↓        raw_newx is negative       ↓
        bounce_y = ((double)game.ball.x*game.ball.y - raw_newx*raw_newy) / (game.ball.x - raw_newx);
    }
    else if (raw_newx >= CONSOLE_W-2){ // right side bounce
        pad = 1;
        bounce_y = ((double)((CONSOLE_W-2) - game.ball.x)*game.ball.y + (raw_newx - (CONSOLE_W-2))*raw_newy) / (raw_newx - game.ball.x);
    }
    else{ // no need to bounce
        return -1;
    }

    // 0.000001: correction for floating point stuff
    return bounce_y >= game.players[pad].pos - 0.500001 &&
           bounce_y <= game.players[pad].pos + game.players[pad].len + 0.500001;
}

void score_and_reset(Game* game, short player){
    ++game->players[player].score;
    draw_score(*game);
    game->bounces = 0;
    game->speed   = difficulties[game->diff].base_speed;

    game->ball.x       = CONSOLE_W / 2;
    game->ball.y       = CONSOLE_H / 2;
    game->ball.right   = rand() & 1;
    game->ball.up      = rand() & 1;
    game->ball.slope   = -2;

    sleep(1);
}

static bool side_bounce(Game* game, short* newx, short* newy){
    /**
      * bounce off of side walls if necessary and possible
      * returns 1 if a player got scored on, 0 otherwise
      * yes, this violates the SRP
      **/
    if(*newx >= CONSOLE_W - 2){
        if(can_bounce_to(*game, *newx, *newy)){
            #ifdef _DEBUG_COLLISION
            fwprintf(stderr, L"BOUNCE RIGHT\nx_from: %d\tx_to: %d\ny_from: %d\ty_to: %d\npad_from: %d\tpad_to: %d\n\n",
                game->ball.x,
                *newx,
                game->ball.y,
                *newy,
                game->players[1].pos,
                game->players[1].pos + game->players[1].len
            );
            #endif
            *newx = 2*CONSOLE_W - 5 - (*newx);
            game->ball.right = 0;
        }
        else{
            #ifdef _DEBUG_COLLISION
            fwprintf(stderr, L"SCORE RIGHT\nx_from: %d\tx_to: %d\ny_from: %d\ty_to: %d\npad_from: %d\tpad_to: %d\n\n",
                game->ball.x,
                *newx,
                game->ball.y,
                *newy,
                game->players[1].pos,
                game->players[1].pos + game->players[1].len
            );
            #endif
            score_and_reset(game, 0);
            return 1;
        }
    }
    else if(*newx <= 0){
        if(can_bounce_to(*game, *newx, *newy)){
            #ifdef _DEBUG_COLLISION
            fwprintf(stderr, L"BOUNCE LEFT\nx_from: %d\tx_to: %d\ny_from: %d\ty_to: %d\npad_from: %d\tpad_to: %d\n\n",
                game->ball.x,
                *newx,
                game->ball.y,
                *newy,
                game->players[1].pos,
                game->players[1].pos + game->players[1].len
            );
            #endif
            *newx = -1 * (*newx) + 3;
            game->ball.right = 1;
        }
        else{
            #ifdef _DEBUG_COLLISION
            fwprintf(stderr, L"SCORE LEFT\nx_from: %d\tx_to: %d\ny_from: %d\ty_to: %d\npad_from: %d\tpad_to: %d\n\n",
                game->ball.x,
                *newx,
                game->ball.y,
                *newy,
                game->players[1].pos,
                game->players[1].pos + game->players[1].len
            );
            #endif
            score_and_reset(game, 1);
            return 1;
        }
    }
    return 0;
}

static void speed_up_maybe(Game* game, short newx){
  if (newx > 0 && newx < CONSOLE_W - 2)
    return;
  ++game->bounces; // TODO: NOT THIS
  if(game->bounces % difficulties[game->diff].speed_increase_interval == 0)
      game->speed *= difficulties[game->diff].speed_increase_amount;
}


void move_ball(Game* game){ // TODO: refactor this (it just kept growing, HELP)

    // erase from old position
    CPOS(game->ball.x + 1, game->ball.y + HEADER_HEIGHT + 2);
    putwchar(L' ');

    short newx = game->ball.x;
    short newy = game->ball.y;
    if(game->ball.slope >= 0){
        if(game->ball.right)
            newx += 1;
        else
            newx -= 1;

        if(game->ball.up)
            newy -= game->ball.slope; // decrease y (slope > 0)
        else
            newy += game->ball.slope; // increase y
    }
    else {
        if(game->ball.right)
            newx -= game->ball.slope; // increase x (slope < 0)
        else
            newx += game->ball.slope; // decrease x

        if(game->ball.up)
            newy -= 1;
        else
            newy += 1;
    }

    speed_up_maybe(game, newx);

    // bounce off of top or bottom if necessary
    if(newy >= GAME_HEIGHT){
        newy = 2*GAME_HEIGHT - newy;
        game->ball.up = 1;
    } else if(newy <= 0){
        newy = -1 * newy + 2;
        game->ball.up = 0;
    }

    bool scored = side_bounce(game, &newx, &newy);
    if(scored)
      return;

    game->ball.x = newx;
    game->ball.y = newy;

    // draw to new position
    CPOS(game->ball.x + 1, game->ball.y + HEADER_HEIGHT + 2);
    putwchar(CH_BALL);
}
