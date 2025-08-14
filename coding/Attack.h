#ifndef ATTACK_H
#define ATTACK_H

#include <vector>
#include <iostream>
#include "SFML/Graphics.hpp"

class Attack{
    private:
        //Nothing
    public:
        int duration; //Measured in frames
        int currentFrame; //Current frame of the attack
        double damage;

        sf::Vector2f pos;
        sf::Sprite sprite;
        sf::Texture texture;

        void update();

};

#endif