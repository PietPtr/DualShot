#include "Player.h"
#include "Target.h"

bool circleColision(Vector2f circlePos, Vector2f pointPos, int radius);
Vector2f drawNumbers(RenderWindow* window, int num, Vector2f position, Texture* texture, Color textColor);
Vector2f getDrawSize(int num, Texture* texture);

using namespace sf;

Player::Player(int _playerSize, int _screenwidth, int _screenheight, Color _color, Color _lineColor)
{
    playerSize = _playerSize;
    playerVector = Vector2f(_playerSize, _playerSize);
    rect = RectangleShape(playerVector);
    xLine = RectangleShape(Vector2f(_screenwidth, 1));
    yLine = RectangleShape(Vector2f(1, _screenheight));
    rect.setFillColor(_color);
    xLine.setFillColor(_lineColor);
    yLine.setFillColor(_lineColor);
    color = _color;
    lineColor = _lineColor;
    points = 0;
    lives = 3;
    dead = false;
}

void Player::draw(RenderWindow* window, Texture* smallNumbersTexture)
{
    //std::cout << window->getSize().x << "\n";
    rect.setSize(Vector2f(playerSize, playerSize));
    rect.setPosition(Vector2f(position.x - playerSize / 2, position.y - playerSize / 2));

    yLine.setFillColor(lineColor);
    yLine.setPosition(position.x, 0);
    xLine.setFillColor(lineColor);
    xLine.setPosition(0, position.y);

    rect.setFillColor(color);

    Vector2f rectSize = getDrawSize(points, smallNumbersTexture);

    RectangleShape rectBG(rectSize + Vector2f(2, 2));
    rectBG.setPosition(position + Vector2f(4, -12));
    rectBG.setFillColor(Color(0, 0, 0));
    rectBG.setOutlineColor(lineColor);
    rectBG.setOutlineThickness(1);
    window->draw(rectBG);

    drawNumbers(window, points, position + Vector2f(5, -11), smallNumbersTexture, lineColor); //Inefficient, numbers are already drawn there ^

    RectangleShape lockon;
    if (collisionCounter != 0)
    {
        int lockonSize = (400 - collisionCounter * collisionCounter) / 2;
        lockon.setSize(Vector2f(lockonSize, lockonSize));
        lockon.setPosition(position - Vector2f(lockon.getSize().x / 2.0, lockon.getSize().y / 2.0));
        lockon.setFillColor(Color(0, 0, 0, 0));
        lockon.setOutlineColor(lineColor);
        lockon.setOutlineThickness(1);
        window->draw(lockon);
    }

    for (int i = 0; i < lives; i++)
    {
        CircleShape life(3);
        life.setPosition(position + Vector2f(i * 9 + 4, 4));
        life.setFillColor(Color(0, 0, 0));
        life.setOutlineColor(lineColor);
        life.setOutlineThickness(1);
        window->draw(life);
    }

    window->draw(xLine);
    window->draw(yLine);
    window->draw(rect);
}

void Player::update(Vector2f circlePos, int radius)
{
    playerSize = playerSize > 5 ? playerSize - 10 : 5;

    if (lives <= 0)
    {
        dead = true;
    }

    if (circleColision(circlePos, position, radius) && radius >= 2 && playerSize == 5)
    {
        collisionCounter++;
    }
    else
    {
        collisionCounter = 0;
    }

    if (collisionCounter > 20 && playerSize <= 5)
    {
        playerSize = 100;
        collisionCounter = 0;
        points += (90 - radius);
    }
}

void Player::setColors(Color _color, Color _lineColor)
{
    color = _color;
    lineColor = _lineColor;
}

void Player::setPosition(int x, int y)
{
    position = Vector2f(x, y);
}

void Player::setSize(int _playerSize) { playerSize = _playerSize; }

int Player::getSize() { return playerSize; }
