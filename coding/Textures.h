#ifndef TEXTURES_H
#define TEXTURES_H
#include <SFML/Graphics.hpp>

class Textures{
    private:
        sf::Texture brick;
    public:
        Textures();
        sf::Texture* getBrick();
};

#endif