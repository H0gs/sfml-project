#include "Entity.h"

//test

Entity::Entity(Player* player)
{
    target = player;
}

std::vector<FakePlatform> Entity::getPath(){
    return path;
}

sf::Vector2f Entity::update(std::vector<std::unique_ptr<Platform>>& platforms)
{
    framesSincePathRefresh++;
    //Temporary
    // std::cout << "Canjump: " << canJump << std::endl;

    //Velocity section
    yVelocity += gravity;

    pos.x += xVelocity;
    pos.y += yVelocity;
    bool collided = false;
    for(std::unique_ptr<Platform> &platform : platforms){
        if(collides(platform.get())){
            pos.x -= xVelocity;
            pos.y -= yVelocity;
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
        if(jumpInput){ //Ignoring canJump for testing
            canJump = false;
            yVelocity = -maxYVelocity;
        }
    }
    //Max velocity checking
    if(xVelocity > maxXVelocity){
        xVelocity = maxXVelocity;
    }else if(xVelocity < -maxXVelocity){
        xVelocity = -maxXVelocity;
    }
    if(yVelocity > maxYVelocity){
        yVelocity = maxXVelocity;
    }else if(yVelocity < -maxYVelocity){
        yVelocity = -maxYVelocity;
    }

    if(framesSincePathRefresh > PATH_REFRESH_FRAMES){
        if(currentPlatform(platforms) != nullptr && target->currentPlatform(platforms) != nullptr){
            framesSincePathRefresh = 0;
            // std::cout << "Generating path" << std::endl;
            std::vector<std::vector<FakePlatform>> temp = scramble(platforms);
            path = mostEfficient(temp, platforms);
        }else{
            // std::cout << "Unable to generate path" << std::endl;
        }
    }
    

    //States and behavior
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

    sprite.setPosition(pos);

    
    return sf::Vector2f();
}

/**
 * @brief Attempts to attack the current target
 * @return A boolean indicating whether or not a player was dealt damage
 */
bool Entity::attack(){
    return false;
}

bool Entity::collides(Platform* platform){
    return 
        ((pos.x >= platform->getPos().x && pos.x <= platform->getPos().x + platform->getSize().x) || (pos.x + getSize().x >= platform->getPos().x && pos.x + getSize().x <= platform->getPos().x + platform->getSize().x))
    &&
        ((pos.y >= platform->getPos().y && pos.y <= platform->getPos().y + platform->getSize().y) || (pos.y + getSize().y >= platform->getPos().y && pos.y + getSize().y <= platform->getPos().y + platform->getSize().y));
}

bool Entity::collides(Platform platform){
    return 
        ((pos.x >= platform.getPos().x && pos.x <= platform.getPos().x + platform.getSize().x) || (pos.x + getSize().x >= platform.getPos().x && pos.x + getSize().x <= platform.getPos().x + platform.getSize().x))
    &&
        ((pos.y >= platform.getPos().y && pos.y <= platform.getPos().y + platform.getSize().y) || (pos.y + getSize().y >= platform.getPos().y && pos.y + getSize().y <= platform.getPos().y + platform.getSize().y));
}

bool Entity::jumpable(FakePlatform start, FakePlatform end){

    //This section handles the case that end is directly above start, and end is wider than start (see below)
    /*
    ⏹⏹⏹⏹⏹ <--End
      ⏹⏹ <--Start
    we assume that given this scenario, it is impossible to get from start to end because of the lip. 
    */
    if(start.getPos().y > end.getPos().y){ //If end is above start
        double startX1 = start.getPos().x;
        double startX2 = startX1 + start.getWidth();

        double endX1 = end.getPos().x;
        double endX2 = endX1 + start.getWidth();

        if(endX1 < startX1 && endX2 > startX2){
            return false;
        }        
    }


    double xPos1 = start.getPos().x;
    double xPos2 = start.getPos().x + end.getWidth();


    if(jumpableHelper(end, sf::Vector2f(xPos1, start.getPos().y - getSize().y))){ //This one (still)
        return true;
    }
    
    if(jumpableHelper(end, sf::Vector2f(xPos2, start.getPos().y - getSize().y))){
        return true;
    }
    

    return false;
}

bool Entity::jumpableHelper(FakePlatform platform, sf::Vector2f p){
    // if(
    //     (p.x + getSize().x > platform.getPos().x && p.x < platform.getPos().x + platform.getWidth()) && //At least some part of the player/entity is between the ends of the platform
    //     (p.y < platform.getPos().y)
    // ){
    //     std::cout << "Top check" << std::endl;
    //     return true;
    // }

    double yPositiontracker = 0;
    double xPositiontracker = 0;
    double yVelocityTracker = jumpVelocity;

    double yDistance = p.y - platform.getPos().y + getSize().y;
    double apexDist = yPositiontracker;

    while(yPositiontracker >= yDistance || yVelocityTracker > 0){
        yVelocityTracker -= gravity;
        yPositiontracker += yVelocityTracker;
        xPositiontracker += maxXVelocity;
        if(yPositiontracker > apexDist){
            apexDist = yPositiontracker;
        }
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

    return xDistance < xPositiontracker && 
    apexDist > yDistance;
            // (yPositiontracker - jumpVelocity < platform.getPos().y);
}

bool Entity::collidesBottom(Platform* platform){ //This is pretty much only used for jumping
    return 
        ((pos.x > platform->getPos().x && pos.x < platform->getPos().x + platform->getSize().x) || (pos.x + getSize().x > platform->getPos().x && pos.x + getSize().x < platform->getPos().x + platform->getSize().x))
    &&
        (pos.y + getSize().y > platform->getPos().y && pos.y + getSize().y < platform->getPos().y + platform->getSize().y);
}

Platform* Entity::currentPlatform(std::vector<std::unique_ptr<Platform>>& platforms)
{
    pos.y += 20;
    for(std::unique_ptr<Platform>& platform : platforms){
        if(collides(platform.get())){
            pos.y -= 20;
            return platform.get();
        }

    }
    pos.y -= 20;
    return nullptr;
    
}

/**
 * @brief Can the entity, given a platform pointer jump to a platform? Takes into account the entity's jumpvelocity and movement speed. 
 * @param A pointer to the platform to jump to
 * @return A boolean indicating whether or not the entity can jump to the platform
 */

 bool Entity::contains(std::vector<FakePlatform> platforms, FakePlatform platform){
    for(FakePlatform p : platforms){
        if(p == platform){
            return true;
        }
    }
    return false;
}



void Entity::scrambleHelper(std::vector<FakePlatform> origin, std::vector<FakePlatform> vec, std::vector<std::vector<FakePlatform>>* storage){
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
std::vector<FakePlatform> Entity::mostEfficient(std::vector<std::vector<FakePlatform>> storage, std::vector<std::unique_ptr<Platform>>& platforms){
    //Storage is the scrambled arrays, platforms is the original game array set

    if(storage.size() == 0){
        std::cout << "Empty Storage" << std::endl;
        return std::vector<FakePlatform>();
    }

    for(std::vector<FakePlatform> vec : storage){
        //Adding these platforms back in here
        //We start at the entity, then we work out way over to the player
        Platform* current = currentPlatform(platforms);
        Platform* playerPlat = target->currentPlatform(platforms);

        vec.insert(vec.begin(), current->toFakePlatform());
        vec.push_back(playerPlat->toFakePlatform());

        if(vec.size() != 0){
            bool usable = true;
            //Note: the return of size() is an unsigned integer, meaning that 0 - 1 will loop around and be the max size (approx) 18446744073709551615, so running vec.size() - 1 without checking for 0 will massively increase run time
            if(vec.size() != 0){
                for(int i = 0; i < vec.size() - 1; i++){
                    if(!jumpable(vec.at(i), vec.at(i + 1))){ // Jumpable is the main logic for this function
                        usable = false;
                    }
                }
                if(usable){
                    return vec;
                }

            }
        }
    }
    std::vector<FakePlatform> a;
    // std::cout << "Returning Empty" << std::endl;
    return a;
}

/**
 * @brief This will be used to find the most effecient route to the player by looping through the return array, determining if each oute is possible, and calculating time for each route. Each route is represented by a vector of Platforms to be followed in order of looping. 
 * @param platforms The unique_ptr Platform array, holding all pf the games Platforms. 
 * @return A vector of vectors for every order of the platforms. 
 */
std::vector<std::vector<FakePlatform>> Entity::scramble(std::vector<std::unique_ptr<Platform>>& platforms){
    std::vector<FakePlatform> origin;
    FakePlatform targetCurent = target->currentPlatform(platforms)->toFakePlatform();
    FakePlatform entityCurrent = currentPlatform(platforms)->toFakePlatform();

    for(std::unique_ptr<Platform>& platform : platforms){
        FakePlatform fp = FakePlatform(platform->getPos(), platform->getSize(), platform->getID());
        //These will be force added in to the first and last position back in mostEfficient. 
        //We don't need combinations of the array with these platforms not in these positions because they must be the starting and ending positions
        if(fp != targetCurent && fp != entityCurrent){ 
            origin.push_back(fp);
        }
    }
    std::vector<FakePlatform> vec;
    std::vector<std::vector<FakePlatform>> storage;

    scrambleHelper(origin, vec, &storage);

    return storage;
}

bool Entity::getJumpInput()
{
    return jumpInput;
}

void Entity::setJumpInput(bool input)
{
    jumpInput = input;
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