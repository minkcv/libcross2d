//
// Created by cpasjuste on 21/11/16.
//

#ifdef __SDL2_GL__

#include "c2d.h"

using namespace c2d;

#if defined(__SWITCH__) && defined(__NET_DEBUG__)
void initNxLink();
void deinitNxLink();
#endif

SDL2Renderer::SDL2Renderer(const Vector2f &size) : GLRenderer(size) {

#if defined(__SWITCH__) && defined(__NET_DEBUG__)
    initNxLink();
#endif

    printf("SDL2Renderer(GL)\n");

    if ((SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init sdl: %s\n", SDL_GetError());
        return;
    }

    Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    if (!getSize().x || !getSize().y) { // force fullscreen if window size == 0
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    printf("SDL_GL_SetAttribute\n");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);

    printf("SDL_CreateWindow\n");
    window = SDL_CreateWindow(
            "CROSS2D_SDL2_GL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            (int) getSize().x, (int) getSize().y, flags);
    if (!window) {
        printf("Couldn't SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s\n", SDL_GetError());
        return;
    }

    printf("SDL_GL_CreateContext\n");
    ctx = SDL_GL_CreateContext(window);
    if (!ctx) {
        printf("Couldn't SDL_GL_CreateContext: %s\n", SDL_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't SDL_GL_CreateContext: %s\n", SDL_GetError());
        return;
    }

    int major, minor;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &minor);
    printf("OpenGL: %i.%i\n", major, minor);

    initGL();

    available = true;

    // set shape size after GL initialization, for gl buffer generation
    setSize(size.x, size.y);

    printf("SDL2Renderer(GL)(%p): %ix%i\n", this, (int) getSize().x, (int) getSize().y);
}

void SDL2Renderer::flip(bool draw) {

    if (draw) {
        // call base class (draw childs)
        GLRenderer::flip();
    }

    // flip
    SDL_GL_SwapWindow(window);
}

void SDL2Renderer::delay(unsigned int ms) {

    SDL_Delay(ms);
}

SDL2Renderer::~SDL2Renderer() {

    printf("~SDL2Renderer(GL)\n");

    if (ctx) {
        SDL_GL_DeleteContext(ctx);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();

#if defined(__SWITCH__) && defined(__NET_DEBUG__)
    deinitNxLink();
#endif
}

#endif
