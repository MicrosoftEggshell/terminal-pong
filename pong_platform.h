#ifndef _PONG_PLATFORM_H
#define _PONG_PLATFORM_H

extern short CONSOLE_H;   // viewport height
extern short CONSOLE_W;   // viewport width
void platform_init(void);
short   up_pressed(void);
short down_pressed(void);
short    w_pressed(void);
short    s_pressed(void);

// for sleep():
#ifdef _WIN32

#include <windows.h>
#define sleep(x) Sleep(x*1000) // windows sleeps milliseconds by default

#elif defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))

#include <unistd.h>

#endif // sleep

#endif // !_PONG_PLATFORM_H
