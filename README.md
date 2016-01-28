# Turbo-Bomber

                       TTTTTT    MMMMMMM  MM   MM  MMMMMMMM  MMMMMMM   MMMMMMM               
            TTTTT    TTT     T      MM    MM   MM  MM    MM  MM   MM  MM     MM              
         TTTTTTTTTTTTTT             MM    MM   MM  MMMMMMM   MMMMMM   MM     MM              
      TTTTTTTTTTTTTTTTT             MM    MM   MM  MM   MM   MM    MM  MM    MM              
     TTTT           TTTT            MM     MMMMM   MM    MM  MMMMMMM    MMMMM                
    TTTTT  TT   TT  TTTTT                                                                    
    TTTTT  TT   TT  TTTTT                          MM                                        
    TTTTTTTTT   TTTTTTTTT         MMMMMMM     MMMM M   MMM   MMM  MMMMMMM   MMMMMMM  MMMMMM  
    TTTTTTTTT   TTTTTTTTT         MM   MMM  MM  MMMMM  MMMM MMMM  MM   MM   MM       M    MM 
     TTTTTT       TTTTTT          MMMMMMMM  MMMMMMMMM  MM M MMMM  MMMMMMMM  MMMMMM   MMMMM   
      TTTTTTTTTTTTTTTTT           MM     MM MMMMMMMMM  MM MMM MM  MM    MM  MM       M   MM  
        TTTTTTTTTTTTT             MMMMMMMM   MMMMMM    MM MMM MM  MMMMMMMM  MMMMMMM  M    MM 

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

```bash
$ make
$ bin/turbo-bomber
```

## Need help?

Contact the developers if you have questions: [dev.nipe.systems@gmail.com](mailto:dev.nipe.systems@gmail.com)

## Found a bug?

Write an issue: [https://github.com/NIPE-SYSTEMS/node-vgcanvas/issues](https://github.com/NIPE-SYSTEMS/node-vgcanvas/issues)

## License

lastest GPL-license (see `LICENSE`-file)

Copyright (C) 2015 NIPE-SYSTEMS ([http://nipe-systems.de](http://nipe-systems.de)), Jonas Krug, Tim Gevers
