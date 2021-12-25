# terminal-pong
A simple game of pong for Windows terminal

***

Using a terminal with UTF-8 support (such as Windows Terminal) is highly recommended - on standard cmd, your ball will be a rectangle (change in [pong_draw.h](/pong_draw.h)).

[Demo video](https://i.imgur.com/7au2jmY.mp4)

***

## Working features

* Game automatically adjusts on launch to fill screen buffer
* Left pad is controlled by W/S, right by up/down arrow keys
* New characters are only printed when strictly necessary, no reprint of entire screen on every frame
* Game speeds up progressively
* Score of each player is displayed in a header row

***

## Partially working features

* Game stops and announces winner after 9 points - this is hardcoded in [pong.h](/pong.h)
* Support for multiple difficulties (EASY, MEDIUM, HARD) - this is also hardcoded, change in [main.c](/pong.c#L50). Difficulty changes:
  * Game speed
  * Interval at which game speeds up
  * Speed multiplier that is applied each time it speeds up
  * Length of player pads
* Slope of the ball is also hardcoded, but change a number in a [few](/pong.c#L40) [places](/pong_logic.c#L98) and the game will behave accordingly

***

## Missing features/bugs

* Slope of ball should change depending on where it hits the pads (Atari Breakout-style)
* Option to choose between AI and player control for each pads
* Sometimes the ball gets drawn on top of pads for some reason? This isn't a huge problem
* Some error handling would be nice, but isn't priority
* Simple menu system or input parameters to change variables mentioned in **partially working features**
* Cross platform support would be amazing (but linux input sucks and external libraries are a hassle + bloated)
