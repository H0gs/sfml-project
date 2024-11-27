#include <iostream>
#include <SFML/Graphics.hpp>

class Platform{
    private:
        sf::Texture texture;
        sf::Vector2f pos = sf::Vector2f(0, 0);
        sf::Sprite sprite;
        double damage;

    public:
        void setX(double xPos){
            pos.x = xPos;
            sprite.setPosition(pos);
        }
        void setY(double yPos){
            pos.y = yPos;
            sprite.setPosition(pos);
        }
        void setPos(sf::Vector2f newPos){
            pos = newPos;
            sprite.setPosition(pos);
        }
        void setTexture(std::string path){
            texture.loadFromFile(path);
            sprite.setTexture(texture);
            sprite.setScale(sf::Vector2f(7, 3));
            sprite.setTextureRect(sf::Rect(25, 25, 25, 25));
            // distance from top left, distance from the point we just established as the distance from top left
            std::cout << sprite.getLocalBounds().height;
        }
        void setTexture(sf::Texture t){
            texture = t;
            sprite.setTexture(texture);
            sprite.setScale(sf::Vector2f(4, 4));
            sprite.setTextureRect(sf::Rect(0, 0, 16, 16));
        }
        void setSize(sf::Vector2f size){
            sprite.setScale(size.x / sprite.getLocalBounds().width, size.y / sprite.getLocalBounds().height);
        }
        sf::Sprite getSprite(){
            return sprite;
        }
        sf::Vector2f getPos(){
            return pos;
        }
        sf::Vector2f getSize(){
            return sf::Vector2f(sprite.getLocalBounds().width * sprite.getScale().x, sprite.getLocalBounds().height * sprite.getScale().y);
        }
        double getHeight(){
            return sprite.getLocalBounds().height * sprite.getScale().y;
        }
        double getWidth(){
            return sprite.getLocalBounds().width * sprite.getScale().x;
        }
        double getDamage(){
            return damage;
        }
        Platform(){

        }
        Platform(sf::Vector2f startingPos){
            pos = startingPos;
        }
};