#include "Animation.h"
#include <iostream>
#include <chrono>

/**
 * @brief Returns a pointer to the current texture in the animation, and sets newTexture to false.  
 * @return A sf::RenderTexture*
 */
sf::RenderTexture* Animation::getTexture(){
    newTexture = false; //We just acessed the texture so the texture is no longer new.
    return texture;
}

/**
 * @brief Indicates whether or not the texture of the sprite needs to be updated. 
 */
bool Animation::hasNewTexture(){
    return newTexture;
}

/**
 * @brief Switches the sf::RenderTexture to show the next frame in the texture sheet if enough time has passed. 
 */
void Animation::updateTexture(){
    long long timeSinceLastCall; //The time from the last time this function has been caled to now. 

    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    // std::cout << "Time: " << " | " << duration.count() << std::endl;
    timeSinceLastCall = duration.count() - lastTime;
    timeCounter += timeSinceLastCall;
    // std::cout << "test2" << std::endl;
    if(timeCounter >= frameTime){
        // timeCounter = 0;
        timeCounter -= frameTime; //Exactly the time per frame might not have passed, it may have been longer, so we should account for that time and do this rather than resetting to 0. 
        currentFrame++;
        if(currentFrame >= frameCount){
            currentFrame = 0;
        }

        texture->clear();
        sf::Sprite temp(*textureSheet);
        temp.setTextureRect(sf::IntRect(currentFrame * size.x, textureNumber * size.y, size.x, size.y));
        temp.setScale(1, -1); //SFML RenderTextures flip on the y-axis for some reason
        temp.setPosition(0, size.y); //Acount for the flip
        texture->draw(temp);
    }
    lastTime = duration.count();
}

Animation::Animation(sf::Texture *textureSheet, sf::Vector2u size, int textureNumber, long long frameTime){
    this->textureSheet = textureSheet;
    this->size = size;
    this->frameTime = frameTime;
    this->textureNumber = textureNumber;
    newTexture = false;

    frameCount = textureSheet->getSize().x / size.x;
    currentFrame = 0;

    timeCounter = 0;
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    lastTime = duration.count(); //Set the time it was the last time this loop ran to the time it has been since the epoch. (January 1, 1970)

    texture = new sf::RenderTexture();
    texture->create(size.x, size.y);

    sf::Sprite temp(*textureSheet);
    temp.setTextureRect(sf::IntRect(0, textureNumber * size.y, size.x, size.y));
    temp.setScale(1, -1); //SFML RenderTextures flip on the y-axis for some reason
    temp.setPosition(0, size.y); //Adjust because we have to flip
    texture->draw(temp);
}