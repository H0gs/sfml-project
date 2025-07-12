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

            // std::vector<std::vector<FakePlatform>> temp = scramble(platforms);

            auto end1 = std::chrono::high_resolution_clock::now();
            

            // path = mostEfficient(temp, platforms);
            path = mostEfficient3(platforms);
            auto end2 = std::chrono::high_resolution_clock::now();
            auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - end1);

            movementNodes = calculateNodes();
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
                if(std::abs(movementNodes.at(0).x - pos.x) <= maxXVelocity  && std::abs(movementNodes.at(0).y - (pos.y + getSize().y)) <= maxXVelocity){ //Use 0.01 to account for double truncation and weird number stuff
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
    if(movementNodes.size() > 0){
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
    double xPos2 = start.getPos().x + end.getWidth();


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
                // std::cout << "Id: " << fp.getID() << std::endl;
                return false;
            }
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
std::vector<sf::Vector2f> Entity::calculateNodes(){
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
                if(b.getPos().x < a.getPos().x){
                    //B is to the left of A
                    nodeA.x = a.getPos().x;
                    nodeA.y = a.getPos().y;
                    nodeB.x = b.getPos().x + b.getWidth();
                    nodeB.y = b.getPos().y;
                }else{
                    //B is to the right of A
                    nodeA.x = a.getPos().x + a.getWidth();
                    nodeA.y = a.getPos().y;
                    nodeB.x = b.getPos().x;
                    nodeB.y = b.getPos().y;
                }
            }
            //Check before adding to prevent duplicates
            nodes.push_back(nodeA);
            nodes.push_back(nodeB);

        }//End for loop
    }//If statement

    //Add code to clean up nodes here
    return nodes;
}

//Assumes currentPlatform is not null \/

/**
 * @brief [CURRENTLY UNUSED] Finds a jumpable series of fakeplatforms to get to the player
 * @param storage A vector containing vectors, each sub-vector representing a possible path in the form of fakeplatforms, this is created by scramble
 * @param platforms A reference to the unique pointer Platform array created in main
 * @return A vector<FakePlatform> representing the most efficient path to get to the player. Start of the array is at the entity, ends at the player
 */
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

        if(current->toFakePlatform() != playerPlat->toFakePlatform()){
            vec.insert(vec.begin(), current->toFakePlatform());
            vec.push_back(playerPlat->toFakePlatform());
        }else{
            vec.insert(vec.begin(), current->toFakePlatform());
        }

        

        if(vec.size() != 0){
            bool usable = true;
            //Note: the return of size() is an unsigned integer, meaning that 0 - 1 will loop around and be the max size (approx) 18446744073709551615, so running vec.size() - 1 without checking for 0 will massively increase run time
            if(vec.size() != 0){
                for(int i = 0; i < vec.size() - 1; i++){
                    if(!jumpable(vec.at(i), vec.at(i + 1), platforms)){ // Jumpable is the main logic for this function
                        usable = false;
                    }
                }
                if(usable){
                    for(FakePlatform fp : vec){
                        std::cout << fp.getID() << " ";
                    }
                    std::cout << "\n";
                    return vec;
                }

            }
        }
    }
    std::vector<FakePlatform> a;
    // std::cout << "Returning Empty" << std::endl;
    return a;
}

//[CURRENTLY UNUSED] - mostEfficient3 is more efficient
std::vector<FakePlatform> Entity::mostEfficient2(std::vector<std::unique_ptr<Platform>> &platforms)
{
    
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
        fakePlatforms.push_back(platform.get()->toFakePlatform());
    }
    
    fakePlatforms.erase(std::remove(fakePlatforms.begin(), fakePlatforms.end(), playerPlat), fakePlatforms.end());
    fakePlatforms.erase(std::remove(fakePlatforms.begin(), fakePlatforms.end(), entityPlat), fakePlatforms.end());

    std::vector<std::vector<int>> nums;
    int n = fakePlatforms.size();

    for (int mask = 0; mask < (1 << n); ++mask) {
        std::vector<int> vec;
        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i)) {
                vec.push_back(i);
            }
        }

        //vec
        do {
            nums.push_back(vec);
        } while (std::next_permutation(vec.begin(), vec.end()));
    }
    // std::cout << "Nums: " << nums.size() << std::endl;

    fakePlatforms.insert(fakePlatforms.begin(), entityPlat);
    fakePlatforms.push_back(playerPlat);


    int numCount = 0;

    std::unordered_set<std::array<int, 2>> blacklist; // checks IDs of FakePlatforms

    for(std::vector<int>& vec : nums){
        numCount++;
        vec.insert(vec.begin(), -1); //Everything will be shifted up by one, so -1 will be 0 so that it points to the front of the vector
        vec.push_back(fakePlatforms.size() - 2);

        bool usable = true;
        //Note: the return of size() is an unsigned integer, meaning that 0 - 1 will loop around and be the max size (approx) 18446744073709551615, so running vec.size() - 1 without checking for 0 will massively increase run time
        if(vec.size() != 0){
            for(int i = 0; i < vec.size() - 1; i++){
                std::array<int, 2> arr = {vec.at(i), vec.at(i + 1)};
                if(blacklist.find(arr) != blacklist.end()){ //If blacklist contains arr
                    usable = false;
                    break;
                }else{
                    if(!jumpable(fakePlatforms.at(vec.at(i) + 1), fakePlatforms.at(vec.at(i + 1) + 1), platforms)){ // Jumpable is the main logic for this function
                        usable = false;
                        blacklist.insert(arr);
                        // for()
                    }
                }
                
            }
            if(usable){
                std::vector<FakePlatform> finalVec;
                for(int n : vec){
                    // std::cout << fakePlatforms.at(n + 1).getID() << " ";
                    finalVec.push_back(fakePlatforms.at(n + 1));
                }
                // std::cout << std::endl;
                // std::cout << "NumCount: " << numCount << std::endl;
                jumpMap[currentJump] = finalVec;
                // std::cout << "Calculation Added To jumpMap!" << std::endl;
                return finalVec;
            }

        }

    }

    jumpMap[currentJump] = std::vector<FakePlatform>();
    return std::vector<FakePlatform>();
}

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
                    for(FakePlatform fp : tempVec){
                        std::cout << fp.getID() << " ";
                    }
                    std::cout << std::endl;
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