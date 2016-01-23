#!/bin/sh

valgrind --log-file=valgrind.log --leak-check=full --show-leak-kinds=all bin/project-game
