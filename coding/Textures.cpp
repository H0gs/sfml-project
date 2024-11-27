#include <SFML/Graphics.hpp>
#include<iostream>

class Textures{
    private:
        sf::Texture brick;
    public:
        Textures(){
            // if(!brick.loadFromFile("textures/brick.png")){
            //     std::cout << "Error loading brick.png" << std::endl;
            // }else{
            //     std::cout << "Brick texture loaded successfully!" << std::endl;
            // }
            
        }
        sf::Texture* getBrick(){
            return &brick;
        }
};