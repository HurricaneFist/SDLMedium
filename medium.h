#ifndef MEDIUM_H
#define MEDIUM_H

#include "SDL/SDL.h"
#include <cstdlib>
#include <string>
#include <vector>

// Key constants //
enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SPACE,
    ENTER
};

namespace SDL {

    // Vector of all SDL_Surfaces on the screen.
    std::vector<SDL_Surface*> surfaces;

    /* Event handling
     * ---
     * SDL::beginEvents();
     *
     * while (true) 
     *     if (SDL::event()) {
     *         if (SDL::exit())
     *              SDL::quit();
     *
     *         switch (SDL::key()) {
     *             case UP:   // do something
     *             case DOWN: // do something else
     *             ...
     *             default:   // do things
     *         }
     *     }
     *
     * SDL::endEvents();
     */
    
    // Listens for events.
    struct EventListener {
        SDL_Event* ev = NULL;
        EventListener() {
            this->ev = new SDL_Event();
        }
    };
    
    EventListener* listener;
    
    // Initializes the event listener.
    void beginEvents() {
        listener = new EventListener();
    }
    
    // Returns true if an event occurs. Should always be used inside a loop.
    bool event() {
        return SDL_PollEvent(listener->ev);
    }
    
    // Stops the event listener. 
    void endEvents() {
        if (listener) {
            delete listener->ev;
            delete listener;
        }
    }
    
    // Returns true if a key is being pressed.
    bool keyDown() {
        if (listener->ev->type == SDL_KEYDOWN)
            return true;
        return false;
    }
    
    // Returns true if a key is not being pressed.
    bool keyUp() {
        if (listener->ev->type == SDL_KEYUP)
            return true;
        return false;
    }
    
    // Returns true if the user is exiting the program.
    bool exiting() {
        if (listener->ev->type == SDL_QUIT)
            return true;
        return false;
    }

    // Returns the ID of the current key.
    int key() {
        switch (listener->ev->key.keysym.sym) {
            case SDLK_UP:     return UP;    break;
            case SDLK_DOWN:   return DOWN;  break;
            case SDLK_LEFT:   return LEFT;  break;
            case SDLK_RIGHT:  return RIGHT; break;
            case SDLK_SPACE:  return SPACE; break;
            case SDLK_RETURN: return ENTER; break;
        }
    }
    
    /*
        Creates a rectangle.
        @param x1: The top left x position (in pixels)
        @param y1: The top left y position (in pixels)
        @param x2: The bottom right x position (in pixels)
        @param y2: The bottom right y position (in pixels)
    */
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

    /*
        Creates an image (AKA anything that can possibly be shown on the screen). 
    */
    struct Image {
        SDL_Surface* self = NULL;
        Image(SDL_Surface* self) {
            this->self = self;
            surfaces.push_back(this->self);
        }
    };
    
    /*
        Creates a window.
        @param width:  The width of the window in pixels
        @param height: The height of the window in pixels
        @param bpp:    The color depth of the window in bits per pixel (default 32)
    */
    struct Window {
        SDL_Surface* self = NULL;

        Window(int width, int height, int bpp = 32) {
            this->self = SDL_SetVideoMode(width, height, bpp, SDL_SWSURFACE);
            surfaces.push_back(this->self);
        }
    };
    
    /*
        Deallocates memory for a rectangle.
        @param r: The rectangle
    */
    void destroy(Rect* r) {
        r->self = NULL;
        delete r;
    }

    /*
        Deallocates memory for an image.
        @param img: The image
    */
    void destroy(Image* img) {
        SDL_FreeSurface(img->self);
        delete img;
    }

    // Destroys all surfaces (images and windows).
    void destroyAll() {
        while (!surfaces.empty()) {
            delete surfaces.back();
            surfaces.back() = NULL;
            surfaces.pop_back();
        }
    }

    /*
        Draws an image on top of another image.
        @param src:  The image to be drawn
        @param dest: The image drawn upon
    */
    void draw(Image* src, Image* dest) {
        SDL_BlitSurface(src->self, NULL, dest->self, NULL);
    }
    
    /*
        Draws an image onto a window.
        @param src:  The image to be drawn
        @param dest: The window
    */
    void draw(Image* src, Window* dest) {
        SDL_BlitSurface(src->self, NULL, dest->self, NULL);
    }
    
    /*
        Draws an image onto a window at the specified coordinates.
        @param src:  The image to be drawn
        @param dest: The window
        @param x:    The x position (in pixels)
        @param y:    The y position (in pixels)
    */
    void draw(Image* src, Window* dest, int x, int y) {
        SDL_Rect offset;
        
        offset.x = x;
        offset.y = y;
        
        SDL_BlitSurface(src->self, NULL, dest->self, &offset);
    }
    
    /*
        Loads an image from a file
        @param filename: The name of the file
    */
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

    /*
        Initializes the SDL library.
    */
    void init() {
        SDL_Init(SDL_INIT_EVERYTHING);
    }

    /*
        Quits the program.
    */
    void quit() {
        if (!surfaces.empty())
            destroyAll();
        SDL_Quit();
    }

    /*
        Sets the caption for the window.
    */
    void setCaption(const char* caption) {
        SDL_WM_SetCaption(caption, NULL);
    }

    /*
        Sleeps for a specified amount of time.
        @param time: The amount of time (in milliseconds)
    */
    void sleep(int time) {
        SDL_Delay(time);
    }
    
    /*
        Updates the window.
        @param win: The window to be updated.
    */
    void update(Window* win) {
        SDL_Flip(win->self);
    }
}

#endif