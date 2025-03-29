#include "Menu.h"
#include <cstdio>

Menu::Menu(SDL_Renderer* renderer) : renderer(renderer), state(MAIN_MENU) {
    backgroundTexture = IMG_LoadTexture(renderer, "assets/menu_background.png");
    singlePlayerButton = IMG_LoadTexture(renderer, "assets/single_player.png");
    twoPlayerButton = IMG_LoadTexture(renderer, "assets/two_player.png");
    playNowButton = IMG_LoadTexture(renderer, "assets/play_now.png");
    howToPlayButton = IMG_LoadTexture(renderer, "assets/how_to_play.png");
    howToPlayImage = IMG_LoadTexture(renderer, "assets/how_to_play_screen.png");
    backButton = IMG_LoadTexture(renderer, "assets/back_button.png");
    replayButton = IMG_LoadTexture(renderer, "assets/replay_button.png");
    menuButton = IMG_LoadTexture(renderer, "assets/menu_button.png");

    singlePlayerRect = { 200, 250, 400, 100 };
    twoPlayerRect = { 200, 350, 400, 100 };
    playNowRect = { 200, 250, 400, 100 };
    howToPlayRect = { 200, 350, 400, 100 };
    backButtonRect = { 200, 450, 400, 100 };
}

Menu::~Menu() {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(singlePlayerButton);
    SDL_DestroyTexture(twoPlayerButton);
    SDL_DestroyTexture(playNowButton);
    SDL_DestroyTexture(howToPlayButton);
    SDL_DestroyTexture(howToPlayImage);
}

void Menu::render() {
    SDL_RenderClear(renderer);

    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }

    switch (state) {
        case MAIN_MENU:
            SDL_RenderCopy(renderer, singlePlayerButton, NULL, &singlePlayerRect);
            SDL_RenderCopy(renderer, twoPlayerButton, NULL, &twoPlayerRect);
            break;

        case LOAD_SCREEN:
            backButtonRect = { 200, 450, 400, 100 };
            SDL_RenderCopy(renderer, playNowButton, NULL, &playNowRect);
            SDL_RenderCopy(renderer, howToPlayButton, NULL, &howToPlayRect);
            SDL_RenderCopy(renderer, backButton, NULL, &backButtonRect);
            break;

        case HOW_TO_PLAY:
            backButtonRect = { 250, 500, 300, 75 };
            SDL_RenderCopy(renderer, howToPlayImage, NULL, NULL);
            SDL_RenderCopy(renderer, backButton, NULL, &backButtonRect);
            break;
    }

    SDL_RenderPresent(renderer);
}

bool Menu::handleEvent(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x = e.button.x, y = e.button.y;

        if (state == MAIN_MENU) {
            if (x >= singlePlayerRect.x && x <= singlePlayerRect.x + singlePlayerRect.w &&
                y >= singlePlayerRect.y && y <= singlePlayerRect.y + singlePlayerRect.h) {
                    selectedGameMode = SINGLE_PLAYER;
                    state = LOAD_SCREEN;
                    return false;
            }
            if (x >= twoPlayerRect.x && x <= twoPlayerRect.x + twoPlayerRect.w &&
                y >= twoPlayerRect.y && y <= twoPlayerRect.y + twoPlayerRect.h) {
                    selectedGameMode = TWO_PLAYER;
                    state = LOAD_SCREEN;
                    return false;
            }
            if (x >= howToPlayRect.x && x <= howToPlayRect.x + howToPlayRect.w &&
                y >= howToPlayRect.y && y <= howToPlayRect.y + howToPlayRect.h) {
                state = HOW_TO_PLAY;
            }
        }
        else if (state == LOAD_SCREEN) {
            if (x >= playNowRect.x && x <= playNowRect.x + playNowRect.w &&
                y >= playNowRect.y && y <= playNowRect.y + playNowRect.h) {
                return true;
            }
            if (x >= howToPlayRect.x && x <= howToPlayRect.x + howToPlayRect.w &&
                y >= howToPlayRect.y && y <= howToPlayRect.y + howToPlayRect.h) {
                state = HOW_TO_PLAY;
            }
            if (x >= backButtonRect.x && x <= backButtonRect.x + backButtonRect.w &&
                y >= backButtonRect.y && y <= backButtonRect.y + backButtonRect.h) {
                state = MAIN_MENU;
            }
        }
        else if (state == HOW_TO_PLAY) {
            if (x >= backButtonRect.x && x <= backButtonRect.x + backButtonRect.w &&
                y >= backButtonRect.y && y <= backButtonRect.y + backButtonRect.h) {
                state = LOAD_SCREEN;
            }
        }
    }

    return false;
}
