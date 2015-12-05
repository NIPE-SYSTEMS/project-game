# Copyright (C) 2015 NIPE-SYSTEMS
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

CFLAGS_GAME += `sdl2-config --cflags`
CFLAGS_GAME += `pkg-config --cflags openal`

LIBS_GAME += `sdl2-config --libs`
LIBS_GAME += `pkg-config --libs openal`
LIBS_GAME += -ldl

PROGRAM_NAME = project-game

# game sources
SRC_GAME = $(notdir $(wildcard src/*.c))
OBJS_GAME = $(addprefix bin/obj/, $(SRC_GAME:%.c=%.o))

# module sources
SRC_MODULES = $(notdir $(wildcard src/modules/*.c))
OBJS_MODULES = $(addprefix bin/modules/, $(SRC_MODULES:%.c=%.so))

.PHONY: all clean

all: bin/project-game bin/modules $(OBJS_MODULES)

# create object directory for game objects
bin/obj:
	mkdir -p bin/obj

# create object directory for shared modules objects
bin/modules:
	mkdir -p bin/modules

# compile all objects into the binary
bin/project-game: bin/obj $(OBJS_GAME)
	$(CC) $(CFLAGS) $(CFLAGS_GAME) $(OBJS_GAME) -o bin/project-game $(LIBS_GAME)

# compile a game object
bin/obj/%.o: src/%.c
	$(CC) $(CFLAGS) $(CFLAGS_GAME) -c $< -o $@ $(LIBS_GAME)

# compile a module object 
bin/modules/%.so: src/modules/%.c
	$(CC) $(CFLAGS) $(CFLAGS_GAME) $< -shared -o $@

# clean complete objects directory
clean:
	rm -Rf bin