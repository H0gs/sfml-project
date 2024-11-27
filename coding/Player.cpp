#include <iostream>
#include <SFML/Graphics.hpp>
#include "Platform.cpp"

class Player{
    private:
        // Constants
        double acceleration = 2;
        double jumpVelocity = 15;
        double maxXVelocity = 10;
        double maxYVelocity = 20;
        double decelerationMultiplier = 0.7;
        double minVelocity = 0.1;
        double collisionElasticity = 0; //A double from 0-1 which is multiplied by the velocity determining how fast the player is moving after colliding into something, such as a platform. 
        double gravity = 1; //Gravitational acceleration
        double health = 100;

        // Other stuff
        sf::Texture texture;
        sf::Vector2f pos = sf::Vector2f(0, 0);
        sf::Sprite sprite;
        bool right = false;
        bool left = false;
        bool up = false;
        bool down = false;
        bool jumping = false;
        bool canJump = false;
        double xVelocity = 0;
        double yVelocity = 0;

    public:
        void setX(double xPos){
            pos.x = xPos;
        }
        void setY(double yPos){
            pos.y = yPos;
        }
        void setPos(sf::Vector2f newPos){
            pos = newPos;
            sprite.setPosition(pos);
        }
        void move(sf::Vector2f movement){
            pos.x += movement.x;
            pos.y += movement.y;
            sprite.setPosition(pos);
        }
        void setTexture(std::string path){
            texture.loadFromFile(path);
            sprite.setTexture(texture);
            sprite.setScale(sf::Vector2f(4, 4));
            // std::cout << sprite.getLocalBounds().height;
        }
        void setUp(bool set){
            up = set;
        }
        void setDown(bool set){
            down = set;
        }
        void setRight(bool set){
            right = set;
        }
        void setLeft(bool set){
            left = set;
        }
        void setJumping(bool set){
            jumping = set;
        }
        void damage(){
            health -= 0.5;
        }
        
        /**
         * @brief Updates the players position based on its x and y velocities. 
         * @param platforms A std::vector<Platform> representing the total platform objects in the game to be used for collisions. 
         * @return A sf::vector2f representing the player's new x and y, relative to its old x and y positions. 
         */
        sf::Vector2f movementUpdate(std::vector<Platform> platforms){
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

            //X collisions
            pos.x += xVelocity;
            bool collidedX = false;
            for(Platform platform : platforms){
                if(collides(platform)){
                    collidedX = true;
                }
            }
            if(collidedX){
                pos.x -= xVelocity;
            }
            //Y collisions
            pos.y += yVelocity;
            bool collidedY = false;
            for(Platform platform : platforms){
                if(collides(platform)){
                    collidedY = true;
                    if(collidesBottom(platform)){
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

            //Handling collisions with platforms section
            // for(Platform platform : platforms){
            //     //Note: we are using nested if-else statements here in order to modify pos so that we can test it within the if statement
            //     if(collides(platform)){
            //         //Check x
            //         pos.x -= xVelocity;
            //         if(!collides(platform)){
            //             // std::cout << "Horizontal collision!" << std::endl;
            //             //Collided horizontally
            //             xVelocity *= -collisionElasticity;
                        
            //         }else{
            //             pos.x += xVelocity; //Undos so that we can accurately check a different position

            //             pos.y -= yVelocity;
            //             if(!collides(platform)){
            //                 // std::cout << "Vertical collision!" << std::endl;
            //                 //Collided vertically

            //                 yVelocity *= -collisionElasticity;
            //             }else{
            //                 std::cout << "Corner!" << std::endl;
            //                 pos.y += yVelocity; //Undos so that it doesnt interfere with anything
            //                 //Somehow hit a corner somewhere and both x and y have collided
            //                 // while(collides(platform)){
            //                 //     pos.y -= yVelocity;
            //                 //     pos.x -= xVelocity;
            //                 // }
            //                 xVelocity *= -collisionElasticity;
            //                 yVelocity *= -collisionElasticity;
            //             }
            //         }

            //     }
            // }
        }
        sf::Sprite getSprite(){
            return sprite;
        }
        sf::Vector2f getPos(){
            return pos;
        }
        sf::Vector2f getSize(){
            return sf::Vector2f(sprite.getLocalBounds().width * sprite.getScale().x, sprite.getLocalBounds().height * sprite.getScale().y);
        }
        double getHeight(){
            return sprite.getLocalBounds().height * sprite.getScale().y;
        }
        double getWidth(){
            return sprite.getLocalBounds().width * sprite.getScale().x;
        }

        bool collides(Platform platform, Player player){
            return 
                ((player.getPos().x > platform.getPos().x && player.getPos().x < platform.getPos().x + platform.getSize().x) || (player.getPos().x + player.getSize().x > platform.getPos().x && player.getPos().x + player.getSize().x < platform.getPos().x + platform.getSize().x))
            &&
                ((player.getPos().y > platform.getPos().y && player.getPos().y < platform.getPos().y + platform.getSize().y) || (player.getPos().y + player.getSize().y > platform.getPos().y && player.getPos().y + player.getSize().y < platform.getPos().y + platform.getSize().y));
        }

        bool collides(Platform platform){
            return 
                ((pos.x >= platform.getPos().x && pos.x <= platform.getPos().x + platform.getSize().x) || (pos.x + getSize().x >= platform.getPos().x && pos.x + getSize().x <= platform.getPos().x + platform.getSize().x))
            &&
                ((pos.y >= platform.getPos().y && pos.y <= platform.getPos().y + platform.getSize().y) || (pos.y + getSize().y >= platform.getPos().y && pos.y + getSize().y <= platform.getPos().y + platform.getSize().y));
        }
        bool collidesBottom(Platform platform){ //This is pretty much only used for jumping
            return 
                ((pos.x > platform.getPos().x && pos.x < platform.getPos().x + platform.getSize().x) || (pos.x + getSize().x > platform.getPos().x && pos.x + getSize().x < platform.getPos().x + platform.getSize().x))
            &&
                (pos.y + getSize().y > platform.getPos().y && pos.y + getSize().y < platform.getPos().y + platform.getSize().y);
        }

        bool collides(std::vector<Platform> platforms){
            for(Platform platform : platforms){
                if(collides(platform, *this)){
                    return true;
                }
            }
            return false;
        }
        double getHealth(){
            return health;
        }
};