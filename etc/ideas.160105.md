# Properties of a tile

* type (wall, destructable wall, floor)
* player (pointer to a player-struct, NULL means no player is on the tile)
* explosion (ponter to an animation, NULL means no animation: the tile is displayed normally)
* bomb (pointer to a bomb-struct, NULL means no bomb is on the tile)
* item (see Power ups) (only one item per tile)
* item despawn timing

# Power ups

* atomic bomb: increase detonation radius
* laser: shoot (beam/explosion) in one direction
* glove: move bombs
* shield: short resistance
* heart: extra health point
* extra bomb: place multiple bombs
* remote detonation: explosion where other player stand (after short delay to be able to interact)
* sneakers: faster movement

# Field

* typical bomberman field with destructable and non-destructable tiles
* destructable tiles may drop an item (probability)
* item drops are calculated at initialization

# Properties of a player

* health points
* movement cooldown
* position
* amount of placeable bombs
* amount of currently placed bombs
* current item (only one item per player)
* item usage timing (duration, used time)

# Criteria

* read from files
* random item drops
* read keys from keyboard
* move player
* animations
* place bombs
* destructable map
* game loop
