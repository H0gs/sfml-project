#ifndef ENTITY_H
#define ENTITY_H
#include "SFML/Graphics.hpp"
#include "Platform.h"

#include <iostream>


class Entity{ //Non-player character, may eventually be implemented into the player

    private:
        
    public:
        Player* target;
        double health;
        sf::Vector2f pos;
        sf::Sprite sprite;
        sf::Texture texture;

        double MAXHEALTH;
        double MINHEALTH;

        int DAMAGEFRAMES;
        int damageFrameCount;

        enum State {
            Left, Right, Jumping, LeftHostile, RightHostile, JumpingHostile, Attacking
        };

        double range; //Player detection range (square)

        double getHealth();

        sf::Sprite getSprite();
        sf::Vector2f getPos();
        sf::Vector2f getSize();

        void setPos(sf::Vector2f p);
        void move(sf::Vector2f movement);

        bool collides(Platform platform);
        bool collides(Platform* platform);
        bool collidesBottom(Platform* platform); //This is pretty much only used for jumping

        virtual sf::Vector2f update();
        virtual bool attack();
        virtual void damage(double dmg);

        Entity();
};

#endif