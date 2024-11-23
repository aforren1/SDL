/*
 * This example code just checks for timestamps.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static Uint64 time = 0;
static Uint32 type = 0;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Event Timestamps", "1.0", "com.example.event-timestamps");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/event/timestamps", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    Uint64 ts = 0;
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;

        case SDL_EVENT_KEY_DOWN:
            [[fallthrough]];
        case SDL_EVENT_KEY_UP:
            ts = event->key.timestamp;
            [[fallthrough]];
        case SDL_EVENT_MOUSE_MOTION:
            ts = event->motion.timestamp;
            [[fallthrough]];
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            [[fallthrough]];
        case SDL_EVENT_MOUSE_BUTTON_UP:
            ts = event->button.timestamp;
            [[fallthrough]];
        case SDL_EVENT_MOUSE_WHEEL:
            ts = event->wheel.timestamp;
            [[fallthrough]];
        default:
            //SDL_Log("time %llu, event %i\n", ts, event->type);
            time = ts;
            type = event->type;
            break;
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    char *caption = NULL;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */
    SDL_SetRenderDrawColor(renderer, 51, 102, 255, SDL_ALPHA_OPAQUE);  /* light blue, full alpha */
    SDL_asprintf(&caption, "time: %llu, event: %i",  time, type);
    if (caption) {
        SDL_RenderDebugText(renderer, 100, 100, caption);
        SDL_free(caption);
    }
    SDL_RenderPresent(renderer);  /* put it all on the screen! */
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}
