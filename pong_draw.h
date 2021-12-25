#ifndef _PONG_DRAW_H
#define _PONG_DRAW_H

// https://en.wikipedia.org/wiki/Box-drawing_character
// maybe could use: https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#designate-character-set
// normalize unicode characters in source code #uniCoding
#define CH_HPIPE    L'─'
#define CH_VPIPE    L'│'
#define CH_TOPLEFT  L'┌'
#define CH_TOPRIGHT L'┐'
#define CH_BOTLEFT  L'└'
#define CH_BOTRIGHT L'┘'
#define CH_TSPLIT   L'┬'
#define CH_BSPLIT   L'┴'
#define CH_LSPLIT   L'├'
#define CH_RSPLIT   L'┤'
#define CH_CROSS    L'┼'
#define CH_BALL     L'●'
/** alternatives:
 * ● - '\0x25CF' - BLACK CIRCLE
 * ● - '\0x26AB' - MEDIUM BLACK CIRCLE (current)
 * • - '\0x2022' - BULLET
 * ■ - '\0x25A0' - BLACK SQUARE (for more 'pixelated' feeling)
 * ◯ - '\0x25EF' - LARGE CIRCLE (not actually 2 wide)
**/

void    UP(short n)         ; // moves cursor up n units
void  DOWN(short n)         ; // moves cursor down n units
void RIGHT(short n)         ; // moves cursor n units to the right
void  LEFT(short n)         ; // moves cursor n units to the left
void   CLS(void)            ; // clear screen
void CHIDE(void)            ; // hide cursor
void CSHOW(void)            ; // show cursor
void  CPOS(short x, short y); // places cursor to (x, y) coordinates on the viewport
  // it seems that ANSI uses 1-1 as home while tput uses 0-0


void draw_blank(void);
void draw_score(Game game);
void draw_game(Game game);

#endif
