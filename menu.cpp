#include "Menu.h"
#include <cstdio>

Menu::Menu(SDL_Renderer* renderer) : renderer(renderer) {
    SDL_Surface* bgSurface = IMG_Load("assets/menu_background.png");
    if (!bgSurface) {
        printf("Failed to load menu background: %s\n", IMG_GetError());
    } else {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_FreeSurface(bgSurface);
    }

    SDL_Surface* btnSurface = IMG_Load("assets/start_button.png");
    if (!btnSurface) {
        printf("Failed to load start button: %s\n", IMG_GetError());
    } else {
        startButtonTexture = SDL_CreateTextureFromSurface(renderer, btnSurface);
        SDL_FreeSurface(btnSurface);
    }

    startButton = {150, 250, 500, 200};
}

Menu::~Menu() {
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (startButtonTexture) SDL_DestroyTexture(startButtonTexture);
}

void Menu::render() {
    SDL_RenderClear(renderer);

    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }

    if (startButtonTexture) {
        SDL_RenderCopy(renderer, startButtonTexture, NULL, &startButton);
    }

    SDL_RenderPresent(renderer);
}

bool Menu::handleEvent(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x = e.button.x, y = e.button.y;
        if (x >= startButton.x && x <= startButton.x + startButton.w &&
            y >= startButton.y && y <= startButton.y + startButton.h) {
            return true;
        }
    }
    return false;
}

