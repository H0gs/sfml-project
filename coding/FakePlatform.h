#ifndef FAKEPLATFORM_H
#define FAKEPLATFORM_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "Player.h"

class FakePlatform{
    private:
        sf::Vector2f pos;
        sf::Vector2f size;
        int ID;
    public:
        FakePlatform();
        FakePlatform(sf::Vector2f p, sf::Vector2f s, int ID);
        void setPos(sf::Vector2f p);
        void setSize(sf::Vector2f s);
        sf::Vector2f getPos();
        sf::Vector2f getSize();
        double getWidth();
        double getHeight();
        int getID();
        bool operator==(const FakePlatform& other) const;
};

#endif