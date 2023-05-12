#include "nextstep.h"
#include <SDL2/SDL.h>

extern SDL_Renderer *renderer_i;

// For [NXView setDrawOrigin]
extern SDL_Point draw_origin;

void NXEraseRect(const NXRect *aRect)
{
    SDL_Color old;
    SDL_GetRenderDrawColor (renderer_i, &old.r, &old.g, &old.b, &old.a);
    SDL_SetRenderDrawColor (renderer_i, 0xff, 0xff, 0xff, 0xff);

    SDL_Rect r;
    r.x = aRect->origin.x - draw_origin.x;
    r.y = aRect->origin.y - draw_origin.y;
    r.w = aRect->size.width;
    r.h = aRect->size.height;

    SDL_RenderFillRect(renderer_i, &r);
    SDL_SetRenderDrawColor (renderer_i, old.r, old.g, old.b, old.a);
}

// This is not really an equivalent, but I think it produces the desired
// effect in doombsp. -TF
void NXPing(void)
{
    SDL_RenderPresent(renderer_i);
    SDL_Delay(5);
}
