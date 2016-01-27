# Turbo-Bomber

                       TTTTTT
            TTTTT    TTT     T
         TTTTTTTTTTTTTT
      TTTTTTTTTTTTTTTTT
     TTTT           TTTT
    TTTTT  TT   TT  TTTTT
    TTTTT  TT   TT  TTTTT
    TTTTTTTTT   TTTTTTTTT
    TTTTTTTTT   TTTTTTTTT
     TTTTTT       TTTTTT
      TTTTTTTTTTTTTTTTT
        TTTTTTTTTTTTT

> A game made with ncurses in C, inspired by Bomberman.

The main goal of this project is to implement a game like *Bomberman*. The player plays in a quadratic (2d) world and tries to clear its way with clever placed bombs. Some walls contain items that increase several properties of the player and its utilities.

## Main features

* Rendering
    * Sprite-System: Read ASCII sprites from files and display them in the command line.
    * Animations
    * Colors
    * Cutscenes: Start-, Win- and Game-Over-Screens with animations.
* Gameplay
    * Explosion of bombs
    * Item-System: Pick up *Power Ups*
    * Keyboard input handling
    * Data structures: Save all current states and make them drawable.
* Artificial Intelligence (AI): Playing against an AI which tries to attack the player.

## Installation/Compiling

To compile the project the library `ncurses` is needed. It must also be available in `pkg-config`.

If `ncurses` is installed, change (`cd`) to the root directory of the project and run the following:

    $ make
    $ bin/turbo-bomber
