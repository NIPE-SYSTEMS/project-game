# Copyright (C) 2015 NIPE-SYSTEMS
# Copyright (C) 2015 Jonas Krug
# Copyright (C) 2015 Tim Gevers
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC = gcc

CFLAGS += -Wall
CFLAGS += -Wextra

CFLAGS += `pkg-config --cflags ncurses`

# Enable debug output to "debug.log" and "error.log"
# CFLAGS += -DDEBUG

# Enable debug informations instead of normal sidebar in the game (a.k.a. debug stats)
# CFLAGS += -DDEBUG_INFO

LIBS += `pkg-config --libs ncurses`

# game sources
SRC = $(notdir $(wildcard src/*.c))
OBJS = $(addprefix bin/obj/, $(SRC:%.c=%.o))

.PHONY: all clean specification debug

all: bin/turbo-bomber

# create object directory for game objects
bin/obj:
	mkdir -p bin/obj

# create object directory for shared modules objects
bin/modules:
	mkdir -p bin/modules

# compile all objects into the binary
bin/turbo-bomber: bin/obj $(OBJS)
	$(CC) $(CFLAGS) $(CFLAGS) $(OBJS) -o bin/turbo-bomber $(LIBS)

# compile a game object
bin/obj/%.o: src/%.c
	$(CC) $(CFLAGS) $(CFLAGS) -c $< -o $@ $(LIBS)

# clean complete objects directory
clean:
	rm -Rf bin

specification:
	make -C 'specification'

debug:
	valgrind --log-file=valgrind.log --leak-check=full --show-leak-kinds=all bin/turbo-bomber
