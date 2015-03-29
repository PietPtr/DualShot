#include "LineGraph.h"
#include <iostream>

using namespace sf;

Vector2f drawNumbers(RenderWindow* window, int num, Vector2f position, Texture* texture, Color textColor);
Vector2f getDrawSize(int num, Texture* texture);

LineGraph::LineGraph(Vector2f _position, Color _lineColor, Color _dotColor, int _yMin, int _yMax, Texture* _smallNumbersTexture)
{
    position = _position;
    lineColor = _lineColor;
    dotColor = _dotColor;
    yMin = _yMin;
    yMax = _yMax;
    smallNumbersTexture = _smallNumbersTexture;
    for (int i = 0; i < 50; i++)
    {
        values[i] = 0;
    }
}

void LineGraph::update(float newValue)
{
    for (int i = 0; i < 49; i++)
    {
        values[i] = values[i + 1];
    }

    values[49] = newValue;


}

void LineGraph::draw(RenderWindow* window)
{
    RectangleShape rectBG(Vector2f(200, 125));
    rectBG.setFillColor(Color(0, 0, 0));
    rectBG.setPosition(position);
    //window->draw(rectBG);

    RectangleShape yAxis(Vector2f(1, 126));
    yAxis.setPosition(position);
    yAxis.setFillColor(lineColor);

    RectangleShape xAxis(Vector2f(200, 1));
    xAxis.setPosition(position + Vector2f(0, 126));
    xAxis.setFillColor(lineColor);

    for (int i = 0; i < 50; i++)
    {
        RectangleShape dot(Vector2f(1, 1));
        dot.setFillColor(dotColor);
        float difference = yMax - yMin;
        float drawValue;
        if (yMin < 0)
        {
           drawValue = ((values[i] + yMin) / difference) * 125;
        }
        if (yMin >= 0)
        {
            drawValue = ((values[i] - yMin) / difference) * 125;
        }
        drawValue = -drawValue;
        if (drawValue > 125)
            drawValue = 125;
        if (drawValue < 0)
            drawValue = 0;
        Vector2f dotPosition(position + Vector2f(i * 4 + 1, drawValue));
        dot.setPosition(dotPosition);

        if (i != 49)
        {
            float nextDrawValue;
            if (yMin < 0)
            {
               nextDrawValue = ((values[i + 1] + yMin) / difference) * 125;
            }
            if (yMin >= 0)
            {
                nextDrawValue = ((values[i + 1] - yMin) / difference) * 125;
            }
            nextDrawValue = -nextDrawValue;
            if (nextDrawValue > 125)
                nextDrawValue = 125;
            if (nextDrawValue < 0)
                nextDrawValue = 0;

            VertexArray line(Lines, 2);
            line[0].position = dot.getPosition();
            line[1].position = position + Vector2f((i + 1) * 4 + 1, nextDrawValue);

            line[0].color = lineColor;
            line[1].color = lineColor;

            window->draw(line);
        }

        if (i == 49)
        {
            drawNumbers(window, values[i], dotPosition + Vector2f(2, -2), smallNumbersTexture, lineColor);
        }
        window->draw(dot);
    }

    window->draw(yAxis);
    window->draw(xAxis);
}
