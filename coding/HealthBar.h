#ifndef HEALTHBAR_H
#define HEALTHBAR_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"

class HealthBar{
    private:
        sf::Vector2f pos;
        sf::Texture red;
        sf::Texture black;
        sf::Texture container;

        sf::Sprite redSprite;
        sf::Sprite blackSprite;
        sf::Sprite containerSprite;

        sf::RenderTexture texture;
        sf::Sprite sprite;
    public:
        sf::Vector2f getPos();
        sf::Texture getTexture();
        sf::Sprite getSprite();
        HealthBar();
        void update(Player player);
        void setPos(sf::Vector2f p);
        void move(sf::Vector2f m);
};

#endif