#ifndef MEDIUM_H
#define MEDIUM_H

#include "SDL/SDL.h"
#include <string>

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
            this->self = new SDL_Rect();
            self->x = x1;
            self->y = y1;
            self->w = x2-x1;
            self->h = y2-y1;
        }
    };

    struct Image {
        SDL_Surface* self = NULL;
        Image(SDL_Surface* self) {
            this->self = self;
        }
    };
    
    struct VideoMode {
        SDL_Surface* self = NULL;
        VideoMode(int width, int height, int bpp) {
            this->self = SDL_SetVideoMode(width, height, bpp, SDL_SWSURFACE);
        }
    };

    void destroy(Rect* r) {
        r->self = NULL;
        r = NULL;
    }
    
    void destroy(Image* img) {
        SDL_FreeSurface(img->self);
        img = NULL;
    }
    
    void draw(Image* src, Image* dest) {
        SDL_BlitSurface(src->self, NULL, dest->self, NULL);
    }
    
    void draw(Image* src, VideoMode* dest) {
        SDL_BlitSurface(src->self, NULL, dest->self, NULL);
    }
    
    Image* load(const char* filename) {
        enum { BMP, JPG, PNG };
        int type;
        std::string str(filename);
        
        if      (str.find(".bmp") != std::string::npos)
            type = BMP;
        else if (str.find(".jpg") != std::string::npos)
            type = JPG;
        else if (str.find(".png") != std::string::npos)
            type = PNG;
        
        SDL_Surface *loaded, *optimized;
        
        switch (type) {
            case BMP: loaded = SDL_LoadBMP(filename); break;
            default:  loaded = NULL;
        }
        
        optimized = SDL_DisplayFormat(loaded);
        SDL_FreeSurface(loaded);
        
        return new Image(optimized);
    }
    
    void init() {
        SDL_Init(SDL_INIT_EVERYTHING);
    }
    
    void quit() {
        SDL_Quit();
    }

    void sleep(int time) {
        SDL_Delay(time);
    }
    
    void update(Image* img) {
        SDL_Flip(img->self);
    }
    
    void update(VideoMode* vm) {
        SDL_Flip(vm->self);
    }
}

#endif