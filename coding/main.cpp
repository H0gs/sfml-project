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
    platform2->setPos(sf::Vector2f(50, 600));
    platform2->setSize(sf::Vector2f(64, 64));

    platforms.push_back(std::move(platform2));

    std::unique_ptr<Platform> platform3 = std::make_unique<Platform>(); //Must explicitly call the constructor because the unique_pointer will not automatically do it
    platform3->setTexture("textures/brick.png");
    platform3->setPos(sf::Vector2f(1600, 600));
    platform3->setSize(sf::Vector2f(128, 64));

    platforms.push_back(std::move(platform3));


    std::unique_ptr<Platform> ground = std::make_unique<Platform>();
    ground->setTexture(SINGLECOLOR);
    ground->setPos(sf::Vector2f(-100, 770));
    ground->setSize(sf::Vector2f(2400, 50));


    platforms.push_back(std::move(ground));

    std::unique_ptr<Acid> acid = std::make_unique<Acid>();;
    acid->setPos(sf::Vector2f(20, 20));
    platforms.push_back(std::move(acid));

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

        // view.rotate(1); --> Funny

        window.setView(view);        
        

        for(std::unique_ptr<Platform> &platform : platforms){
            window.draw(platform->getSprite());
        }

        for(std::unique_ptr<Platform> &platform : platforms){
            sf::VertexArray line(sf::Lines, 2); // Define the two points of the line 
            line[0].position = sf::Vector2f(platform.get()->getPos().x, platform.get()->getPos().y); // Starting point
            line[1].position = sf::Vector2f(player.getPos().x, player.getPos().y); // Ending point
            if(player.jumpable(platform.get())){
                line[0].color = sf::Color::Green;
                line[1].color = sf::Color::Green;
            }else{
                line[0].color = sf::Color::Red;
                line[1].color = sf::Color::Red;
            }
            window.draw(line);
        }

        animation.updateTexture();
        window.draw(animationTestSprite);
        window.draw(line0);
        window.draw(text);

        damageOutline.setPosition(damageOutline.getGlobalBounds().left + relativePlayerPosition.x, 0);
        if(player.getDamageFrameCount() > 0){
            window.draw(damageOutline);
        }

        healthBar.move(sf::Vector2f(relativePlayerPosition.x, 0));
        window.draw(healthBar.getSprite());

        window.draw(goblin.getSprite());

        

        window.display();
        std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000/FPS));
    }

    return 0;
}