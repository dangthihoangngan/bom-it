#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

enum GameMode { SINGLE_PLAYER, TWO_PLAYER };

enum MenuState {
    MAIN_MENU,
    LOAD_SCREEN,
    HOW_TO_PLAY
};

class Menu {
public:
    GameMode selectedGameMode = SINGLE_PLAYER;
    Menu(SDL_Renderer* renderer);
    ~Menu();

    void render();
    bool handleEvent(SDL_Event& e);
    MenuState getState() const { return state; }

private:
    SDL_Renderer* renderer;
    MenuState state;

    SDL_Texture* backgroundTexture;
    SDL_Texture* singlePlayerButton;
    SDL_Texture* twoPlayerButton;
    SDL_Texture* playNowButton;
    SDL_Texture* howToPlayButton;
    SDL_Texture* howToPlayImage;
    SDL_Texture* backButton;

    SDL_Rect singlePlayerRect;
    SDL_Rect twoPlayerRect;
    SDL_Rect playNowRect;
    SDL_Rect howToPlayRect;
    SDL_Rect backButtonRect;

    bool showingHowToPlay = false;
};

#endif

