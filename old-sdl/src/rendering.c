#include <stdlib.h>
#include <SDL2/SDL.h>

#include "rendering.h"

/**
 * This function acts as a drawing function. It will be called periodically
 * every frame to render the whole frame. Before this function, the buffer of
 * the surface is cleared with a white paint.
 * 
 * The function is called from "core_main()" in "src/core.c".
 * 
 * @param renderer The renderer which is used in the rendering window. To render
 *                 graphics use this renderer.
 * @param frame_delta The time which passed between the last frame and the
 *                    current frame. To render animations properly use this
 *                    value.
 */
void rendering_main(SDL_Renderer *renderer, double frame_delta)
{
	// put some code in here that renders something
	return;
}
