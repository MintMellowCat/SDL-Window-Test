#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdlib.h>

const Uint32 fps = 60; //Set speed in fps (frames per second)
const Uint32 minframetime = 1000 / fps;

int main() {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Game Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_SHOWN); //Display window
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0); //Create renderer

    SDL_SetWindowFullscreen(window, 0);

    bool running = true;
    Uint32 frametime;
    SDL_Event event;

    while (running) //This is called once per frame
    {
        SDL_SetWindowResizable(window, SDL_TRUE); //Make window resizable

        SDL_RenderClear(renderer); //Clear screen

        static float x = 0; //Characters x position
        static float y = 0; //Characters y position
        static float speed = 10; //Characters speed
        static bool fullScreen = true; //Is fullscreen
        static int windowWidth = 1000; //Get window width
        static int windowHeight = 700; //Get window height
        static int screenWidth = 1920;
        static int screenHeight = 1080;
        static const int deadZone = 8000;

        const Uint8* state = SDL_GetKeyboardState(NULL); //Store key state in variable

        if (state[SDL_SCANCODE_LEFT]) { //Left is pressed
            x -= speed;
        }
        if (state[SDL_SCANCODE_RIGHT]) { //Right is pressed
            x += speed;
        }
        if (state[SDL_SCANCODE_UP]) { //Up is pressed
            y -= speed;
        }
        if (state[SDL_SCANCODE_DOWN]) { //Down is pressed
            y += speed;
        }

        if (fullScreen) {
            if (x < 0) { //Stop character from moving off the edge of the screen
                x = 0;
            }
            if (y < 0) {
                y = 0;
            }
            if (x > (screenWidth - 100)) {
                x = screenWidth - 100;
            }
            if (y > (screenHeight - 100)) {
                y = 1080 - 100;
            }
        }
        else {
            if (x < 0) { //Stop character from moving off the edge of the screen
                x = 0;
            }
            if (y < 0) {
                y = 0;
            }
            if (x > (windowWidth - 100)) {
                x = windowWidth - 100;
            }
            if (y > (windowHeight - 100)) {
                y = windowHeight - 100;
            }
        }

        SDL_Surface* image = SDL_LoadBMP("Character.bmp"); //Load character image
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image); //Convert to texture
        
        SDL_Surface* backgroundImage = SDL_LoadBMP("Background.bmp"); //Load background image
        SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundImage); //Convert to texture

        SDL_Rect rect; //Coordinates and sizes for the sprites
        rect.x = 0;
        rect.y = 0;
        rect.w = 100;
        rect.h = 100;

        SDL_Rect rect2;
        rect2.x = x;
        rect2.y = y;
        rect2.w = 100;
        rect2.h = 100;

        SDL_Rect rect3;
        rect3.x = 0;
        rect3.y = 0;
        rect3.w = 5760;
        rect3.h = 1080;

        SDL_Rect rect4;
        rect4.x = 0;
        rect4.y = 0;
        rect4.w = 7488;
        rect4.h = 1404;

        SDL_RenderCopy(renderer, backgroundTexture, &rect3, &rect4); //Display background
        SDL_RenderCopy(renderer, texture, &rect, &rect2); //Display character

        SDL_RenderPresent(renderer); //Render everything
        
        frametime = SDL_GetTicks(); //Get frame time

        if (SDL_PollEvent(&event))
        {
            switch (event.type)
            {

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_F11) { //Detect if F11 is pressed
                    if (fullScreen) {
                        SDL_SetWindowFullscreen(window, 0);
                        SDL_RestoreWindow(window);
                        SDL_SetWindowSize(window, windowWidth, windowHeight);
                        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED); //Toggle fullscreen off

                        fullScreen = false;
                    }
                    else {
                        SDL_SetWindowSize(window, 1920, 1080);
                        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN); //Toggle fullscreen on

                        fullScreen = true;

                        SDL_GetWindowSize(window, &screenWidth, &screenHeight);
                    }
                }
            }

            if (event.type == SDL_WINDOWEVENT) {
                switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
                }
            }

            if (SDL_QUIT == event.type) //Detect if user presses the x button
            {
                SDL_DestroyTexture(texture);
                SDL_FreeSurface(image);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);

                break; //Quit game
            }
        }

        if (SDL_GetTicks() - frametime < minframetime) //Check if it is running at the right speed
            SDL_Delay(minframetime - (SDL_GetTicks() - frametime));

    }

	return 0;
}