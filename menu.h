#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();

    void render();          // Hàm vẽ menu
    bool handleEvent(SDL_Event& e);  // Xử lý sự kiện

private:
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    SDL_Texture* startButtonTexture;
    SDL_Rect startButton;
};

#endif
