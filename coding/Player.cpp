#include "Player.h"

Player::Player(){
    acceleration = 2;
    jumpVelocity = 20;
    maxXVelocity = 10;
    maxYVelocity = 20;
    decelerationMultiplier = 0.7;
    minVelocity = 0.1;
    collisionElasticity = 0; //A double from 0-1 which is multiplied by the velocity determining how fast the player is moving after colliding into something, such as a platform. 
    gravity = 1; //Gravitational acceleration
    MAXHEALTH = 100;
    MINHEALTH = 0;
    DAMAGEFRAMES = 10;
    damageFrameCount = 0;

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
    damageFrameCount = 10;
    if(health - 0.5 > MINHEALTH){
        health -= 0.5;
    }else{
        health = 0;
    }
}
void Player::damage(double damage){
    damageFrameCount = 10;
    if(health - damage > MINHEALTH){
        health -= damage;
    }else{
        health = 0;
    }
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
    if(right){ //Uncoment this bit out when your done testing so that the player has smoother movement
        // if(xVelocity < maxXVelocity){
        //     xVelocity += acceleration;
        // }else{
            xVelocity = maxXVelocity;
        // }
    }
    if (left){
        // if(xVelocity > -maxXVelocity){
        //     xVelocity -= acceleration;
        // }else{
            xVelocity = -maxXVelocity;
        // }
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
            platform.get()->onCollide(this); //platform.get() returns a Platform*, aka a raw Platform pointer
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

    //Damage frame updates
    if(damageFrameCount > 0){
        damageFrameCount--;
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

/*

_____________
    ___

    /\ a platform setup like this may not work
    |
*/



bool Player::jumpable(Platform* start, Platform* end){
    double xPos1 = end->getPos().x;
    double xPos2 = end->getPos().x + end->getWidth();

    if(xPos1 > start->getPos().x && xPos1 < start->getPos().x + start->getWidth()){
        //xPos1 is valid
        if(jumpableHelper(end, sf::Vector2f(xPos1, start->getPos().y - getSize().y))){
            return true;
        }
    }
    if(xPos2 > start->getPos().x && xPos2 < start->getPos().x + start->getWidth()){
        //xPos2 is valid
        if(jumpableHelper(end, sf::Vector2f(xPos2, start->getPos().y - getSize().y))){
            return true;
        }
    }

    return false;
}

bool Player::jumpableHelper(Platform* platform, sf::Vector2f p){
    if(
        (p.x + getSize().x > platform->getPos().x && p.x < platform->getPos().x + platform->getWidth()) && //At least some part of the player/entity is between the ends of the platform
        (p.y < platform->getPos().y)
    ){
        return true;
    }

    double yPositiontracker = 0;
    double xPositiontracker = 0;
    double yVelocityTracker = jumpVelocity;

    double yDistance = p.y - platform->getPos().y + getSize().y;

    double apexDist = 0;

    while(yPositiontracker >= yDistance || yVelocityTracker > 0){
        // std::cout << "Working" << std::endl;
        yVelocityTracker -= gravity;
        yPositiontracker += yVelocityTracker;
        xPositiontracker += maxXVelocity;
        if(yPositiontracker > apexDist){
            apexDist = yPositiontracker;
        }
    }

    //Find the corner of the platfrom that is closest to the entity / player, this is the corner we should jump to
    double xDistance;

    double leftDist = abs(p.x + getSize().x - platform->getPos().x);
    double rightDist = abs(platform->getPos().x + platform->getSize().x - p.x);

    //Figure out if the player is closer to the left or right side, and then calculate the minimum distance based on that
    if(leftDist > rightDist){
        xDistance = rightDist;
    }else{
        xDistance = leftDist;
    }

    // return abs(pos.x - platform->getPos().x) <= xPositiontracker && (yPositiontracker - jumpVelocity < platform->getPos().y);
    return xDistance < xPositiontracker && //Simulated maximum x distance is greater than the distance needed to travel -> works (I think)
            // (yPositiontracker - jumpVelocity < platform->getPos().y) && //No clue what this does, maybe an attempt to check what I do on the next line?
            apexDist > yDistance; //This was the bug fix for pathfinding
}

bool Player::jumpable(FakePlatform start, FakePlatform end){
    double xPos1 = end.getPos().x;
    double xPos2 = end.getPos().x + end.getWidth();

    if(xPos1 > start.getPos().x && xPos1 < start.getPos().x + start.getWidth()){
        //xPos1 is valid
        if(jumpableHelper(end, sf::Vector2f(xPos1, start.getPos().y - getSize().y))){
            return true;
        }
    }
    if(xPos2 > start.getPos().x && xPos2 < start.getPos().x + start.getWidth()){
        //xPos2 is valid
        if(jumpableHelper(end, sf::Vector2f(xPos2, start.getPos().y - getSize().y))){
            return true;
        }
    }

    return false;
}

bool Player::jumpableHelper(FakePlatform platform, sf::Vector2f p){
    if(
        (p.x + getSize().x > platform.getPos().x && p.x < platform.getPos().x + platform.getWidth()) && //At least some part of the player/entity is between the ends of the platform
        (p.y < platform.getPos().y)
    ){
        return true;
    }

    double yPositiontracker = 0;
    double xPositiontracker = 0;
    double yVelocityTracker = jumpVelocity;

    double yDistance = p.y - platform.getPos().y + getSize().y;

    while(yPositiontracker >= yDistance || yVelocityTracker > 0){
        yVelocityTracker -= gravity;
        yPositiontracker += yVelocityTracker;
        xPositiontracker += maxXVelocity;
    }

    //Find the corner of the platfrom that is closest to the entity / player, this is the corner we should jump to
    double xDistance;

    double leftDist = abs(p.x + getSize().x - platform.getPos().x);
    double rightDist = abs(platform.getPos().x + platform.getSize().x - p.x);

    if(leftDist > rightDist){
        //If the left corner is farther away than the right corner
        xDistance = rightDist;
    }else{
        xDistance = leftDist;
    }

    // return abs(pos.x - platform->getPos().x) <= xPositiontracker && (yPositiontracker - jumpVelocity < platform->getPos().y);
    return xDistance < xPositiontracker && (yPositiontracker - jumpVelocity < platform.getPos().y);
}

bool Player::contains(std::vector<FakePlatform> platforms, FakePlatform platform){
    for(FakePlatform p : platforms){
        if(p == platform){
            return true;
        }
    }
    return false;
}

void Player::scrambleHelper(std::vector<FakePlatform> origin, std::vector<FakePlatform> vec, std::vector<std::vector<FakePlatform>>* storage){
    storage->push_back(vec);
    if(vec.size() != origin.size()){
        for(FakePlatform p : origin){
            if(!contains(vec, p)){
                std::vector<FakePlatform> temp = vec;
                temp.push_back(p);
                scrambleHelper(origin, temp, storage);
            }
        }
    }
}

//Assumes currentPlatform is not null
std::vector<FakePlatform> Player::mostEfficient(std::vector<std::vector<FakePlatform>> storage, std::vector<std::unique_ptr<Platform>>& platforms){
    for(std::vector<FakePlatform> vec : storage){
        if(storage.size() == 0){
            
        }
        bool usable = true;
        for(int i = 0; i < vec.size() - 1; i++){
            if(!jumpable(vec.at(i), vec.at(i + 1))){
                usable = false;
            }
        }
        if(usable){
            Platform* current = currentPlatform(platforms);
            if(vec.at(0) == current->toFakePlatform()){
                
            }
            return vec;
        }
    }
    std::vector<FakePlatform> a;
    return a;
}

/**
 * @brief This will be used to find the most effecient route to the player by looping through the return array, determining if each oute is possible, and calculating time for each route. Each route is represented by a vector of Platforms to be followed in order of looping. 
 * @param platforms The unique_ptr Platform array, holding all pf the games Platforms. 
 * @return A vector of vectors for every order of the platforms. 
 */
std::vector<std::vector<FakePlatform>> Player::scramble(std::vector<std::unique_ptr<Platform>> platforms){
    std::vector<FakePlatform> origin;
    for(std::unique_ptr<Platform>& platform : platforms){
        FakePlatform fp = FakePlatform(platform->getPos(), platform->getSize(), platform->getID());
        origin.push_back(fp);
    }

    std::vector<FakePlatform> vec;
    std::vector<std::vector<FakePlatform>> storage;

    scrambleHelper(origin, vec, &storage);

    return storage;
}


Platform* Player::currentPlatform(std::vector<std::unique_ptr<Platform>>& platforms)
{
    pos.y += 20;
    for(std::unique_ptr<Platform>& platform : platforms){
        if(collides(platform.get())){
            // std::cout << "Success" << std::endl;
            pos.y -= 20;
            return platform.get();
        }else{
            // std::cout << "fail" << std::endl;
        }
    }
    pos.y -= 20;
    return nullptr;
    
}


bool Player::jumpableHelper(Platform* platform){
    return jumpableHelper(platform, pos);
}

double Player::getHealth(){
    return health;
}

int Player::getDamageFrameCount(){
    return damageFrameCount;
}