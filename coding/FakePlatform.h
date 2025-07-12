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
        sf::Vector2f getPos() const;
        sf::Vector2f getSize() const;
        double getWidth();
        double getHeight();
        int getID() const;
        bool operator==(const FakePlatform& other) const;
        bool operator!=(const FakePlatform& other) const;
};



//GPT Code: 
#include <functional>  // for std::hash

namespace std {
    template <>
    struct hash<FakePlatform> {
        size_t operator()(const FakePlatform& fp) const {
            size_t h1 = hash<float>()(fp.getPos().x);
            size_t h2 = hash<float>()(fp.getPos().y);
            size_t h3 = hash<float>()(fp.getSize().x);
            size_t h4 = hash<float>()(fp.getSize().y);
            size_t h5 = hash<int>()(fp.getID());

            // Combine hashes
            return (((((h1 ^ (h2 << 1)) ^ (h3 << 1)) ^ (h4 << 1)) ^ (h5 << 1)));
        }
    };
}

#endif