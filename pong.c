#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>
#include "pong.h"
#include "pong_platform.h"
#include "pong_draw.h"
#include "pong_logic.h"

const diff_t difficulties[3] = {
    // base_speed, speed_increase_interval, speed_increase_amount, pad_length
    { 150, 4, .9, 6 },  // EASY
    { 100, 2, .9, 5 }, // MEDIUM
    { 50,  1, .9, 4 } // HARD
};

Game init_game(diff_name diff){
    Game game;
    game.diff    = diff;
    game.bounces = 0;
    game.speed   = difficulties[diff].base_speed;

    // hard coded loop over 2 values because fuck you that's why
    game.players[0].is_player = true;
    game.players[0].len       = difficulties[diff].pad_length;
    game.players[0].pos       = (GAME_HEIGHT - game.players[0].len) / 2;
    game.players[0].score     = 0;

    game.players[1].is_player = false;
    game.players[1].len       = difficulties[diff].pad_length;
    game.players[1].pos       = (GAME_HEIGHT - game.players[1].len) / 2;
    game.players[1].score     = 0;

    // TODO: this code is used elsewhere too (score_and_reset). reduce, reuse, refactor
    game.ball.x       = CONSOLE_W / 2;
    game.ball.y       = CONSOLE_H / 2;
    game.ball.right   = rand() & 1;
    game.ball.up      = rand() & 1;
    game.ball.slope   = -2;

    return game;
}

int main(){
    platform_init();
    CHIDE();
    srand(time(0));

    Game game = init_game(HARD);
    draw_blank();
    draw_game(game);
    while(game.players[0].score < SCORE_LIMIT && game.players[1].score < SCORE_LIMIT){
        move_ball(&game);

        if(w_pressed())
            player_up(&game, 0);
        if(s_pressed())
            player_down(&game, 0);

        if(up_pressed())
            player_up(&game, 1);
        if(down_pressed())
            player_down(&game, 1);

        // bot control:
        /*if(game.players[1].pos + game.players[1].len / 2 > game.ball.y)
            player_up(&game, 1);
        else if (game.players[1].pos + game.players[1].len / 2 < game.ball.y)
            player_down(&game, 1);*/

        sleep(game.speed/1000.);
    }
    CPOS(1, 1);
    CLS();

    if(game.players[0].score >= SCORE_LIMIT)
      wprintf(L"Left player won!\n");
    if(game.players[1].score >= SCORE_LIMIT)
      wprintf(L"Right player won!\n");

    while(getchar() != '\n'); // flush input buffer
    UP(1);
    wprintf(L"\033[1M");      // clear line

    wprintf(L"\033[!p"); // soft reset cursor properties
}
