#ifndef ENTITY_H
#define ENTITY_H
#include "SFML/Graphics.hpp"
#include "Platform.h"

#include <iostream>
#include <cmath>
#include <vector>


class Entity{ //Non-player character, may eventually be implemented into the player

    private:
        
    public:
        Player* target;
        double health;
        sf::Vector2f pos;
        sf::Sprite sprite;
        sf::Texture texture;
        sf::Texture* spriteSheet;

        double jumpVelocity;
        double speed;
        double gravity = 1; //Gravitational acceleration
        double maxXVelocity = 10;
        double maxYVelocity = 20;

        double MAXHEALTH;
        double MINHEALTH;

        int DAMAGEFRAMES;
        int damageFrameCount;

        enum State {
            Idle, Left, Right, Jumping, LeftHostile, RightHostile, JumpingHostile, LeftAttacking, RightAttacking
        };

        State state;

        double detectionRange; //Player detection range (circular radius)
        double attackRange; //Range of attack
        bool attacking;

        double attackCooldown; //How long in between attacks
        double entityCooldown; //How long until the entity can attack again


        double getHealth();

        sf::Sprite& getSprite();
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
        bool withinDetectionRange();
        bool withinAttackRange();
        bool jumpable(Platform* p);

        Entity(Player* player);
};

#endif