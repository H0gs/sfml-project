#include "Acid.h"

double damage = 1;

int Acid::getDamage(){
    return damage;
}
void Acid::onCollide(Player* player){
    player->damage();
    // std::cout << "Acid Damage!" << std::endl;
}
Acid::Acid(){
    setTexture("textures/acid.png");
}
Acid::Acid(sf::Vector2f pos){
    setPos(pos);
    setTexture("textures/acid.png");
}