# Sprites reference

Sprites are little ASCII-code-images which can be stored in binary files.

## Creation or modification by the user

The user is able to modify existing sprites files. In addition new files may also be created. It is recommended to use the *Sprite Generator* which is a simple HTML-page, stored as `sprite-generator.html`.

With the *Sprite Generator* some basic features are usable:

1. Generation of new sprites.
2. Importing of existing sprite files.
3. Exporting to a sprite file.
4. Modifying sprites directly.
5. Some fancy visualizations for displaying the sprites.

## Sprite file reference

A sprite file can content multiple sprites. All sprites in the file have the same size which is also saved in it. A sprite file is a binary file.

    2 bytes: Amount of sprites in the file (integer)
    2 bytes: Width of the sprites (integer)
    2 bytes: Height of the sprites (integer)
    n bytes: Sprite data

The `Sprite data` part is exactly `amount * width * height` bytes in size. The sprites are saved line by line as ASCII-characters without any encoding or spacing.

## Example code for the reading API

```c
sprite_t *sprite = sprite_load("test.sprite");
if(sprite != NULL)
{
    sprite_free(sprite);
}
```

## Further development

In the future an improved API is planned with higher-level-features. E.g. direct retrieving of animations and single characters.
