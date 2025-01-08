#include "Goblin.h"

// (71, 106, 64)

Goblin::Goblin(Player* player) : Entity(player){ //Explicitly call entity constructor because we need the player
    MAXHEALTH = 50;
    detectionRange = 100;
    attackRange = 20;
    jumpVelocity = 20;
    speed = 10;

    std::cout << "A" << std::endl;

    if(!texture.loadFromFile("textures/goblin.png")){
        std::cerr << "There was an error loading goblin.png in Goblin.cpp!" << std::endl;
    }else{
        std::cout << "goblin.png loaded successfully!" << std::endl;
        sprite.setTexture(texture);
    }

    std::cout << "B" << std::endl;
    setPos(sf::Vector2f(500, 500));
}

void Goblin::damage(double damage){

}



bool Goblin::attack(){
    return false;
}

sf::Vector2f Goblin::update(){
    if(withinDetectionRange()){
        attacking = true;
    }
    if(target->getPos().x - pos.x >= 0){
        //Player is on the left of the goblin
        if(attacking){
            if(withinAttackRange() && entityCooldown <= 0){
                state = LeftAttacking;
            }else{
                state = LeftHostile;
            }
        }else{
            state = Idle;
        }
    }else{
        //Player is on the right of the goblin
        if(attacking){
            if(withinAttackRange() && entityCooldown <= 0){
                state = RightAttacking;
            }else{
                state = RightHostile;
            }
        }else{
            state = Idle;
        }
    }
    return sf::Vector2f();
}