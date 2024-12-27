#include "Entity.h"

//test

Entity::Entity()
{
    if(!texture.loadFromFile("textures/placeholder.png")){
        std::cout << "There was an issue loading placeholder.png in Entity.cpp" << std::endl;
    }
}

sf::Vector2f Entity::update()
{
    return sf::Vector2f();
}

/**
 * @brief Attempts to attack the current target
 * @return A boolean indicating whether or not a player was dealt damage
 */
bool Entity::attack(){
    return false;
}

bool Entity::collides(Platform platform)
{
    return false;
}

bool Entity::collides(Platform* platform){
    return false;
}

bool Entity::collidesBottom(Platform* platform){ //This is pretty much only used for jumping
    return 
        ((pos.x > platform->getPos().x && pos.x < platform->getPos().x + platform->getSize().x) || (pos.x + getSize().x > platform->getPos().x && pos.x + getSize().x < platform->getPos().x + platform->getSize().x))
    &&
        (pos.y + getSize().y > platform->getPos().y && pos.y + getSize().y < platform->getPos().y + platform->getSize().y);
}

double Entity::getHealth()
{
    return health;
}

void Entity::damage(double d){

}

sf::Sprite Entity::getSprite()
{
    return sprite;
}

sf::Vector2f Entity::getPos()
{
    return pos;
}

sf::Vector2f Entity::getSize()
{
    return sf::Vector2f(sprite.getLocalBounds().width * sprite.getScale().x, sprite.getLocalBounds().height * sprite.getScale().y);
}

void Entity::setPos(sf::Vector2f p)
{
    pos = p;
    sprite.setPosition(pos);
}

void Entity::move(sf::Vector2f move){
    pos.x += move.x;
    pos.y += move.y;
    sprite.setPosition(pos);
}

