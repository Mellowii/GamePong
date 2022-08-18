#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>

int main()
{
    srand(time(NULL));
    //Define some constants 
    const int screenWidth = 800;
    const int screenHeight = 500;
    const sf::Vector2f paddleSize(25, 100);
    const float ballRadius = 10.f;
    const float pi = .14159f;

    // Create the window of the application 
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Pong!", sf::Style::Titlebar | sf::Style::Close);

    //Create the left paddle - tao thanh ben trai
    sf::RectangleShape leftPaddle; //ve hinh
    leftPaddle.setSize(paddleSize); //kich thuoc
    leftPaddle.setFillColor(sf::Color::Blue); //to mau
    leftPaddle.setOutlineThickness(3); //vien
    leftPaddle.setOutlineColor(sf::Color(0,0,0)); //to vien
    leftPaddle.setPosition(0, screenHeight / 2.f); //vi tri thanh
    leftPaddle.setOrigin(paddleSize / 2.f);

    //Create the right paddle - tao thanh ben phai
    sf::RectangleShape rightPaddle;
    rightPaddle.setSize(paddleSize);
    rightPaddle.setFillColor(sf::Color::Red);
    rightPaddle.setOutlineThickness(3);
    rightPaddle.setOutlineColor(sf::Color(0, 0, 0));
    rightPaddle.setPosition(screenWidth, screenHeight / 2.f);
    rightPaddle.setOrigin(paddleSize / 2.f);

    //Create the ball - tao qua bong
    sf::CircleShape ball(100.f); 
    ball.setRadius(ballRadius);
    ball.setFillColor(sf::Color::Yellow); 
    ball.setOutlineThickness(3); 
    ball.setOutlineColor(sf::Color::Black);
    ball.setPosition(screenWidth/2, screenHeight/2);

    //Bot properties //tao bot (thanh trai)
    int botTarget = screenHeight / 2;
    float botPreriod = 0.4f; //chu ky bong di chuyen
    float botTimer = 0.f; //thoi gian giua cac chu ky

    //Define paddle properties - xac dinh thuoc tinh cua thanh
    float paddleSpeed = 400.f; //van toc thanh

    //Define the ball properties
    const float ballSpeed = 400.f; //van toc bong
    float angleBall = 0.f; //to change

    //Load font
    sf::Font font;
    if (!font.loadFromFile("../Data/ARCADE.TTF")) return EXIT_FAILURE;

    //Init pause Messenger
    sf::Text pauseMessenger;
    pauseMessenger.setFont(font);
    pauseMessenger.setPosition(screenWidth / 2-215, screenHeight / 2-50);
    pauseMessenger.setFillColor(sf::Color::Red);
    pauseMessenger.setCharacterSize(50);
    pauseMessenger.setString("Press space to play");

    //Timer
    sf::Clock clock;
    float deltaTime = 0.f;

    //Game state
    bool isPlaying = false;

    //Sound
    sf::SoundBuffer ballSFX;
    ballSFX.loadFromFile("../Data/mixkit-ball-bouncing-in-the-ground-2077.wav");
    sf::Sound sfx;
    sfx.setBuffer(ballSFX);

    while (window.isOpen()) //kiem tra dieu kien cua so con mo hay khong
    {
        deltaTime = clock.restart().asSeconds();

        sf::Event event; //khoi tao 
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) //su kien bam nut x
                window.close(); //dong cua so
        }
        if (isPlaying == false && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            ball.setPosition(screenWidth / 2, screenHeight / 2);
            leftPaddle.setPosition(0, screenHeight / 2.f);
            rightPaddle.setPosition(screenWidth, screenHeight / 2.f);
            do {
                angleBall = rand() % 360;
            } while (angleBall<45 || angleBall>135 && angleBall<225 || angleBall>315);
            
            isPlaying = true;
        }
        if(isPlaying)
        {
            //the leftPaddle moving
            //Update bot
            botTimer += botPreriod; //Dem thoi gian
            if (botTimer >= botPreriod) //thoi gian > chu ky
            {
                if (sin(angleBall * pi / 180) > 0.f) //neu qua bong di chuyen ve ben phai
                {
                    botTarget = ball.getPosition().y; //update bot target
                    botTimer -= botPreriod; //dem lai tu dau
                }
            }
            if (leftPaddle.getPosition().y + paddleSize.y / 4 < botTarget) //(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                sf::Vector2f pos = leftPaddle.getPosition();
                if (pos.y + paddleSize.y / 2 + paddleSpeed * deltaTime > screenHeight) leftPaddle.setPosition(pos.x, screenHeight - paddleSize.y / 2);
                else leftPaddle.move(0, paddleSpeed * deltaTime);
            }
            else if (leftPaddle.getPosition().y - paddleSize.y / 4 > botTarget) //(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                sf::Vector2f pos = leftPaddle.getPosition();
                if (pos.y - paddleSize.y / 2 - paddleSpeed * deltaTime < 0) leftPaddle.setPosition(pos.x, paddleSize.y / 2);
                else leftPaddle.move(0, -paddleSpeed * deltaTime);
            }

            //the rightPaddle moving
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) //(rightPaddle.getPosition().y + paddleSize.y / 4 < botTarget)
            {
                sf::Vector2f pos = rightPaddle.getPosition();
                if (pos.y + paddleSize.y / 2 + paddleSpeed * deltaTime > screenHeight) rightPaddle.setPosition(pos.x, screenHeight - paddleSize.y / 2);
                else rightPaddle.move(0, paddleSpeed * deltaTime);
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) //(rightPaddle.getPosition().y - paddleSize.y / 4 > botTarget)
            {
                sf::Vector2f pos = rightPaddle.getPosition();
                if (pos.y - paddleSize.y / 2 - paddleSpeed * deltaTime < 0) rightPaddle.setPosition(pos.x, paddleSize.y / 2);
                else rightPaddle.move(0, -paddleSpeed * deltaTime);
            }

            //The ball moving 
            float factor = ballSpeed * deltaTime;
            //ball.move(cos(angleBall * pi / 180) * factor, sin(angleBall * pi / 180) * factor);
            ball.move(sin(angleBall * pi / 180) * factor, -cos(angleBall * pi / 180) * factor);

            //Check collision top_down wall
            if (ball.getPosition().y <= 0 || ball.getPosition().y >= screenHeight)
            {
                angleBall = 180.f + angleBall;
                sfx.play(); 
            }

            //Check collison with the paddle
            //Left
            if (ball.getPosition().x - ballRadius <= leftPaddle.getPosition().x + paddleSize.x / 2
                && ball.getPosition().y >= leftPaddle.getPosition().y - paddleSize.y / 2
                && ball.getPosition().y <= leftPaddle.getPosition().y + paddleSize.y / 2
                && ball.getPosition().x > leftPaddle.getPosition().x)
            {
                if (ball.getPosition().y > leftPaddle.getPosition().y)
                {
                    angleBall = 360.f - angleBall + rand() % 10;
                }
                else
                {
                    angleBall = 360.f - angleBall - rand() % 10;
                }
                sfx.play();
            }

            //Right
            if (ball.getPosition().x + ballRadius >= rightPaddle.getPosition().x - paddleSize.x / 2
                && ball.getPosition().y >= rightPaddle.getPosition().y - paddleSize.y / 2
                && ball.getPosition().y <= rightPaddle.getPosition().y + paddleSize.y / 2
                && ball.getPosition().x < rightPaddle.getPosition().x)
            {
                if (ball.getPosition().y > rightPaddle.getPosition().y)
                {
                    angleBall = 360.f - angleBall - rand() % 10;
                }
                else
                {
                    angleBall = 360.f - angleBall + rand() % 10;
                }
                sfx.play();
            }
            if (ball.getPosition().x <= 0 )
            {
                pauseMessenger.setString("Red is Winner\nPress space to play");
                isPlaying = false;
            }
            else if(ball.getPosition().x >= screenWidth)
            {
                pauseMessenger.setString("Blue is Winner\nPress space to play");
                isPlaying = false;
            }
        }

        window.clear(sf::Color::White);

        if (isPlaying)
        {
            window.draw(leftPaddle);
            window.draw(rightPaddle);
            window.draw(ball);
        }
        else
        {
            window.draw(pauseMessenger);
        }
        
        window.display();
    }

    return 0;
}