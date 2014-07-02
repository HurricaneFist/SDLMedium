#ifndef MEDIUM_H
#define MEDIUM_H

#include "SDL/SDL.h"

namespace SDL {

    /* Event handling
     * ---
     * SDL::beginEvents();
     * 
     * while (SDL::events()) {
     *
     *     // code goes here 
     *
     * }
     *
     * SDL::endEvents();
     */
    
    struct Event {
        SDL_Event* ev = NULL;
        Event() {
            this->ev = new SDL_Event();
        }
    };
    
    Event* globalEventHandler;
    
    void beginEvents() {
        globalEventHandler = new Event();
    }
    
    bool events() {
        return SDL_PollEvent(globalEventHandler->ev);
    }
    
    void endEvents() {
        if (globalEventHandler) {
            globalEventHandler->ev = NULL;
            globalEventHandler     = NULL;
        }
    }
    
    /* --- */

    struct Rect {
        SDL_Rect* self = NULL;
        Rect(int x1, int y1, int x2, int y2) {
            this->self = new SDL_Rect(x1, y1, x2-x1, y2-y1);
        }
    }

    struct Image {
        SDL_Surface* self = NULL;
        Image(SDL_Surface* sf) {
            this->sf = sf;
        }
    };
    
    struct Window {
        SDL_Window* win = NULL;
        Window(SDL_Window* win) {
            this->win = win;
        }
    };

    void destroy(Rect* r) {
        r->self = NULL;
        r = NULL;
    }
    
    void destroy(Image* img) {
        SDL_FreeSurface(img->sf);
        img = NULL;
    }

    void destroy(Window* win) {
        SDL_DestroyWindow(win->win);
        win = NULL;
    }
    
    void draw(Image* src, Image* dest) {
        SDL_BlitSurface(src->self, NULL, dest->self, NULL);
    }
    
    SDL_Surface* getBMP(const char* filename) {
        return SDL_DisplayFormat(SDL_LoadBMP(filename));
    }
    
    void quit() {
        SDL_Quit();
    }

    void sleep(int time) {
        SDL_Delay(time);
    }
    
    void update(Image* img) {
        SDL_Flip(img->sf);
    }
    
    Window* window(const char* text, int width, int height) {
        return new Window(SDL_CreateWindow(text, SDL_WINDOWPOS_CENTERED, 
                          SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN));
    }
}

#endif