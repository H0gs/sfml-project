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

    // Textures textures;
    Player player;
    std::vector<std::unique_ptr<Platform>> platforms;

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

    player.setTexture("textures/player.png");
    player.setPos(sf::Vector2f(600, 0));
    

    sf::RenderWindow window(sf::VideoMode(1200, 800), "SFML TUTORIAL");
    HealthBar healthBar = HealthBar();

    std::cout << "A" << std::endl;
    // window.SetFramerateLimit(60):

    // Platform platform2;
    std::unique_ptr<Platform> platform2 = std::make_unique<Platform>(); //Must explicitly call the constructor because the unique_pointer will not automatically do it
    std::cout << "A2" << std::endl;
    platform2->setTexture("textures/brick.png");
    std::cout << "A3" << std::endl;
    platform2->setPos(sf::Vector2f(50, 600));
    std::cout << "A4" << std::endl;
    platform2->setSize(sf::Vector2f(64, 64));
    std::cout << "A5" << std::endl;

    platforms.push_back(std::move(platform2));

    std::cout << "B" << std::endl;

    std::unique_ptr<Platform> ground = std::make_unique<Platform>();
    ground->setTexture(SINGLECOLOR);
    ground->setPos(sf::Vector2f(-100, 770));
    ground->setSize(sf::Vector2f(1400, 50));

    std::cout << "C" << std::endl;

    platforms.push_back(std::move(ground));

    std::unique_ptr<Acid> acid = std::make_unique<Acid>();;
    acid->setPos(sf::Vector2f(20, 20));
    platforms.push_back(std::move(acid));

    std::cout << "D" << std::endl;
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

        // std::cout << collides(platform1, player);

        window.clear();
        // std::cout << "X" << std::endl;
        sf::Vector2f relativePlayerPosition = player.movementUpdate(platforms);
        // std::cout << "Y" << std::endl;
        
        
        // player.damage();
        healthBar.update(player);


        window.draw(player.getSprite());
        window.draw(text);
        
        sf::View view = window.getView();
        view.move(relativePlayerPosition.x, 0);

        // view.rotate(1); --> Funny

        window.setView(view);

        healthBar.move(sf::Vector2f(relativePlayerPosition.x, 0));
        window.draw(healthBar.getSprite());

        for(std::unique_ptr<Platform> &platform : platforms){
            window.draw(platform->getSprite());
        }
        window.display();
        std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000/FPS));
    }

    return 0;
}