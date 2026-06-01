#include "game/entities/player/player.h"

#include "game/core/event_handler/input_mapping/input_mapping.h"

// FUNCTIONS

void player_init(Player *p, int world_x, int y)
{
    p->world_x = world_x;
    p->y = y;
    p->vy = 0;
    p->on_ground = false;
}

bool player_jump_pressed(const SDL_Event *e)
{
    // Keyboard parity (macOS dev) OR the mapped A button (RG35XXH, remappable).
    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_SPACE)
        return true;
    return input_mapping_is_press(e, MAPPING_LABEL_A);
}

void player_jump(Player *p, int jump_vy)
{
    if (p->on_ground)
    {
        p->vy = jump_vy;
        p->on_ground = false;
    }
}

void player_apply_gravity(Player *p, int gravity, int max_vy, int ground_top, int player_h)
{
    p->vy += gravity;
    if (p->vy > max_vy)
        p->vy = max_vy;
    p->y += p->vy;

    if (p->y + player_h >= ground_top)
    {
        p->y = ground_top - player_h;
        p->vy = 0;
        p->on_ground = true;
    }
    else
    {
        p->on_ground = false;
    }
}
