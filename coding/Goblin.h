#ifndef GOBLIN_H
#define GOBLIN_H

#include "iostream"
#include "Entity.h"

class Goblin : public Entity{
    private:
        
    public:
        Goblin(Player* player);
        bool attack() override;
        void damage(double damage) override;
        sf::Vector2f update() override;
};

#endif