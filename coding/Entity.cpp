#include "Entity.h"

//test

std::unordered_map<FakePlatform, std::vector<FakePlatform>> Entity::jumpablePlatforms;
bool Entity::updateJumpablePlatforms = true;

Entity::Entity(Player* player)
{
    target = player;
}

std::vector<FakePlatform> Entity::getPath(){
    return path;
}

std::vector<sf::Vector2f> Entity::getMovementNodes(){
    return movementNodes;
}

sf::Vector2f Entity::update(std::vector<std::unique_ptr<Platform>>& platforms)
{
    framesSincePathRefresh++;
    //Temporary
    // std::cout << "Canjump: " << canJump << std::endl;

    //Velocity section
    yVelocity += gravity;

    pos.x += xVelocity + appliedXVelocity;
    pos.y += yVelocity + appliedYVelocity;
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
        yVelocity = maxYVelocity;
    }else if(yVelocity < -maxYVelocity){
        yVelocity = -maxYVelocity;
    }

    if(framesSincePathRefresh > PATH_REFRESH_FRAMES){
        if(currentPlatform(platforms) != nullptr && target->currentPlatform(platforms) != nullptr){
            framesSincePathRefresh = 0;
            // std::cout << "Generating path" << std::endl;

            // std::vector<std::vector<FakePlatform>> temp = scramble(platforms);

            auto end1 = std::chrono::high_resolution_clock::now();
            

            // path = mostEfficient(temp, platforms);
            path = mostEfficient3(platforms);
            auto end2 = std::chrono::high_resolution_clock::now();
            auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - end1);

            movementNodes = calculateNodes(platforms);
            auto end3 = std::chrono::high_resolution_clock::now();
            auto duration3 = std::chrono::duration_cast<std::chrono::milliseconds>(end3 - end2);

            if(duration2.count() > 0){
                std::cout << duration2.count() << " ms" << std::endl;
            }
        }else{
            // std::cout << "Unable to generate path" << std::endl;
        }
    }


    //Compare current position to the first node in the movmentNodes vector

    //This makes sure that the first node is not the same as the current entity position, the first node in movementNodes should be the target node. 
    if(movementNodes.size() > 0){
        bool repeat = true;
        while(repeat){
            if(movementNodes.size() > 0){
                if(std::abs(movementNodes.at(0).x - pos.x) <= maxXVelocity  && std::abs(movementNodes.at(0).y - (pos.y + getSize().y)) <= maxYVelocity){ //If the position of the entity is roughly at the position of the node, this might cause issues later though because its not very precise
                    pos.x = movementNodes.at(0).x;
                    pos.y = movementNodes.at(0).y - getSize().y - 1;

                //if(std::abs(movementNodes.at(0).x - pos.x) < 0.01 && std::abs(movementNodes.at(0).y - (pos.y + getSize().y)) < 0.01){ //Use 0.01 to account for double truncation and weird number stuff
                    movementNodes.erase(movementNodes.begin());
                    // std::cout << "Removed!" << std::endl;
                    xVelocity = 0;
                }else{
                    repeat = false;
                    // std::cout << "(" << std::abs(movementNodes.at(0).x - pos.x) << ", " << std::abs(movementNodes.at(0).y - (pos.y + getSize().y)) << ")" << std::endl;
                }
            }else{
                repeat = false;
            }
        }

        

    }
    //Need to recheck to see if it has not completed
    if(movementNodes.size() > 0 && currentPlatform(platforms) != nullptr){
        FakePlatform current = currentPlatform(platforms)->toFakePlatform();
        sf::Vector2f temp = pos;
        pos = movementNodes.at(0);
        FakePlatform endPlatform = currentPlatform(platforms)->toFakePlatform();
        pos = temp;


        if(endPlatform == current){ //If we are moving in a straight line horizontally
            if(movementNodes.at(0).x > pos.x){
                xVelocity = maxXVelocity;
            }else{
                xVelocity = -maxXVelocity;
            }
        }
        else{
            //We still have a few nodes to go until we reach the end platform
            if(currentPlatform(platforms) != nullptr){
                // std::cout << fallable(pos, movementNodes.at(0)) << std::endl;
                // std::cout << "(" << pos.x << ", " << pos.y << "), (" << movementNodes.at(0).x << ", " << movementNodes.at(0).y << ")" << std::endl;
                if(!fallable(pos, movementNodes.at(0))){
                    yVelocity = -jumpVelocity;
                }else{
                    //We are still on the platform, move to one side until we fall off, then adjust midair in the next code section
                    if(movementNodes.at(0).x > pos.x){
                        xVelocity = maxXVelocity;
                    }else{
                        xVelocity = -maxXVelocity;
                    }
                }
            }
        }
    }else if (movementNodes.size() > 0){
        //If we are midair, move to the platform
        // std::cout << "Working - " << fallable(pos, movementNodes.at(0)) << std::endl;
        // std::cout << "Jumpable: " << 
        // std::cout << "Falling Frames: " << fallingFrames(pos, movementNodes.at(0)) << std::endl;
        if(fallingFrames(pos, movementNodes.at(0), yVelocity) > 0 && fallable(pos, movementNodes.at(0))){
            double xDist;

            double xDist1 = std::abs(pos.x - movementNodes.at(0).x);
            double xDist2 = std::abs(pos.x + getSize().x - movementNodes.at(0).x);
            if(xDist1 < xDist2){
                xDist = xDist1;
            }else{
                xDist = xDist2;
            }
            int direction = 1;
            if(!(pos.x < getMovementNodes().at(0).x)){
                direction = -1;
            }

            xVelocity = direction * xDist / fallingFrames(pos, movementNodes.at(0), yVelocity);
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

bool Entity::jumpable(FakePlatform start, FakePlatform end, std::vector<std::unique_ptr<Platform>>& platforms){

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
    double xPos2 = start.getPos().x + start.getWidth();

    if(jumpableHelper(end, sf::Vector2f(xPos1, start.getPos().y - getSize().y), platforms)){ //This one (still)
        return true;
    }
    
    if(jumpableHelper(end, sf::Vector2f(xPos2, start.getPos().y - getSize().y), platforms)){
        return true;
    }

    if(end.getPos().y < start.getPos().y){ //If the end is higher up than the start
        double startX1 = start.getPos().x;
        double startX2 = startX1 + start.getWidth();

        double endX1 = end.getPos().x;
        double endX2 = endX1 + end.getWidth();

        
        if(endX1 > startX1 && endX1 < startX2){//If the first edge of end is over start
            if(jumpableHelper(end, sf::Vector2f(endX1 - getSize().x, start.getPos().y - getSize().y), platforms)){
                return true;
            }
        }
        
        

        if(endX2 > startX1 && endX2 < startX2){//If the second edge of end is over start
            if(jumpableHelper(end, sf::Vector2f(endX2, start.getPos().y - getSize().y), platforms)){
                return true;
            }
        }
    }
    

    return false;
}

bool Entity::jumpableHelper(FakePlatform platform, sf::Vector2f p, std::vector<std::unique_ptr<Platform>>& platforms){
    if(
        (p.x + getSize().x > platform.getPos().x && p.x < platform.getPos().x + platform.getWidth()) && //At least some part of the player/entity is between the ends of the platform
        (p.y < platform.getPos().y)
    ){
        // std::cout << "Top check" << std::endl;
        return true;
    }

    std::vector<FakePlatform> fakePlatforms; //Will represent the platforms in between the player and the entity
    for(std::unique_ptr<Platform>& plat : platforms){ //This can be optimized later to only account for platforms in between the entity and the platform
         fakePlatforms.push_back(plat->toFakePlatform());
    }

    int direction = 1;

    //Starting position: p
    if(p.x > platform.getPos().x){
        //Starting pos is to the right of platform, moving left
        direction = -1;
    }


    double yPositiontracker = 0;
    double xPositiontracker = 0;
    double yVelocityTracker = jumpVelocity;

    double yDistance = p.y - platform.getPos().y + getSize().y;
    double apexDist = yPositiontracker;

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

    sf::Vector2f posActual;
    posActual.x = p.x;
    posActual.y = p.y;

    while(yPositiontracker >= yDistance || yVelocityTracker > 0){
        yVelocityTracker -= gravity;
        yPositiontracker += yVelocityTracker;
        xPositiontracker += maxXVelocity;
        if(yPositiontracker > apexDist){
            apexDist = yPositiontracker;
        }
        posActual.x += direction * maxXVelocity;
        posActual.y -= yVelocityTracker; //Invert because top of the screen is 0 (?)
        for(FakePlatform fp : fakePlatforms){
            if(collides(fp, posActual) && fp != platform){                
                return xDistance < xPositiontracker && apexDist > yDistance;
            }
        }
    }

    


    return xDistance < xPositiontracker && 
    apexDist > yDistance;
            // (yPositiontracker - jumpVelocity < platform.getPos().y);
}

bool Entity::fallable(sf::Vector2f p, sf::Vector2f target){
    int count = 0;

    double yVelocityTracker = yVelocity;
    double yDist = target.y - (p.y + getSize().y);

    double xDist;

    double xDist1 = std::abs(p.x - target.x);
    double xDist2 = std::abs(p.x + getSize().x - target.x);
    if(xDist1 < xDist2){
        xDist = xDist1;
    }else{
        xDist = xDist2;
    }

    // double xDist = std::abs(p.x - target.x);


    double yPositionTracker = 0;
    double xPositionTracker = 0;

    while(yPositionTracker < yDist){
        // count++;
        // if(count > 1000){
        //     std::cout << "Fallable!!!" << std::endl;
        //     break;
        // }
        if(xPositionTracker > xDist){
            return true;
        }
        if(yVelocityTracker < maxYVelocity){
            yVelocityTracker += gravity;
        }
        if(yVelocityTracker > maxYVelocity){
            yVelocityTracker = maxYVelocity;
        }
        xPositionTracker += maxXVelocity;
        yPositionTracker += yVelocityTracker;
    }

    return xPositionTracker >= xDist;
}

int Entity::fallingFrames(sf::Vector2f p, sf::Vector2f target, double velo){
    int frames = 0;

    double yDistance = std::abs(target.y - p.y - getSize().y);

    double yVelocityTracker = velo;
    double yPositionTracker = p.y;
    while(yPositionTracker + getSize().y < target.y || yVelocityTracker < 0){
        frames++;
        if(frames > 1000){
            std::cout << "Frames!!!" << std::endl;
            break;
        }
        if(yVelocityTracker < maxYVelocity){
            yVelocityTracker += gravity;
        }
        if(yVelocityTracker > maxYVelocity){
            yVelocityTracker = maxYVelocity;
        }
        yPositionTracker += yVelocityTracker;
        // std::cout << "Loop velo: " << yVelocityTracker <<  ", Math: " << frames * gravity + yVelocity << std::endl;
        // std::cout << "Loop yPos: " << yPositionTracker <<  ", Math: " << 0.5 * frames * frames * gravity + yVelocity * frames + p.y << std::endl;


    }

    // std::cout << "Frames: " << frames << std::endl;
    // std::cout << std::round(std::sqrt((yVelocity * yVelocity) / (gravity * gravity) + 2 / gravity * (yDistance - 1)) - yVelocity / gravity + 0.5) << std::endl;
    // //This works as long as the entity velocity is less than its max velocity
    // double time = std::sqrt(-2 * (target.y - p.y - getSize().y) + (yVelocity * yVelocity)) - yVelocity;
    // std::cout << "Calculated Frames: " << time << std::endl;
    return frames;
}

bool Entity::jumpableWithSomeVelocity(FakePlatform start, FakePlatform end, std::vector<std::unique_ptr<Platform>> &platforms, double v, sf::Vector2f startingPos)
{
    double xPositionTracker = startingPos.x;
    double yPositionTracker = startingPos.y;
    double yVelocityTracker = -jumpVelocity;

    double yDistance = end.getPos().y - start.getPos().y;

    while(yPositionTracker + getSize().y < end.getPos().y || yVelocityTracker < 0){
        if(yVelocityTracker < maxYVelocity){
            yVelocityTracker += gravity;
        }
        if(yVelocityTracker > maxYVelocity){
            yVelocityTracker = maxYVelocity;
        }

        xPositionTracker += v;
        yPositionTracker += yVelocityTracker;

        bool collided = false;
        for(std::unique_ptr<Platform> &platform : platforms){
            if(collides(platform->toFakePlatform(), sf::Vector2f(xPositionTracker, yPositionTracker))){
                collided = true;
                if(platform->toFakePlatform() == end){
                    yPositionTracker -= yVelocityTracker;
                    if(collides(platform->toFakePlatform(), sf::Vector2f(xPositionTracker, yPositionTracker))){
                        return false; //We hit the platform from the side, we are too short of the platform and the velocity is not great enough
                    }else{
                        return true;
                    }
                }
            }
        }
    }

    if(currentPlatform(platforms) != nullptr){
        return currentPlatform(platforms)->toFakePlatform() == end;
    }

    return false;
}

bool Entity::collidesBottom(Platform* platform){ //This is pretty much only used for jumping
    return 
        ((pos.x > platform->getPos().x && pos.x < platform->getPos().x + platform->getSize().x) || (pos.x + getSize().x > platform->getPos().x && pos.x + getSize().x < platform->getPos().x + platform->getSize().x))
    &&
        (pos.y + getSize().y > platform->getPos().y && pos.y + getSize().y < platform->getPos().y + platform->getSize().y);
}

bool Entity::collides(FakePlatform platform, sf::Vector2f p){
    
    return         
        ((p.x >= platform.getPos().x && p.x <= platform.getPos().x + platform.getSize().x) || (p.x + getSize().x >= platform.getPos().x && p.x + getSize().x <= platform.getPos().x + platform.getSize().x))
            &&
        ((p.y >= platform.getPos().y && p.y <= platform.getPos().y + platform.getSize().y) || (p.y + getSize().y >= platform.getPos().y && p.y + getSize().y <= platform.getPos().y + platform.getSize().y));
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

//Starts at entity, ends at player
std::vector<sf::Vector2f> Entity::calculateNodes(std::vector<std::unique_ptr<Platform>>& platforms){
    std::vector<sf::Vector2f> nodes;
    if(path.size() == 1){
        sf::Vector2f node;
        node.x = target->getPos().x;
        node.y = path.at(0).getPos().y;
        nodes.push_back(node);
        return nodes;
    }
    if(path.size() > 0){
        for(int i = 0; i < path.size() - 1; i++){
            // std::cout << "Iteration" << std::endl;
            FakePlatform a = path.at(i);
            FakePlatform b = path.at(i + 1);

            sf::Vector2f nodeA;
            sf::Vector2f nodeB;

            if(a.getPos().y > b.getPos().y){ //a is below b
                if(b.getPos().x > a.getPos().x && b.getPos().x < a.getPos().x + a.getWidth()){ //This should be optimized to use the closer one (between the first and second block in this if statement)
                    //Left side of b is over a
                    nodeA.x = b.getPos().x - (getSize().x + 0.5); //The 0.5 is to account for any chance that it hits its head on a truncation-caused headhitter
                    nodeA.y = a.getPos().y;
                    nodeB.x = b.getPos().x;
                    nodeB.y = b.getPos().y;
                }else if(b.getPos().x + b.getWidth() > a.getPos().x && b.getPos().x + b.getWidth() < a.getPos().x + a.getWidth()){
                    //Right side of b is over a
                    nodeA.x = b.getPos().x + b.getWidth() + 0.5; //The 0.5 is to account for any chance that it hits its head on a truncation-caused headhitter
                    nodeA.y = a.getPos().y;
                    nodeB.x = b.getPos().x + b.getWidth();
                    nodeB.y = b.getPos().y;
                }else{
                    //B is off the side of a somewhere
                    if(b.getPos().x + b.getWidth() < a.getPos().x){
                        nodeA.x = a.getPos().x;
                        nodeA.y = a.getPos().y;
                        nodeB.x = b.getPos().x + b.getWidth();
                        nodeB.y = b.getPos().y;
                    }else{
                        nodeA.x = a.getPos().x + a.getWidth();
                        nodeA.y = a.getPos().y;
                        nodeB.x = b.getPos().x;
                        nodeB.y = b.getPos().y;
                    }
                }

                

            }else{ //a is above b
                //Note that B cannot be directly under A and smaller than A because of how most efficient works
                bool leftSideOver = a.getPos().x > b.getPos().x && a.getPos().x < b.getPos().x + b.getWidth();
                bool rightSideOver = a.getPos().x + a.getSize().x > b.getPos().x && a.getPos().x + a.getSize().x < b.getPos().x + b.getWidth();
                double playerCenter = target->getPos().x + (target->getSize().x) / 2;

                if(leftSideOver && rightSideOver){
                    if(std::abs(a.getPos().x - playerCenter) < std::abs(a.getPos().x + a.getSize().x - playerCenter)){
                        nodeA.x = a.getPos().x;
                        nodeA.y = a.getPos().y;
                        nodeB.x = a.getPos().x - getSize().x - 1;
                        nodeB.y = b.getPos().y;
                    }else{
                        nodeA.x = a.getPos().x + a.getSize().x;
                        nodeA.y = a.getPos().y;
                        nodeB.x = a.getPos().x + a.getSize().x + 1;
                        nodeB.y = b.getPos().y;
                    }
                }else if(leftSideOver){
                    nodeA.x = a.getPos().x;
                    nodeA.y = a.getPos().y;
                    nodeB.x = a.getPos().x - getSize().x - 1;
                    nodeB.y = b.getPos().y;
                }else if(rightSideOver){
                    nodeA.x = a.getPos().x + a.getSize().x;
                    nodeA.y = a.getPos().y;
                    nodeB.x = a.getPos().x + a.getSize().x + 1;
                    nodeB.y = b.getPos().y;
                }else{
                    bool advancedCalc = false;
                    if(b.getPos().x < a.getPos().x){
                        //B is to the left of A
                        // std::cout << "B to the Left" << std::endl;
                        if(i + 1 < path.size() - 1){
                            // std::cout << "Not Last Jump" << std::endl;                            
                            
                            if(path.at(i + 2).getPos().x + path.at(i + 2).getSize().x < b.getPos().x){
                                advancedCalc = true;
                            }
                        }else{
                            // std::cout << "Last Jump" << std::endl;
                            advancedCalc = true;
                        }
                        if(advancedCalc){
                            // std::cout << "Use Advanced Calc!" << std::endl;
                            //Here use the maximum possible velocity
                            double maxVelo = 0;
                            for(int i = 1; i <= 10; i++){
                                if(jumpableWithSomeVelocity(a, b, platforms, -maxXVelocity / 10 * i, a.getPos())){
                                    if(maxXVelocity / 10 * i > maxVelo){
                                        maxVelo = maxXVelocity / 10 * i;
                                    }
                                }
                            }
                            if(maxVelo != 0){
                                int frames = 0;
                                double xDist = 0;
                                // std::cout << "Advanced Calc Succeded - " << maxVelo << std::endl;
                                if(fallable(a.getPos(), sf::Vector2f(b.getPos().x + b.getWidth(), b.getPos().y))){
                                    xDist = maxVelo * fallingFrames(sf::Vector2f(a.getPos().x, a.getPos().y - getSize().y), sf::Vector2f(b.getPos().x + b.getWidth(), b.getPos().y), 0) - getSize().x;
                                }else{
                                    xDist = maxVelo * fallingFrames(sf::Vector2f(a.getPos().x, a.getPos().y - getSize().y), sf::Vector2f(b.getPos().x + b.getWidth(), b.getPos().y), -jumpVelocity) - getSize().x; // No clue why we need the -getSize().x but it seems to work
                                }
                                // std::cout << "XDist: " << xDist << std::endl;
                                nodeA.x = a.getPos().x;
                                nodeA.y = a.getPos().y;
                                nodeB.x = -xDist + a.getPos().x;
                                nodeB.y = b.getPos().y;
                            }else{
                                // Advanced Calc Failed
                                nodeA.x = a.getPos().x;
                                nodeA.y = a.getPos().y;
                                nodeB.x = b.getPos().x + b.getWidth();
                                nodeB.y = b.getPos().y;
                            }
                        }else{
                            nodeA.x = a.getPos().x;
                            nodeA.y = a.getPos().y;
                            nodeB.x = b.getPos().x + b.getWidth();
                            nodeB.y = b.getPos().y;
                        }
                        
                    }else{
                        //B is to the right of A

                        //Start -------------------------------------------------------------------------------------
                        if(i + 1 < path.size() - 1){
                            // Not Last Jump                            
                            if(b.getPos().x > path.at(i + 2).getSize().x + path.at(i + 2).getSize().x){
                                advancedCalc = true;
                            }
                        }else{
                            // Last Jump
                            advancedCalc = true;
                        }
                        if(advancedCalc){
                            //Here use the maximum possible velocity
                            double maxVelo = 0;
                            for(int i = 1; i <= 10; i++){
                                if(jumpableWithSomeVelocity(a, b, platforms, maxXVelocity / 10 * i, sf::Vector2f(a.getPos().x + a.getSize().x - getSize().x, a.getPos().y))){ //The -getSize().x is because the entity wont jump all the way at the end of the platform but instead once it reaches the node so it isnt really hanging over the edge
                                    if(maxXVelocity / 10 * i > maxVelo){
                                        maxVelo = maxXVelocity / 10 * i;
                                    }
                                }
                            }
                            if(maxVelo != 0){
                                int frames = 0;
                                double xDist = 0;
                                // std::cout << "Advanced Calc Succeded - " << maxVelo << std::endl;
                                if(fallable(sf::Vector2f(a.getPos().x + a.getSize().x, a.getPos().y), b.getPos())){
                                    xDist = maxVelo * fallingFrames(sf::Vector2f(a.getPos().x, a.getPos().y - getSize().y), sf::Vector2f(b.getPos().x + b.getWidth(), b.getPos().y), 0);
                                }else{
                                    xDist = maxVelo * fallingFrames(sf::Vector2f(a.getPos().x, a.getPos().y - getSize().y), sf::Vector2f(b.getPos().x + b.getWidth(), b.getPos().y), -jumpVelocity); // No clue why we need the -getSize().x but it seems to work
                                }
                                // std::cout << "XDist: " << xDist << std::endl;
                                nodeA.x = a.getPos().x + a.getWidth();
                                nodeA.y = a.getPos().y;
                                if(xDist + a.getPos().x > b.getPos().x + b.getSize().x){
                                    nodeB.x = b.getPos().x + b.getSize().x;
                                }else{
                                    nodeB.x = xDist + a.getPos().x + a.getSize().x;
                                }                                
                                nodeB.y = b.getPos().y;
                            }else{
                                // Advanced Calc Failed - 1
                                nodeA.x = a.getPos().x + a.getWidth();
                                nodeA.y = a.getPos().y;
                                nodeB.x = b.getPos().x;
                                nodeB.y = b.getPos().y;
                            }
                        }else{
                            // Advanced Calc Failed - 2
                            nodeA.x = a.getPos().x + a.getWidth();
                            nodeA.y = a.getPos().y;
                            nodeB.x = b.getPos().x;
                            nodeB.y = b.getPos().y;
                        }

                        //End ---------------------------------------------------------------------------------------
                        
                    }
                }
                
            }
            //Check before adding to prevent duplicates
            nodes.push_back(nodeA);
            nodes.push_back(nodeB);

        }//End for loop
    }//If statement

    //Add code to clean up nodes here
    // std::cout << "-----------------------------------------------------" << std::endl;
    return nodes;
}

//Assumes currentPlatform is not null \/


/**
 * @brief Finds a jumpable series of fakeplatforms to get to the player
 * @param platforms A reference to the unique pointer Platform array created in main
 * @return A vector<FakePlatform> representing the most efficient path to get to the player. Start of the array is at the entity, ends at the player
 */
std::vector<FakePlatform> Entity::mostEfficient3(std::vector<std::unique_ptr<Platform>>& platforms){
    FakePlatform playerPlat = target->currentPlatform(platforms)->toFakePlatform();
    FakePlatform entityPlat = currentPlatform(platforms)->toFakePlatform();

    std::vector<FakePlatform> fakePlatforms;

    if(entityPlat == playerPlat){
        fakePlatforms.push_back(entityPlat);
        return fakePlatforms;
    }

    std::array<FakePlatform, 2> currentJump = {playerPlat, entityPlat};
    if(jumpMap.find(currentJump) != jumpMap.end()){ //If this jump has already been calculated
        // std::cout << "Contains" << std::endl;
        return jumpMap[currentJump];
    }else{
        // std::cout << "Calculation required" << std::endl;
    }

    for(std::unique_ptr<Platform> &platform : platforms){
        fakePlatforms.push_back(platform->toFakePlatform());
    }

    if(updateJumpablePlatforms){
        updateJumpablePlatforms = false;
        for(FakePlatform p1 : fakePlatforms){
            for(FakePlatform p2 : fakePlatforms){
                if(p1 != p2){
                    if(jumpable(p1, p2, platforms)){
                        if(jumpablePlatforms.find(p1) == jumpablePlatforms.end()){ //If p1 isnt in the dictionary yet
                            std::vector<FakePlatform> vec = {p2};
                            jumpablePlatforms[p1] = vec;
                        }else{
                            jumpablePlatforms[p1].push_back(p2);
                        }
                    }
                }
            }
        }
    }

    std::vector<FakePlatform> finalPath;
    std::unordered_set<FakePlatform> set;
    set.insert(entityPlat);
    std::vector<FakePlatform> vec;
    vec.push_back(entityPlat);
    int count = fakePlatforms.size() + 1;
    mostEfficient3_Helper(entityPlat, playerPlat, set, vec, &finalPath, &count);


    jumpMap[currentJump] = finalPath;
    return finalPath;
}

//Recursively check jumpablePlatforms until we find the end platform
void Entity::mostEfficient3_Helper(FakePlatform start, FakePlatform end, std::unordered_set<FakePlatform> alreadyDone, std::vector<FakePlatform> currentPath, std::vector<FakePlatform> *storage, int *count)
{
    if(jumpablePlatforms.find(start) != jumpablePlatforms.end()){ //JumpablePlatforms contains start
        for(FakePlatform platform : jumpablePlatforms[start]){
            if(alreadyDone.find(platform) == alreadyDone.end()){ //We haven't done this patform yet
                std::vector<FakePlatform> tempVec = currentPath;
                std::unordered_set<FakePlatform> tempSet = alreadyDone;
                tempVec.push_back(platform);
                if(platform == end && tempVec.size() < *count){
                    *storage = tempVec;
                    *count = tempVec.size();
                    // for(FakePlatform fp : tempVec){
                    //     std::cout << fp.getID() << " ";
                    // }
                    // std::cout << std::endl;
                }else{
                    tempSet.insert(platform);
                    mostEfficient3_Helper(platform, end, tempSet, tempVec, storage, count);
                }
            }
        }
    }
}

void Entity::scrambleHelper(std::vector<FakePlatform> origin, std::vector<FakePlatform> vec, std::vector<std::vector<FakePlatform>>* storage){
    auto start = std::chrono::high_resolution_clock::now();

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
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start);
    if(duration1.count() > 0){
        // std::cout << "Time: " << duration1.count() << " ms" << std::endl;
    }
}

void Entity::scrambleHelper2(std::vector<FakePlatform> origin, std::unordered_map<FakePlatform, int> map, std::vector<std::unordered_map<FakePlatform, int>>* storage){
    // auto start = std::chrono::high_resolution_clock::now();

    storage->push_back(map);
    if(map.size() != origin.size()){
        for(FakePlatform p : origin){
            if(map.find(p) == map.end()){ //Element not in map yet
                std::unordered_map<FakePlatform, int> temp = map;
                temp[p] = map.size();
                scrambleHelper2(origin, temp, storage);
            }
        }
    }
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
        FakePlatform fp = platform->toFakePlatform();
        //These will be force added in to the first and last position back in mostEfficient. 
        //We don't need combinations of the array with these platforms not in these positions because they must be the starting and ending positions
        if(fp != targetCurent && fp != entityCurrent){ 
            origin.push_back(fp);
        }
    }
    std::vector<FakePlatform> vec;
    std::vector<std::vector<FakePlatform>> storage;


    scrambleHelper(origin, vec, &storage);
    // scrambleHelper2(origin, vec, set, &storage); --> 50% slower

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