#ifndef ITEM_H
#define ITEM_H

#include <vector>
#include <iostream>
#include "SFML/Graphics.hpp"

class Item{
    public:
        std::string name;
        double damage;
        double knockback;
        int cooldown; //Measured in frames

        double range;
    private:

};
#endif