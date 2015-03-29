#include "Target.h"
#include <math.h>
#include <iostream>

bool circleColision(Vector2f circlePos, Vector2f pointPos, int radius);
int randint(int low, int high, int extraSeed);
int randint(int low, int high);

using namespace sf;

Target::Target(Vector2f _position, int _targetRadius, Color _outlineColor, Color _fillColor, Color _outerColor)
{
    position = _position;
    targetRadius = (float)_targetRadius;
    outlineColor = _outlineColor;
    fillColor = _fillColor;
    outerColor = _outerColor;
    target = CircleShape(_targetRadius);
    target.setFillColor(_fillColor);
    target.setOutlineThickness(1);
    target.setOutlineColor(_outerColor);
    hit = false;
}

void Target::draw(RenderWindow* window)
{
    Color lightGray = Color(100, 100, 100);
    Color darkGray = Color(25, 25, 25);

    target.setPosition(Vector2f(position.x - targetRadius, position.y - targetRadius));
    target.setFillColor(Color(0, 0, 0, 0));
    target.setRadius(targetRadius);
    if (hit)
    {
        target.setOutlineColor(lightGray);
    }
    else
    {
        target.setOutlineColor(outerColor);
    }

    for (int i = targetRadius / 10; i > 0 ; i--)
    {
        CircleShape inner(i * 10);
        inner.setPosition(Vector2f(position.x - i * 10, position.y - i * 10));
        inner.setFillColor(fillColor);
        inner.setOutlineThickness(1);
        inner.setOutlineColor(hit == false ? outlineColor : darkGray);
        window->draw(inner);
    }

    window->draw(target);
}

int Target::update(float difficulty)
{
    targetRadius = targetRadius > 0 ? targetRadius - 0.25 * difficulty : 0;

    if (targetRadius <= 0)
        return 1;
    else
        return 0;
    return 0;
}

int Target::reset(int windowWidth, int windowHeight)
{
    targetRadius = 90;
    target.setOutlineColor(outerColor);

    Vector2f screenCenter = Vector2f(windowWidth / 2, windowHeight / 2);
    int spawnRadius = (windowHeight / 200 - (windowHeight / 2000)) * 100;

    while (true)
    {
        position = screenCenter + Vector2f(randint(-spawnRadius, spawnRadius, position.y), randint(-spawnRadius, spawnRadius, position.x));
        if (circleColision(screenCenter, position, spawnRadius))
        {
            break;
        }
        else
        {
            continue;
        }
    }

    if (hit == true)
    {
        hit = false;
        return 0;
    }
    else if (hit == false)
    {
        return 1;
    }
}
