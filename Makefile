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

CFLAGS += `pkg-config --cflags ncurses`
CFLAGS += -DDEBUG

LIBS += `pkg-config --libs ncurses`

PROGRAM_NAME = project-game

# game sources
SRC = $(notdir $(wildcard src/*.c))
OBJS = $(addprefix bin/obj/, $(SRC:%.c=%.o))

.PHONY: all clean

all: bin/project-game

# create object directory for game objects
bin/obj:
	mkdir -p bin/obj

# create object directory for shared modules objects
bin/modules:
	mkdir -p bin/modules

# compile all objects into the binary
bin/project-game: bin/obj $(OBJS)
	$(CC) $(CFLAGS) $(CFLAGS) $(OBJS) -o bin/project-game $(LIBS)

# compile a game object
bin/obj/%.o: src/%.c
	$(CC) $(CFLAGS) $(CFLAGS) -c $< -o $@ $(LIBS)

# clean complete objects directory
clean:
	rm -Rf bin