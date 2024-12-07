#include "HealthBar.h"

sf::Vector2f HealthBar::getPos(){
    return pos;
}
sf::Texture HealthBar::getTexture(){
    return texture.getTexture();
}
sf::Sprite HealthBar::getSprite(){
    return sprite;
}
HealthBar::HealthBar(){
    if(!container.loadFromFile("textures/healthContainer.png")){
        std::cout << "Error loading healthContainer.png" << std::endl;
    }else{
        std::cout << "healthContainer.png loaded successfully!" << std::endl;
    }

    if(!red.loadFromFile("textures/red2.png")){
        std::cout << "Error loading red2.png" << std::endl;
    }else{
        std::cout << "red2.png loaded successfully!" << std::endl;
    }

    if(!black.loadFromFile("textures/black.png")){
        std::cout << "Error loading black.png" << std::endl;
    }else{
        std::cout << "black.png loaded successfully!" << std::endl;
    }

    redSprite.setTexture(red);
    redSprite.setPosition(0, 0);

    blackSprite.setTexture(black);
    blackSprite.setPosition(256 - (100 * 256 / 100), 0);

    containerSprite.setTexture(container);
    containerSprite.setPosition(0, 0);

    texture.create(256, 64);
    texture.draw(redSprite);
    texture.draw(blackSprite);
    texture.draw(containerSprite);
    sprite.setTexture(texture.getTexture());
    
}
void HealthBar::update(Player player){
    texture.clear();


    redSprite.setTexture(red);
    redSprite.setPosition(0, 0);

    blackSprite.setTexture(black);
    blackSprite.setPosition(256 - ((100 - player.getHealth()) * 256 / 100), 0);

    containerSprite.setTexture(container);
    containerSprite.setPosition(0, 0);

    texture.create(256, 64);
    texture.draw(redSprite);
    texture.draw(blackSprite);
    texture.draw(containerSprite);
    sprite.setTexture(texture.getTexture());
}
void HealthBar::setPos(sf::Vector2f p){
    pos = p;
    sprite.setPosition(pos);
}
void HealthBar::move(sf::Vector2f m){
    setPos(sf::Vector2f(pos.x + m.x, pos.y + m.y));
}
