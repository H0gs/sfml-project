#include "Platform.h"

void Platform::setX(double xPos){
    pos.x = xPos;
    sprite.setPosition(pos);
}
void Platform::setY(double yPos){
    pos.y = yPos;
    sprite.setPosition(pos);
}
void Platform::setPos(sf::Vector2f newPos){
    pos = newPos;
    sprite.setPosition(pos);
}
void Platform::setTexture(std::string path){
    texture.loadFromFile(path);
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(7, 3));
    sprite.setTextureRect(sf::Rect(25, 25, 25, 25));
    // distance from top left, distance from the point we just established as the distance from top left
    std::cout << sprite.getLocalBounds().height;
}
void Platform::setTexture(sf::Texture t){
    texture = t;
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(4, 4));
    sprite.setTextureRect(sf::Rect(0, 0, 16, 16));
}
void Platform::setSize(sf::Vector2f size){
    sprite.setScale(size.x / sprite.getLocalBounds().width, size.y / sprite.getLocalBounds().height);
}
void Platform::onCollide(Player* player){
    // std::cout << "Platform Collide!" << std::endl;
}
sf::Sprite Platform::getSprite(){
    return sprite;
}
sf::Vector2f Platform::getPos(){
    return pos;
}
sf::Vector2f Platform::getSize(){
    return sf::Vector2f(sprite.getLocalBounds().width * sprite.getScale().x, sprite.getLocalBounds().height * sprite.getScale().y);
}
double Platform::getHeight(){
    return sprite.getLocalBounds().height * sprite.getScale().y;
}
double Platform::getWidth(){
    return sprite.getLocalBounds().width * sprite.getScale().x;
}
double Platform::getDamage(){
    return damage;
}
Platform::Platform(){

}
Platform::Platform(sf::Vector2f startingPos){
    pos = startingPos;
}
