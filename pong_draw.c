#include <stdio.h>
#include "pong.h"
#include "pong_platform.h"
#include "pong_draw.h"

// codes from https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#cursor-positioning
void    UP(short n)          { wprintf(L"\033[%dA"   , n   ); } // moves cursor up n units
void  DOWN(short n)          { wprintf(L"\033[%dB"   , n   ); } // moves cursor down n units
void RIGHT(short n)          { wprintf(L"\033[%dC"   , n   ); } // moves cursor n units to the right
void  LEFT(short n)          { wprintf(L"\033[%dD"   , n   ); } // moves cursor n units to the left
void   CLS(void)             { wprintf(L"\033[J"           ); } // clear screen
void CHIDE(void)             { wprintf(L"\033[?25l"        ); } // hide cursor
void CSHOW(void)             { wprintf(L"\033[?25h"        ); } // show cursor
void  CPOS(short x, short y) { wprintf(L"\033[%d;%dH", y, x); } // places cursor to (x, y) coordinates on the viewport (yes, I swapped x and y)
  // it seems that ANSI uses 1-1 as home while tput uses 0-0

void draw_blank(void){ // TODO: move special characters to #define-s

    // top border
    putwchar(CH_TOPLEFT);
    for(short i = 0; i < CONSOLE_W-2; ++i)
        putwchar(CH_HPIPE);
    putwchar(CH_TOPRIGHT);
    putwchar(L'\n');

    // header
    for(short i = 0; i < HEADER_HEIGHT; ++i){
        // faster but ugly: wprintf(L"│\033[%dG│\n", CONSOLE_W);
        putwchar(CH_VPIPE);
        RIGHT(CONSOLE_W-2);
        putwchar(CH_VPIPE);
    }

    // header/game area divider
    putwchar(CH_LSPLIT);
    for(short i = 0; i < CONSOLE_W-2; ++i)
        putwchar(CH_HPIPE);
    putwchar(CH_RSPLIT);

    // game area
    for(short i = 0; i < GAME_HEIGHT; ++i){
        putwchar(CH_VPIPE);
        RIGHT(CONSOLE_W-2);
        putwchar(CH_VPIPE);
    }

    // bottom border
    putwchar(CH_BOTLEFT);
    for(int j = 0; j < CONSOLE_W-2; ++j)
        putwchar(CH_HPIPE);
    putwchar(CH_BOTRIGHT);
}

void draw_score(Game game){
    CPOS(CONSOLE_W / 4, 2);
    wprintf(L"%d", game.players[0].score);

    CPOS(CONSOLE_W*3/4, 2);
    wprintf(L"%d", game.players[1].score);
}

void draw_game(Game game){ // doesn't clear game area, only draws on it!

    // left player's pad
    CPOS(3, HEADER_HEIGHT + 2 + game.players[0].pos);
    for(short i = 0; i < game.players[0].len; ++i){
        DOWN(1); // identical to putwchar(L'\n'); -- might be slower (3 extra control characters printed), but more readable code
        LEFT(1); // identical to putwchar(L'\b'); -- same as above
        putwchar(CH_VPIPE);
    }

    // right player's pad
    CPOS(CONSOLE_W, HEADER_HEIGHT + 2 + game.players[1].pos);
    for(short i = 0; i < game.players[1].len; ++i){
        DOWN(1);
        LEFT(1);
        putwchar(CH_VPIPE);
    }

    // draw ball
    CPOS(game.ball.x + 1, HEADER_HEIGHT + 2 + game.ball.y); // NO CLUE why I have to add 1 to x
    putwchar(CH_HPIPE);

    draw_score(game);
}
