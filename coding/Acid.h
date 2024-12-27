#ifndef ACID_H
#define ACID_H
#include <SFML/Graphics.hpp>
#include <memory>
#include "Platform.h"
#include "Player.h"

class Acid : public Platform{
    private: 
        double damage;
    public:
        int getDamage();
        void onCollide(Player* player) override;
        Acid();
        Acid(sf::Vector2f pos);
};

#endif