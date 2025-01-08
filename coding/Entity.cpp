#include "Entity.h"

//test

Entity::Entity(Player* player)
{
    target = player;
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

/**
 * @brief Can the entity, given a platform pointer jump to a platform? Takes into account the entity's jumpvelocity and movement speed. 
 * @param A pointer to the platform to jump to
 * @return A boolean indicating whether or not the entity can jump to the platform
 */


bool Entity::jumpable(Platform* platform){
    double yPositiontracker = 0;
    double xPositiontracker = 0;
    double yVelocityTracker = jumpVelocity;

    double yDistance = pos.y - platform->getPos().y + getSize().y;

    while(yPositiontracker >= yDistance || yVelocityTracker > 0){
        yVelocityTracker -= gravity;
        yPositiontracker += yVelocityTracker;
        xPositiontracker += maxXVelocity;
    }

    //Find the corner of the platfrom that is closest to the entity / player, this is the corner we should jump to
    double xDistance;

    double leftDist = abs(pos.x + getSize().x - platform->getPos().x);
    double rightDist = abs(platform->getPos().x + platform->getSize().x - pos.x);

    if(leftDist > rightDist){
        //If the left corner is farther away than the right corner
        xDistance = rightDist;
    }else{
        xDistance = leftDist;
    }



    // return abs(pos.x - platform->getPos().x) <= xPositiontracker && (yPositiontracker - jumpVelocity < platform->getPos().y);
    return xDistance < xPositiontracker && (yPositiontracker - jumpVelocity < platform->getPos().y); //where is -40 coming from???
}

double Entity::getHealth()
{
    return health;
}

void Entity::damage(double d){

}

bool Entity::withinDetectionRange(){
    return sqrt(pow(target->getPos().x - pos.x, 2) + pow(target->getPos().y - pos.y, 2)) <= detectionRange; //Pythagorean theorum to find distance between player and entity
}

bool Entity::withinAttackRange(){
    return sqrt(pow(target->getPos().x - pos.x, 2) + pow(target->getPos().y - pos.y, 2)) <= attackRange; //Pythagorean theorum to find distance between player and entity
}

sf::Sprite& Entity::getSprite()
{
    return sprite;
}

sf::Vector2f Entity::getPos(){
    return pos;
}

sf::Vector2f Entity::getSize(){
    return sf::Vector2f(sprite.getLocalBounds().width * sprite.getScale().x, sprite.getLocalBounds().height * sprite.getScale().y);
}

void Entity::setPos(sf::Vector2f p){
    pos = p;
    sprite.setPosition(pos);
}

void Entity::move(sf::Vector2f move){
    pos.x += move.x;
    pos.y += move.y;
    sprite.setPosition(pos);
}

