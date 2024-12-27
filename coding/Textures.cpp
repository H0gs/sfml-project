#include <SFML/Graphics.hpp>
#include<iostream>
#include "Textures.h"

sf::Texture brick;

Textures::Textures(){
    // if(!brick.loadFromFile("textures/brick.png")){
    //     std::cout << "Error loading brick.png" << std::endl;
    // }else{
    //     std::cout << "Brick texture loaded successfully!" << std::endl;
    // }
    
}
sf::Texture *Textures::getBrick()
{
    return &brick;
}