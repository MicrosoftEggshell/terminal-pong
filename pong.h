#ifndef _PONG_H
#define _PONG_H

#define SCORE_LIMIT   9                               // TODO: not hardcode this
#define HEADER_HEIGHT 1                               // score is displayed in header, should be at least 1
#define GAME_HEIGHT   (CONSOLE_H - HEADER_HEIGHT - 3) // don't change - 3 is the borders' height
  // TODO: GAME_WIDTH

typedef enum {false, true} bool; // I literally do not care whether this is good practice or not

typedef struct {
    short base_speed;               // time between frames, in ms
    short speed_increase_interval; // speed increases (<=> sleep time decreases) after every nth bounce
    float speed_increase_amount;  // the ratio at which speed increases
    short pad_length;            // pads should be this long (very useful comment, you are welcome)
} diff_t;

typedef enum {EASY, MEDIUM, HARD} diff_name;

const extern diff_t difficulties[3];

typedef struct {
    struct { // player
        bool  is_player; // both left and right pads could be controlled by AI or player
        short pos;      // distance between pad and top of game area
        short len;     // pad length
        short score;  // player score
    } players[2];

    struct { // ball
        short x;
        short y;
        /** slope
         * stores the ball's direction from 0 to 90°, represented as a slope.
         * non-negative values represent how many vertical units
         * the ball moves under the span of 1 horizontal unit, while
         * negative values represent "flat" trajectory ( 1 / |slope| ).
        **/
        short slope;
        bool right; // true while ball is moving right
        bool up;   // true while ball is moving up
    } ball;
    diff_name diff;         // difficulty
    unsigned short bounces; // bounces since last serve
    short speed;            // delay between frames in ms
} Game;

#endif
