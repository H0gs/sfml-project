#ifndef ANIMATION_H
#define ANIMATION_H
#include <iostream>
#include "SFML/Graphics.hpp"

class Animation{
    private:
        sf::RenderTexture* texture;
        sf::Texture* textureSheet;
        sf::Vector2u size;
        int frameCount;
        int currentFrame;
        int textureNumber; //Which sprite the texture is on the sprite sheet vertically
        long long frameTime; //Time per frame (miliseconds)
        long long lastTime;
        long long timeCounter;
        bool newTexture; //Does the texture of the sprite need to be updated?
        
    public:
        sf::RenderTexture* getTexture();
        void updateTexture();
        Animation(sf::Texture* textureSheet, sf::Vector2u size, int textureNumber, long long frameTime);
        bool hasNewTexture();

};


#endif