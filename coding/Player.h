#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "Platform.h"

class Platform; //Forward declaration needed because of the circular import

class Player{
    private:
    //Constants / modifiers
        double acceleration;
        double jumpVelocity;
        double maxXVelocity;
        double maxYVelocity;
        double decelerationMultiplier;
        double minVelocity;
        double collisionElasticity; //A double from 0-1 which is multiplied by the velocity determining how fast the player is moving after colliding into something, such as a platform. 
        double gravity; //Gravitational acceleration
        double MAXHEALTH;
    public:
        // Other stuff
        sf::Texture texture;
        sf::Vector2f pos;
        sf::Sprite sprite;
        bool right = false;
        bool left = false;
        bool up = false;
        bool down = false;
        bool jumping = false;
        bool canJump = false;
        double xVelocity;
        double yVelocity;
        double health;
    public:
        void setX(double xPos);
        void setY(double yPos);
        void setPos(sf::Vector2f newPos);
        void move(sf::Vector2f movement);
        void setTexture(std::string path);
        void setUp(bool set);
        void setDown(bool set);
        void setRight(bool set);
        void setLeft(bool set);
        void setJumping(bool set);
        void damage();
        void damage(double damage);
        sf::Vector2f movementUpdate(std::vector<std::unique_ptr<Platform>>& platforms);
        sf::Sprite getSprite();
        sf::Vector2f getPos();
        sf::Vector2f getSize();
        double getHeight();
        double getWidth();
        // bool collides(Platform platform, Player player);
        // bool collides(std::unique_ptr<Platform> platform);
        bool collides(Platform platform);
        bool collides(Platform* platform);
        // bool collidesBottom(std::unique_ptr<Platform> platform); //This is pretty much only used for jumping
        bool collidesBottom(Platform* platform); //This is pretty much only used for jumping
        bool collides(std::vector<Platform> platforms);
        double getHealth();
        Player();
};

#endif