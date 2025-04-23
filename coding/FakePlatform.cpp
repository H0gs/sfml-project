#include "FakePlatform.h"

FakePlatform::FakePlatform(){
    pos = sf::Vector2f(0, 0);
    size = sf::Vector2f(0, 0);
}

FakePlatform::FakePlatform(sf::Vector2f p, sf::Vector2f s, int ID){
    pos = p;
    size = s;
    this->ID = ID;
}

void FakePlatform::setPos(sf::Vector2f p){
    pos = p;
}

void FakePlatform::setSize(sf::Vector2f s){
    size = s;
}

sf::Vector2f FakePlatform::getPos(){
    return pos;
}

sf::Vector2f FakePlatform::getSize(){
    return size;
}

double FakePlatform::getWidth(){
    return size.x;
}

double FakePlatform::getHeight(){
    return size.y;
}
int FakePlatform::getID(){
    return ID;
}

//Made this so we can use == in the Player and Entity scramble methods
bool FakePlatform::operator==(const FakePlatform& other) const {
    return pos == other.pos && size == other.size && ID == other.ID;
}

bool FakePlatform::operator!=(const FakePlatform& other) const {
    return !(*this == other);
}