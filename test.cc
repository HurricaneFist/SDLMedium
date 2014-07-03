#include "medium.h"

const int WIDTH  = 640,
          HEIGHT = 480, 
          BPP    = 32,
          FPS    = 60;

SDL::Image  *hello  = NULL;
SDL::Window *screen = NULL;

int main(int argc, char** args) {
    SDL::init();
    screen = new SDL::Window(WIDTH, HEIGHT, BPP);
    SDL::setCaption("Hello");

    SDL::beginEvents();

    double vx = 0, vy = 0,
           x  = 0, y  = 0;

    hello = SDL::load("window.bmp");

    bool quit = false;
    while (!quit) {
        if (SDL::event()) {
        
            if (SDL::exiting())
                quit = true;

            if (SDL::keyDown()) {

                switch (SDL::key()) {
                    case UP:    vy += -1; break;
                    case DOWN:  vy +=  1; break;
                    case LEFT:  vx += -1; break;
                    case RIGHT: vx +=  1; break;
                }

            } else if (SDL::keyUp()) {
                if (SDL::key() == RIGHT || SDL::key() == LEFT)
                    vx = 0;
                else if (SDL::key() == UP || SDL::key() == DOWN)
                    vy = 0;
            }
        }

        x += vx;
        y += vy;

        SDL::draw(hello, screen, x, y);
        SDL::update(screen);
        SDL::sleep(1000/FPS);
    }

    SDL::endEvents();
    SDL::destroyAll();
    SDL::quit();
}