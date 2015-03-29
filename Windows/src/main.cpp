#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "Player.h"
#include "Target.h"
#include <vector>
#include <string>
#include <sstream>
#include "LineGraph.h"

//Shoot red targets with red and blues with blue
//shoot with triggers
//random graphs on HUD
int randint(int low, int high);
int randint(int low, int high, int extraSeed);
bool circleColision(Vector2f circlePos, Vector2f pointPos, int radius);
Vector2f drawNumbers(RenderWindow* window, int num, Vector2f position, Texture* texture, Color textColor);
Vector2f getDrawSize(int num, Texture* texture);
void drawGameOver(RenderWindow* window, int windowWidth, int windowHeight, Texture* Ytexture, Texture* Xtexture, int player1Score, int player2Score, Texture* bigNums);

const Color BLACK(0, 0, 0);
const Color RED(215, 0, 0);
const Color DARKRED(67, 25, 25);
const Color BLUE(0, 255, 255);
const Color DARKBLUE(25, 25, 67);
const Color GREY(100, 100, 100);
const Color DARKGREY(25, 25, 25);

using namespace sf;

int main()
{
    // Create the main window
    int windowWidth = 1920;
    int windowHeight = 1080;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML window");

    Texture bigNumbersTexture;
    if (!bigNumbersTexture.loadFromFile("numbersBig.png"))
        return EXIT_FAILURE;

    Texture smallNumbersTexture;
    if (!smallNumbersTexture.loadFromFile("numbersSmall.png"))
        return EXIT_FAILURE;

    Texture Ytexture;
    if (!Ytexture.loadFromFile("Y.png"))
        return EXIT_FAILURE;

    Texture Xtexture;
    if (!Xtexture.loadFromFile("X.png"))
        return EXIT_FAILURE;

    window.setVerticalSyncEnabled(true);

    Player player1(5, windowWidth, windowHeight, RED, DARKRED);
    Player player2(5, windowWidth, windowHeight, BLUE, DARKBLUE);

    int frame = 0;
    int countDownStart = frame;
    int respawnTime = 300;
    int lostLifeAt = -1;

    float difficulty = 1;

    int oldButtonState[Joystick::getButtonCount(0)];

    int gameState = 0; //0: countdown, 1: game, 2: game over

    Target target1(Vector2f(1000, 100), 90, DARKRED, Color(0, 0, 0), RED);
    Target target2(Vector2f(800, 500), 90, DARKBLUE, Color(0, 0, 0), BLUE);

    //Graph objects and stat variables

    LineGraph graphLeftX(Vector2f(10, windowHeight / 2 - 130), DARKRED, RED, -100, 100, &smallNumbersTexture); //graphs X position of the left analog stick
    LineGraph graphLeftY(Vector2f(10, windowHeight / 2 - 255), DARKRED, RED, -100, 100, &smallNumbersTexture);
    LineGraph graphRightX(Vector2f(windowWidth - 223, windowHeight / 2 - 130), DARKBLUE, BLUE, -100, 100, &smallNumbersTexture);
    LineGraph graphRightY(Vector2f(windowWidth - 223, windowHeight / 2 - 255), DARKBLUE, BLUE, -100, 100, &smallNumbersTexture);

    LineGraph graphLeftAccX(Vector2f(10, windowHeight / 2 + 5), DARKRED, RED, -25, 25, &smallNumbersTexture);
    LineGraph graphLeftAccY(Vector2f(10, windowHeight / 2 + 130), DARKRED, RED, -25, 25, &smallNumbersTexture);
    LineGraph graphRightAccX(Vector2f(windowWidth - 223, windowHeight / 2 + 5), DARKBLUE, BLUE, -25, 25, &smallNumbersTexture);
    LineGraph graphRightAccY(Vector2f(windowWidth - 223, windowHeight / 2 + 130), DARKBLUE, BLUE, -25, 25, &smallNumbersTexture);

    float oldX = Joystick::getAxisPosition(0, Joystick::X);
    float oldY = Joystick::getAxisPosition(0, Joystick::Y);
    float oldU = Joystick::getAxisPosition(0, Joystick::U);
    float oldR = Joystick::getAxisPosition(0, Joystick::R);

    View defaultView = window.getView();

	// Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Escape)
                {
                    window.close();
                }
            }

            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                windowWidth = event.size.width;
                windowHeight = event.size.height;

                //update graph position
                graphLeftX.setPosition(Vector2f(10, windowHeight / 2 - 130));
                graphLeftY.setPosition(Vector2f(10, windowHeight / 2 - 255));
                graphRightX.setPosition(Vector2f(windowWidth - 223, windowHeight / 2 - 130));
                graphRightY.setPosition(Vector2f(windowWidth - 223, windowHeight / 2 - 255));

                graphLeftAccX.setPosition(Vector2f(10, windowHeight / 2 + 5));
                graphLeftAccY.setPosition(Vector2f(10, windowHeight / 2 + 130));
                graphRightAccX.setPosition(Vector2f(windowWidth - 223, windowHeight / 2 + 5));
                graphRightAccY.setPosition(Vector2f(windowWidth - 223, windowHeight / 2 + 130));

                window.setView(sf::View(visibleArea));
            }
        }
        if (gameState == 0)
        {
            if (frame - countDownStart > respawnTime)
            {
                gameState = 1;
            }
        }

        if (gameState == 1)
        {
            float circleSize = windowHeight / 200.0 - (windowHeight / 2000.0);
            player1.setPosition((windowWidth / 2) + Joystick::getAxisPosition(0, Joystick::X) * circleSize, (windowHeight / 2) + Joystick::getAxisPosition(0, Joystick::Y) * circleSize);
            player2.setPosition((windowWidth / 2) + Joystick::getAxisPosition(0, Joystick::U) * circleSize, (windowHeight / 2) + Joystick::getAxisPosition(0, Joystick::R) * circleSize);

            player1.update(target1.getPosition(), target1.getRadius());
            player2.update(target2.getPosition(), target2.getRadius());
            if (target1.update(difficulty) == 1)
            {
                if (target1.reset(windowWidth, windowHeight) == 1)
                {
                    player1.subtractLives(1);
                    lostLifeAt = frame;
                }
            }
            if (target2.update(difficulty) == 1)
            {
                if (target2.reset(windowWidth, windowHeight) == 1)
                {
                    player2.subtractLives(1);
                    lostLifeAt = frame;
                }
            }

            if (player1.getSize() > 5)
            {
                target1.setRadius(target1.getRadius() - 1);
            }
            if (player2.getSize() > 5)
            {
                target2.setRadius(target2.getRadius() - 1);
            }

            if (player1.getSize() > 98)
                target1.setHit(true);
            if (player2.getSize() > 98)
                target2.setHit(true);

            if (player1.isDead() || player2.isDead())
            {
                gameState = 2;
                player1.setColors(GREY, DARKGREY);
                player2.setColors(GREY, DARKGREY);
                player1.update(target1.getPosition(), target1.getRadius());
                player2.update(target2.getPosition(), target2.getRadius());
            }

            //change difficulty
            std::cout << difficulty << "\n";
            int score = player1.getPoints() + player2.getPoints();
            if (score >= 0 && score < 1200)
                difficulty = 1;
            else if (score >= 1200 && score < 2700)
                difficulty = 1.5;
            else if (score >= 2700 && score < 5400)
                difficulty = 2.0;
            else if (score >= 5400)
                difficulty = 3.0;
        }

        //This is indifferent to gamestates because the shaking and resetting has to continue in the gameover state
        if (lostLifeAt > -1)
        {
            View view(FloatRect(0, 0, windowWidth, windowHeight));
            view.setCenter(Vector2f((windowWidth / 2) + (frame % 5) * 3, windowHeight / 2));
            window.setView(view);
            if (frame - lostLifeAt > 40)
            {
                lostLifeAt = -1;
                window.setView(View(FloatRect(0, 0, windowWidth, windowHeight)));
            }
        }

        if (gameState == 2)
        {
            if (target1.update(difficulty) == 1)
            {
                target1.reset(windowWidth, windowHeight);
            }
            if (target2.update(difficulty) == 1)
            {
                target2.reset(windowWidth, windowHeight);
            }

            if (Joystick::isButtonPressed(0, 3))
            {
                player1 = Player(5, windowWidth, windowHeight, RED, DARKRED);
                player2 = Player(5, windowWidth, windowHeight, BLUE, DARKBLUE);

                target1 = Target(Vector2f(1000, 100), 90, DARKRED, Color(0, 0, 0), RED);
                target2 = Target(Vector2f(800, 500), 90, DARKBLUE, Color(0, 0, 0), BLUE);

                countDownStart = frame;
                difficulty = 1;
                respawnTime = 60;
                gameState = 0;
            }
            if (Joystick::isButtonPressed(0, 2))
            {
                window.close();
            }
        }

        if (frame % 3 == 0)
        {
            graphLeftX.update(-Joystick::getAxisPosition(0, Joystick::X));
            graphLeftY.update(-Joystick::getAxisPosition(0, Joystick::Y));
            graphRightX.update(-Joystick::getAxisPosition(0, Joystick::U));
            graphRightY.update(-Joystick::getAxisPosition(0, Joystick::R));

            graphLeftAccX.update(oldX - Joystick::getAxisPosition(0, Joystick::X));
            graphLeftAccY.update(oldY - Joystick::getAxisPosition(0, Joystick::Y));
            graphRightAccY.update(oldU - Joystick::getAxisPosition(0, Joystick::U));
            graphRightAccX.update(oldR - Joystick::getAxisPosition(0, Joystick::R));
        }

        window.clear();

        if (gameState == 0)
        {
            int num = respawnTime == 300 ? 300 - (frame - countDownStart) : 60 - (frame - countDownStart);

            float circleSize = windowHeight / 200.0 - (windowHeight / 2000.0);
            Vector2f redCoords((windowWidth / 2) + Joystick::getAxisPosition(0, Joystick::X) * circleSize - 21, (windowHeight / 2) + Joystick::getAxisPosition(0, Joystick::Y) * circleSize);
            Vector2f bluCoords((windowWidth / 2) + Joystick::getAxisPosition(0, Joystick::U) * circleSize, (windowHeight / 2) + Joystick::getAxisPosition(0, Joystick::R) * circleSize);

            drawNumbers(&window, num, redCoords, &smallNumbersTexture, RED);
            drawNumbers(&window, num, bluCoords, &smallNumbersTexture, BLUE);
            graphLeftX.draw(&window);
            graphLeftY.draw(&window);
            graphRightX.draw(&window);
            graphRightY.draw(&window);
            graphLeftAccX.draw(&window);
            graphLeftAccY.draw(&window);
            graphRightAccX.draw(&window);
            graphRightAccY.draw(&window);
        }
        if (gameState == 1)
        {
            target1.draw(&window);
            target2.draw(&window);
            player1.draw(&window, &smallNumbersTexture);
            player2.draw(&window, &smallNumbersTexture);
            graphLeftX.draw(&window);
            graphLeftY.draw(&window);
            graphRightX.draw(&window);
            graphRightY.draw(&window);
            graphLeftAccX.draw(&window);
            graphLeftAccY.draw(&window);
            graphRightAccX.draw(&window);
            graphRightAccY.draw(&window);
        }


        if (gameState == 2)
        {
            target1.draw(&window);
            target2.draw(&window);
            player1.draw(&window, &smallNumbersTexture);
            player2.draw(&window, &smallNumbersTexture);
            graphLeftX.draw(&window);
            graphLeftY.draw(&window);
            graphRightX.draw(&window);
            graphRightY.draw(&window);
            graphLeftAccX.draw(&window);
            graphLeftAccY.draw(&window);
            graphRightAccX.draw(&window);
            graphRightAccY.draw(&window);

            drawGameOver(&window, windowWidth, windowHeight, &Ytexture, &Xtexture, player1.getPoints(), player2.getPoints(), &bigNumbersTexture);
        }


        // Update the window
        window.display();

        for (int i = 0; i < Joystick::getButtonCount(0); i++)
        {
            //std::cout << i << ": " << Joystick::isButtonPressed(0, i) << ", ";
            if (Joystick::isButtonPressed(0, i))
            {
                oldButtonState[i] = 1;
            }
            else
            {
                oldButtonState[i] = 0;
            }
        }

        oldX = Joystick::getAxisPosition(0, Joystick::X);
        oldY = Joystick::getAxisPosition(0, Joystick::Y);
        oldU = Joystick::getAxisPosition(0, Joystick::U);
        oldR = Joystick::getAxisPosition(0, Joystick::R );

        frame++;
    }

    return EXIT_SUCCESS;
}

int randint(int low, int high)
{
    int value = rand() % (high + 1 - low) + low;
    srand(value * clock());
    return value;
    /*std::default_random_engine generator;
    generator.seed(clock());
    std::uniform_int_distribution<int> distribution(low, high);*/
}

int randint(int low, int high, int extraSeed)
{
    int value = rand() % (high + 1 - low) + low;
    srand(value * clock() * extraSeed);
    return value;
}

bool circleColision(Vector2f circlePos, Vector2f pointPos, int radius)
{
    int o = abs(pointPos.y - circlePos.y);
    int a = abs(pointPos.x - circlePos.x);
    int s = sqrt(o * o + a * a);

    if (s <= radius)
        return true;
    else
        return false;
}

Vector2f drawNumbers(RenderWindow* window, int num, Vector2f position, Texture* texture, Color textColor) //returns total size of the drawn sprites
{
    int bigOrSmall; //small: 0, big: 1
    Vector2f spriteSize;

    if (texture->getSize().y == 7)
    {
        bigOrSmall = 0;
        spriteSize.y = 7;
    }

    else if (texture->getSize().y == 14)
    {
        bigOrSmall = 1;
        spriteSize.y = 14;
    }

    std::string numStr;
    std::stringstream convert;
    convert << num;
    numStr = convert.str();

    std::vector<int> numList;

    for (int i = 0; i < numStr.length(); i++)
    {
        if (numStr[i] != '-')
            numList.push_back((int)(numStr[i]) - 48);
        else
            numList.push_back(10);
    }

    for (int i = 0; i < numStr.length(); i++)
    {
        Sprite number(*texture);
        if (bigOrSmall == 0)
        {
            number.setTextureRect(IntRect(numList[i] * 5, 0, 5, 7));
            number.setPosition(position + Vector2f(i * 6, 0));
            number.setColor(textColor);
            window->draw(number);
            spriteSize.x += 6;
        }
        else if (bigOrSmall == 1)
        {
            number.setTextureRect(IntRect(numList[i] * 10, 0, 10, 14));
            number.setPosition(position + Vector2f(i * 11, 0));
            number.setColor(textColor);
            window->draw(number);
            spriteSize.x += 10;
        }
    }

    return spriteSize;
}

Vector2f getDrawSize(int num, Texture* texture) //remove redundancy
{
    int bigOrSmall; //small: 0, big: 1
    Vector2f spriteSize;

    if (texture->getSize().y == 7)
    {
        bigOrSmall = 0;
        spriteSize.y = 7;
    }

    else if (texture->getSize().y == 14)
    {
        bigOrSmall = 1;
        spriteSize.y = 14;
    }

    std::string numStr;
    std::stringstream convert;
    convert << num;
    numStr = convert.str();

    for (int i = 0; i < numStr.length(); i++)
    {
        if (bigOrSmall == 0)
        {
            spriteSize.x += 6;
        }
        else if (bigOrSmall == 1)
        {
            spriteSize.x += 10;
        }
    }

    return spriteSize;
}

void drawGameOver(RenderWindow* window, int windowWidth, int windowHeight, Texture* Ytexture, Texture* Xtexture, int player1Score, int player2Score, Texture* bigNums)
{
    Sprite Ybutton(*Ytexture);
    Sprite Xbutton(*Xtexture);

    Ybutton.setPosition(Vector2f(windowWidth / 2 - 150, windowHeight / 2 - 50));
    Xbutton.setPosition(Vector2f(windowWidth / 2 + 50, windowHeight / 2 - 50));

    window->draw(Ybutton);
    window->draw(Xbutton);

    Vector2f score1Position(windowWidth / 2 + 100 - (getDrawSize(player1Score, bigNums).x / 2), windowHeight / 2 - 175);
    Vector2f score2Position(windowWidth / 2 - 100 - (getDrawSize(player2Score, bigNums).x / 2), windowHeight / 2 - 175);

    /*RectangleShape score1BG(getDrawSize(player1Score, bigNums) + Vector2f(4, 4));
    score1BG.setPosition(score1Position - Vector2f(2, 3));
    score1BG.setFillColor(BLACK);
    score1BG.setOutlineColor(DARKRED);
    score1BG.setOutlineThickness(1);
    RectangleShape score2BG(getDrawSize(player2Score, bigNums) + Vector2f(4, 4));
    score2BG.setPosition(score2Position - Vector2f(2, 3));
    score2BG.setFillColor(BLACK);
    score2BG.setOutlineColor(DARKBLUE);
    score2BG.setOutlineThickness(1);

    window->draw(score1BG);
    window->draw(score2BG);*/

    drawNumbers(window, player1Score, score1Position, bigNums, DARKRED);
    drawNumbers(window, player2Score, score2Position, bigNums, DARKBLUE);

}
