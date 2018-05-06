     ____   ___  ____   ___ _____ ___ ____ ____    _     ___ _____ _____
    |  _ \ / _ \| __ ) / _ \_   _|_ _/ ___/ ___|  | |   |_ _|_   _| ____|
    | |_) | | | |  _ \| | | || |  | | |   \___ \  | |    | |  | | |  _|
    |  _ <| |_| | |_) | |_| || |  | | |___ ___) | | |___ | |  | | | |___
    |_| \_\\___/|____/ \___/ |_| |___\____|____/  |_____|___| |_| |_____|

A multiplayer arcade game built upon _FIRST_(r) POWER UP.

Johan Bockbr.... <j@pelirrojo.ninja> Mentor. 5690.

The Story Thus Far
==================

In the beginning, the universe was created. This is considered by some to
be a mistake and has made several people upset, and as a result, hasn't
been attempted again..... Oops, wrong monologue. Let's see here....

The idea started after watching the teaser. I coded out a web-based test
version and it came out ... decent. It was sort of a single-player puzzle 
where you'd deliver a cogs to a drop site, and other misc. power ups.

I scrapped that idea for a bit, and then build season came along. I was
talking to our pit head about having a game running on our pit. And I got
really sick during the build season, and while I couldn't talk, I could
still type, and I could still type out code.

And so it worked. I ended up having to do some more work after the fact,
including fixing a memory leak at our Week 2 Competition that I didn't end
up accounting for (even after beta testing it at a Week 0).

How to Compile
==============

On Linux, install g++, make, and development headers for SDL2, SDL2-ttf,
and SDL2-image. Then, a simple `make` should do the trick.

On Windows, follow the Linux instructions and use MinGW.

On Macs, the game can be compiled by learning how to do that.
(:steamsalty: ×7)

How to Play
===========

Take cubes, drive into a cube to pick it up, and drop them off at the
scale platform by simply driving over the platform. Hitboxes are picky, so
be warned.

Players 1-4 keyboard binds can be set in the settings.

Joystick binds are hard coded, That might be a thing that needs to change.

--------------------------------------------------------------------------

(The rest of this file isn't exactly README material, but since I don't
want to externalize it to another file, here it is)

Under the Bonnet
================

I coded the game in SDL. Primarily since we decided we were going to be
running the game on a Raspberry Pi in the pit, and I really didn't want
the overhead of running a web browser and interpreting the Javascript. The
Raspberry Pi was running Arch Linux, set to automatically login, and in
the `~/.bashrc`, there was `cd roboticslite && startx ./roboticslite`.
(and then well after our second regional, I read SDL's documentation on
running the code on a Raspberry Pi and was promply fustrated with myself.)

Input logic is converted from your raw input (keyboard, joystick, MIDI
accordion, etc.) into the format that follows:

Input Mask:

 + 0x01: Move North
 + 0x02: Move West
 + 0x04: Move South
 + 0x08: Move East
 + 0x10: Enable Strafe-lock / the fancy Mecanum Wheels

Events are pretty self explanatory.

I'm looking at the code right now, and I'm glad I decided that if I take
this any further, I would like to start it from scratch again.

But, if for whatever reason, you're going through the code, all of the
references to `rr` were from the old name, as I was coding it. Somebody
suggested "Robotics Lite" and that's the name I went with.

Into the Future
===============

I do want to remake this code. Primarily make it easier to follow and
easier to read, because I'm looking at it right now and I'm wondering if
all the cold and flu meds I took while coding this had altered my mental
state.

I would also like to make it easier to port.

The Things I Wanted to Do but Didn't
====================================

Demo / Replay Files
-------------------

I actually started this. Having come from playing primarily Source based
games (Team Fortress, Counterstrike), where sometimes it makes sense to
record the actions and then replay them through the game engine. Basically
yielding a smaller video file at the expense of not being able to replay
through a standard video player.

It didn't end up working out at the end and I just scrapped it.

Single Player Puzzle Mode
-------------------------

This actually probably will be more time consuming designing the levels
than it would be to actually implement a level datatype/format.

High Score Arcade Board
-----------------------

Tried it. I thought it worked. It didn't so I disabled it.

Network Multiplayer
-------------------

A complete and total mess that I didn't want to get into. At all.

Actually, let me elaborate on this a little bit more.

Local Multiplayer is easy. You have multiple players, and just bind to
different input sources to the different players.

LAN Multiplayer is slightly moar difficult, but you can still have all of
the players just shouting their movements at each other and because it's a
LAN, it's fine because it'll go from host A to host B in less than 5
milliseconds, which probably wouldn't have any noticable delay issues.

Internet Multiplayer servers basically run into the issue where you have
a bunch of different computers, of various processor types and powers
connecting over a wide range of network connections playing what boils
down to a "fair" game. And there is a lot of maths involved.

AI Robots
---------

For either a single-player Pacman-like experience, or to play against in
the multiplayer games.

This is also one of those long thought projects, like seperating
development of it from the rest of the game sort of things.

Since I was looking to make this game asthetically similar to Pacman, I
decided to look into how the Pacman ghosts make their pathfinding
decisions. TL;DR of which: multiple modes, make decisions only at the
intersections in the mase. (for more details, I referenced
<http://gameinternals.com/post/2072558330/understanding-pac-man-ghost-behavior>)

Robotics Lite here is an open tile based map, so a labyrinth-based
path-finding system would not work.

I never actually got a start on this, but I was thinking something akin to
SethBling's [MarI/O](https://www.youtube.com/watch?v=qv6UVOQ0F44), without
the neural network evolution. (At least, in a sense: visualize a simpler
version of the board relative to the AI-bot itself, and then make
decisions based on those inputs ... it's starting to sound like a game
within a game right here.... wowow.....)

Final Notes
===========

We have since removed "What's New Pussycat" from this jukebox.

