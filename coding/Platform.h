#ifndef PLATFORM_H
#define PLATFORM_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "Player.h"

class Player; //Forward declaration needed because of the circular import

class Platform{
    private:
        sf::Texture texture;
        sf::Vector2f pos;
        sf::Sprite sprite;
        double damage;
    public:
        void setX(double xPos);
        void setY(double yPos);
        void setPos(sf::Vector2f newPos);
        void setTexture(std::string path);
        void setTexture(sf::Texture t);
        void setSize(sf::Vector2f size);
        virtual void onCollide(Player player); // Overriden by subclasses, such as Acid
        sf::Sprite getSprite();
        sf::Vector2f getPos();
        sf::Vector2f getSize();
        double getHeight();
        double getWidth();
        double getDamage();
        Platform();
        Platform(sf::Vector2f startingPos);
};

#endif