#include "Player.h"

Player::Player(){
    acceleration = 2;
    jumpVelocity = 15;
    maxXVelocity = 10;
    maxYVelocity = 20;
    decelerationMultiplier = 0.7;
    minVelocity = 0.1;
    collisionElasticity = 0; //A double from 0-1 which is multiplied by the velocity determining how fast the player is moving after colliding into something, such as a platform. 
    gravity = 1; //Gravitational acceleration
    MAXHEALTH = 100;

    // Other stuff
    pos = sf::Vector2f(0, 0);
    right = false;
    left = false;
    up = false;
    down = false;
    jumping = false;
    canJump = false;
    xVelocity = 0;
    yVelocity = 0;
    health = 100;
}

void Player::setX(double xPos){
    pos.x = xPos;
    sprite.setPosition(pos);
}
void Player::setY(double yPos){
    pos.y = yPos;
    sprite.setPosition(pos);
}
void Player::setPos(sf::Vector2f newPos){
    pos = newPos;
    sprite.setPosition(pos);
}
void Player::move(sf::Vector2f movement){
    pos.x += movement.x;
    pos.y += movement.y;
    sprite.setPosition(pos);
}
void Player::setTexture(std::string path){
    texture.loadFromFile(path);
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(4, 4));
    // std::cout << sprite.getLocalBounds().height;
}
void Player::setUp(bool set){
    up = set;
}
void Player::setDown(bool set){
    down = set;
}
void Player::setRight(bool set){
    right = set;
}
void Player::setLeft(bool set){
    left = set;
}
void Player::setJumping(bool set){
    jumping = set;
}
void Player::damage(){
    health -= 0.5;
}
void Player::damage(double damage){
    health -= damage;
}

/**
 * @brief Updates the players position based on its x and y velocities. 
 * @param platforms A std::vector<Platform> representing the total platform objects in the game to be used for collisions. 
 * @return A sf::vector2f representing the player's new x and y, relative to its old x and y positions. 
 */
sf::Vector2f Player::movementUpdate(std::vector<std::unique_ptr<Platform>>& platforms){
    // Main movement section

    float oldX = pos.x;
    float oldY = pos.y;

    if(up){
        if(yVelocity > -maxYVelocity){
            yVelocity -= acceleration;
        }else{
            yVelocity = -maxYVelocity;
        }
    }
    if (down){
        if(yVelocity < maxYVelocity){
            yVelocity += acceleration;
        }else{
            yVelocity = maxYVelocity;
        }
    }
    if(right){
        if(xVelocity < maxXVelocity){
            xVelocity += acceleration;
        }else{
            xVelocity = maxXVelocity;
        }
    }
    if (left){
        if(xVelocity > -maxXVelocity){
            xVelocity -= acceleration;
        }else{
            xVelocity = -maxXVelocity;
        }
    }
    if(!right && !left){
        xVelocity *= decelerationMultiplier;
        if(std::abs(xVelocity) < minVelocity){
            xVelocity = 0;
        }
    }
    yVelocity += gravity;

    pos.x += xVelocity;
    pos.y += yVelocity;
    bool collided = false;
    for(std::unique_ptr<Platform> &platform : platforms){
        if(collides(platform.get())){
            pos.x -= xVelocity;
            pos.y -= yVelocity;
            platform.get()->onCollide(*this); //platform.get() returns a Platform*, aka a raw Platform pointer
            collided = true;
            break;
        }
    }
    if(!collided){
        pos.y -= yVelocity;
        pos.x -= xVelocity;
    }

    //X collisions
    pos.x += xVelocity;
    bool collidedX = false;
    for(std::unique_ptr<Platform> &platform : platforms){
        if(collides(platform.get())){
            collidedX = true;
        }
    }
    if(collidedX){
        pos.x -= xVelocity;
    }
    //Y collisions
    pos.y += yVelocity;
    bool collidedY = false;
    for(std::unique_ptr<Platform>& platform : platforms){
        if(collides(platform.get())){
            collidedY = true;
            if(collidesBottom(platform.get())){
                canJump = true;
            }
        }
    }
    if(collidedY){
        pos.y -= yVelocity;
        yVelocity = 0;
        //Jumping can go in here as collidedY represents whether or not they have made contact with the ground. 
        if(jumping && canJump){
            yVelocity = -jumpVelocity;
            canJump = false;
        }else{
        }
    }
    
    sprite.setPosition(pos);

    return sf::Vector2f(pos.x - oldX, pos.y - oldY);
}
sf::Sprite Player::getSprite(){
    return sprite;
}
sf::Vector2f Player::getPos(){
    return pos;
}
sf::Vector2f Player::getSize(){
    return sf::Vector2f(sprite.getLocalBounds().width * sprite.getScale().x, sprite.getLocalBounds().height * sprite.getScale().y);
}
double Player::getHeight(){
    return sprite.getLocalBounds().height * sprite.getScale().y;
}
double Player::getWidth(){
    return sprite.getLocalBounds().width * sprite.getScale().x;
}

// bool Player::collides(std::unique_ptr<Platform> platform){
//     return 
//         ((pos.x >= platform->getPos().x && pos.x <= platform->getPos().x + platform->getSize().x) || (pos.x + getSize().x >= platform->getPos().x && pos.x + getSize().x <= platform->getPos().x + platform->getSize().x))
//     &&
//         ((pos.y >= platform->getPos().y && pos.y <= platform->getPos().y + platform->getSize().y) || (pos.y + getSize().y >= platform->getPos().y && pos.y + getSize().y <= platform->getPos().y + platform->getSize().y));
// }

bool Player::collides(Platform* platform){
    return 
        ((pos.x >= platform->getPos().x && pos.x <= platform->getPos().x + platform->getSize().x) || (pos.x + getSize().x >= platform->getPos().x && pos.x + getSize().x <= platform->getPos().x + platform->getSize().x))
    &&
        ((pos.y >= platform->getPos().y && pos.y <= platform->getPos().y + platform->getSize().y) || (pos.y + getSize().y >= platform->getPos().y && pos.y + getSize().y <= platform->getPos().y + platform->getSize().y));
}

bool Player::collides(Platform platform){
    return 
        ((pos.x >= platform.getPos().x && pos.x <= platform.getPos().x + platform.getSize().x) || (pos.x + getSize().x >= platform.getPos().x && pos.x + getSize().x <= platform.getPos().x + platform.getSize().x))
    &&
        ((pos.y >= platform.getPos().y && pos.y <= platform.getPos().y + platform.getSize().y) || (pos.y + getSize().y >= platform.getPos().y && pos.y + getSize().y <= platform.getPos().y + platform.getSize().y));
}

// bool Player::collidesBottom(std::unique_ptr<Platform> platform){ //This is pretty much only used for jumping
//     return 
//         ((pos.x > platform->getPos().x && pos.x < platform->getPos().x + platform->getSize().x) || (pos.x + getSize().x > platform->getPos().x && pos.x + getSize().x < platform->getPos().x + platform->getSize().x))
//     &&
//         (pos.y + getSize().y > platform->getPos().y && pos.y + getSize().y < platform->getPos().y + platform->getSize().y);
// }

bool Player::collidesBottom(Platform* platform){ //This is pretty much only used for jumping
    return 
        ((pos.x > platform->getPos().x && pos.x < platform->getPos().x + platform->getSize().x) || (pos.x + getSize().x > platform->getPos().x && pos.x + getSize().x < platform->getPos().x + platform->getSize().x))
    &&
        (pos.y + getSize().y > platform->getPos().y && pos.y + getSize().y < platform->getPos().y + platform->getSize().y);
}

bool Player::collides(std::vector<Platform> platforms){
    for(Platform platform : platforms){
        if(collides(platform)){
            return true;
        }
    }
    return false;
}
double Player::getHealth(){
    return health;
}