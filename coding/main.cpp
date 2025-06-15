#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <chrono>
#include <ctime>
#include <thread>
#include <vector>
#include "TestClass.cpp"
#include "Platform.h"
#include "Player.h"
#include "Textures.h"
#include "HealthBar.h"
#include "Acid.h"
#include "Animation.h"
#include "Goblin.h"
#include "Entity.h"
#include "FakePlatform.h"
// makefile is just the name of the makefile file, which has been named makefile
// mingw32-make -f makefile
// main.exe


bool collides(Platform platform, Player player){
        return 
        ((player.getPos().x > platform.getPos().x && player.getPos().x < platform.getPos().x + platform.getSize().x) || (player.getPos().x + player.getSize().x > platform.getPos().x && player.getPos().x + player.getSize().x < platform.getPos().x + platform.getSize().x))
        &&
        ((player.getPos().y > platform.getPos().y && player.getPos().y < platform.getPos().y + platform.getSize().y) || (player.getPos().y + player.getSize().y > platform.getPos().y && player.getPos().y + player.getSize().y < platform.getPos().y + platform.getSize().y));
}

int main()
{

    Textures textures;
    Player player;
    std::vector<std::unique_ptr<Platform>> platforms;
    std::vector<std::unique_ptr<Entity>> entities;

    sf::VertexArray line0(sf::Lines, 2); // Define the two points of the line 
    line0[0].position = sf::Vector2f(100, 100); // Starting point 
    line0[0].color = sf::Color::Red; // Color of the starting point 
    line0[1].position = sf::Vector2f(700, 500); // Ending point 
    line0[1].color = sf::Color::Blue; //Ending point color

    sf::Texture BRICKTEXTURE;
    if(BRICKTEXTURE.loadFromFile("textures/testingSubRects.png")){
        std::cout << "Loaded properly" << std::endl;
    }else{
        std::cout << "Loading failed!" << std::endl;
    }
    sf::Texture SINGLECOLOR;
    if(SINGLECOLOR.loadFromFile("textures/singleColor.png")){
        std::cout << "Loaded properly" << std::endl;
    }else{
        std::cout << "Loading failed!" << std::endl;
    }

    sf::Texture ANIMATION_TEST;
    if(ANIMATION_TEST.loadFromFile("textures/Animation_Test.png")){
        std::cout << "Loaded properly" << std::endl;
    }else{
        std::cout << "Loading failed!" << std::endl;
    }

    Animation animation(&ANIMATION_TEST, sf::Vector2u(16, 16), 0, 500);
    sf::Sprite animationTestSprite;

    Goblin goblin = Goblin(&player);
    std::unique_ptr<Entity> uniqueGoblin(&goblin);
    entities.push_back(std::move(uniqueGoblin));

    animationTestSprite.setPosition(400, 400);
    animationTestSprite.setScale(4, 4);
    animationTestSprite.setTexture(animation.getTexture()->getTexture());

    player.setTexture("textures/player.png");
    player.setPos(sf::Vector2f(600, 0));
    

    sf::RenderWindow window(sf::VideoMode(1200, 800), "SFML TUTORIAL");
    HealthBar healthBar = HealthBar();

    sf::Sprite damageOutline;
    sf::Texture damageOutlineTexture;
    if(damageOutlineTexture.loadFromFile("textures/damage_overlay.png")){
        std::cout << "Loaded properly" << std::endl;
    }else{
        std::cout << "Loading failed!" << std::endl;
    }
    damageOutline.setTexture(damageOutlineTexture);
    damageOutline.setScale(1200 / damageOutline.getGlobalBounds().width, 800 / damageOutline.getGlobalBounds().height);

    // window.SetFramerateLimit(60):

    // Platform platform2;
    std::unique_ptr<Platform> platform2 = std::make_unique<Platform>(); //Must explicitly call the constructor because the unique_pointer will not automatically do it
    platform2->setTexture("textures/brick.png");
    platform2->setPos(sf::Vector2f(550, 600));
    platform2->setSize(sf::Vector2f(64, 64));
    platform2->setID(2);

    platforms.push_back(std::move(platform2));

    std::unique_ptr<Platform> platform3 = std::make_unique<Platform>(); //Must explicitly call the constructor because the unique_pointer will not automatically do it
    platform3->setTexture("textures/brick.png");
    platform3->setPos(sf::Vector2f(1600, 600));
    platform3->setSize(sf::Vector2f(128, 64));
    platform3->setID(3);

    platforms.push_back(std::move(platform3));



    std::unique_ptr<Platform> ground = std::make_unique<Platform>();
    ground->setTexture(SINGLECOLOR);
    ground->setPos(sf::Vector2f(-100, 770));
    ground->setSize(sf::Vector2f(2400, 50));


    platforms.push_back(std::move(ground));

    std::unique_ptr<Acid> acid = std::make_unique<Acid>();;
    acid->setPos(sf::Vector2f(20, 20));
    acid->setID(27);
    platforms.push_back(std::move(acid));

    std::unique_ptr<Platform> platform4 = std::make_unique<Platform>(); //Must explicitly call the constructor because the unique_pointer will not automatically do it
    platform4->setTexture("textures/brick.png");
    platform4->setPos(sf::Vector2f(400, 100));
    platform4->setSize(sf::Vector2f(64, 64));

    platforms.push_back(std::move(platform4));

    std::unique_ptr<Platform> platform5 = std::make_unique<Platform>(); //Must explicitly call the constructor because the unique_pointer will not automatically do it
    platform5->setTexture("textures/brick.png");
    platform5->setPos(sf::Vector2f(700, 400));
    platform5->setSize(sf::Vector2f(64, 64));
    platform5->setID(5);

    platforms.push_back(std::move(platform5));

    std::unique_ptr<Platform> platform6 = std::make_unique<Platform>(); //Must explicitly call the constructor because the unique_pointer will not automatically do it
    platform6->setTexture("textures/brick.png");
    platform6->setPos(sf::Vector2f(400, 450));
    platform6->setSize(sf::Vector2f(64, 64));
    platform6->setID(6);

    platforms.push_back(std::move(platform6));

    std::unique_ptr<Platform> platform7 = std::make_unique<Platform>(); //Must explicitly call the constructor because the unique_pointer will not automatically do it
    platform7->setTexture("textures/brick.png");
    platform7->setPos(sf::Vector2f(980, 550));
    platform7->setSize(sf::Vector2f(64, 64));
    platform7->setID(7);

    platforms.push_back(std::move(platform7));

    // platform1.setTexture(newTexture);



    //System constants
    int FPS = 60;

    //System variables
    std::time_t time = 0;
    int frameCount = 0;

    //Player variables
    int x = 0;
    int y = 0;

    sf::Font font;
    if(!font.loadFromFile("ka1.ttf")){
        //Error
        std::cout << "ERROR" << std::endl;
    }else{
        std::cout << "Loaded!" << std::endl;
    }
    sf::Text text;
    text.setFont(font);
    text.setString("This is a much longer test string");
    text.setFillColor(sf::Color::Red);
    text.setCharacterSize(40);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        

        player.setLeft(sf::Keyboard::isKeyPressed(sf::Keyboard::A));
        player.setRight(sf::Keyboard::isKeyPressed(sf::Keyboard::D));
        player.setUp(sf::Keyboard::isKeyPressed(sf::Keyboard::W));
        player.setDown(sf::Keyboard::isKeyPressed(sf::Keyboard::S));
        player.setJumping(sf::Keyboard::isKeyPressed(sf::Keyboard::Space));
        for(std::unique_ptr<Entity>& e : entities){
            e->setJumpInput(sf::Keyboard::isKeyPressed(sf::Keyboard::P));
            
        }
        
        if(time != std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())){
            frameCount++;
            // std::cout << "Frames: " << frameCount << std::endl;
            //If the time it was on the last iteration of the loop is not equal to the time it is now, meaning exactlly one second since the last success of this if-statement. Note: this only works as long as we get at least 1 frame per second
            time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            frameCount = 0;
        }else{
            frameCount++;
        }
        // 1B / 1487 = time in nanoseconds per frame
        // (1B / 60) - time per frame


        window.clear();
        sf::Vector2f relativePlayerPosition = player.movementUpdate(platforms);
        
        
        // player.damage();
        healthBar.update(player);


        window.draw(player.getSprite());

        
        sf::View view = window.getView();
        view.move(relativePlayerPosition.x, 0);

        // view.rotate(1); //--> Funny

        window.setView(view);
        

        for(std::unique_ptr<Platform> &platform : platforms){
            window.draw(platform->getSprite());
        }
        for(std::unique_ptr<Entity>& entity : entities){
            window.draw(entity->getSprite());
        }

        //Player jump testing
        for(std::unique_ptr<Platform> &platform : platforms){
            sf::VertexArray line(sf::Lines, 2); // Define the two points of the line 
            line[0].position = sf::Vector2f(platform.get()->getPos().x, platform.get()->getPos().y); // Starting point
            line[1].position = sf::Vector2f(player.getPos().x, player.getPos().y); // Ending point
            if(player.jumpableHelper(platform.get())){
                line[0].color = sf::Color::Green;
                line[1].color = sf::Color::Green;
            }else{
                line[0].color = sf::Color::Red;
                line[1].color = sf::Color::Red;
            }
            // window.draw(line);
        }

        animation.updateTexture();
        window.draw(animationTestSprite);
        // window.draw(line0);
        window.draw(text);

        damageOutline.setPosition(damageOutline.getGlobalBounds().left + relativePlayerPosition.x, 0);
        if(player.getDamageFrameCount() > 0){
            window.draw(damageOutline);
        }

        healthBar.move(sf::Vector2f(relativePlayerPosition.x, 0));
        window.draw(healthBar.getSprite());
        

        //Pathfinding line testing
        for(std::unique_ptr<Entity>& entity : entities){
            entity->update(platforms);
            window.draw(entity.get()->getSprite());
            if(entity->getPath().size() != 0){
                // std::cout << "Path" << std::endl;
                for(int i = 0; i < entity->getPath().size() - 1; i++){
                    sf::VertexArray line(sf::Lines, 2); // Define the two points of the line 
                    line[0].position = sf::Vector2f(entity->getPath().at(i).getPos().x, entity->getPath().at(i).getPos().y); // Starting point
                    line[1].position = sf::Vector2f(entity->getPath().at(i + 1).getPos().x, entity->getPath().at(i + 1).getPos().y); // Ending point
                    line[0].color = sf::Color::White;
                    window.draw(line);
                }
            }else{
                // std::cout << "Empty" << std::endl;
            }
            if(entity->getMovementNodes().size() != 0){
                for(int i = 0; i < entity->getMovementNodes().size() - 1; i++){
                    sf::VertexArray line(sf::Lines, 2); // Define the two points of the line 
                    line[0].position = entity->getMovementNodes().at(i); // Starting point
                    line[1].position = entity->getMovementNodes().at(i + 1); // Ending point
                    line[0].color = sf::Color::Magenta;
                    window.draw(line);
                }
            }
        }
        // std::cout << player.getPos().x << ", " << player.getPos().y << std::endl;
        // std::cout << "Jumpable: " << entities.at(0).get()->jumpableHelper(platforms.at(1).get()->toFakePlatform(), sf::Vector2f(439, 385)) << std::endl;
        // std::cout << "Jumpable: " << entities.at(0).get()->jumpable(platforms.at(1).get()->toFakePlatform(), platforms.at(0).get()->toFakePlatform()) << std::endl;
        // std::cout << platforms.at(1).get()->getPos().x << ", " << platforms.at(0).get()->getPos().x << std::endl;
        // std::cout << "Jumpable: " << entities.at(0).get()->jumpable(platforms.at(3).get()->toFakePlatform(), platforms.at(0).get()->toFakePlatform()) << std::endl;

        //Goblin jump testing
        for(std::unique_ptr<Platform> &platform : platforms){
            sf::VertexArray line(sf::Lines, 2); // Define the two points of the line 
            line[0].position = sf::Vector2f(platform.get()->getPos().x, platform.get()->getPos().y); // Starting point
            line[1].position = sf::Vector2f(goblin.getPos().x, goblin.getPos().y); // Ending point
            if(goblin.jumpableHelper(platform.get()->toFakePlatform(), goblin.getPos(), platforms)){
                line[0].color = sf::Color::Green;
                line[1].color = sf::Color::Green;
            }else{
                line[0].color = sf::Color::Red;
                line[1].color = sf::Color::Red;
            }
            // window.draw(line);
        }

        // window.draw(goblin.getSprite());

        

        window.display();
        std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000/FPS));
    }

    return 0;
}