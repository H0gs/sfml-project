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
    ID = 0;
}

//For comparing platform objects, used in the scramble methods in player and entity
bool Platform::equals(Platform platform){
    return (sprite.getTexture() == platform.getSprite().getTexture()) &&  // Compare texture pointers
           (sprite.getTextureRect() == platform.getSprite().getTextureRect()) &&  // Compare texture rect
           (sprite.getPosition() == platform.getSprite().getPosition()) &&  // Compare position
           (sprite.getScale() == platform.getSprite().getScale()) &&  // Compare scale
           (sprite.getRotation() == platform.getSprite().getRotation());  // Compare rotation
}

std::unique_ptr<Platform> Platform::duplicate(){
    // Platform platform;
    auto platform = std::make_unique<Platform>();

    sf::Image image;
    image.create(100, 50, sf::Color::Red); // Fill with white

    platform->texture.loadFromImage(image);


    platform->setPos(pos);
    platform->setSize(getSize());
    
    

    return platform;
}

Platform Platform::duplicate2(){
    Platform p;
    return p;
}

FakePlatform Platform::toFakePlatform(){
    return FakePlatform(pos, getSize(), ID);
}
void Platform::setID(int ID){
    this->ID = ID;
}
int Platform::getID(){
    return ID;
}