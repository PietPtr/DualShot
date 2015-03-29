#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class Player
{
    public:
        Player(int _playerSize, int _screenwidth, int _screenheight, Color _color, Color _lineColor);
        void draw(RenderWindow* window, Texture* smallNumbersTexture);
        void update(Vector2f circlePos, int radius);
        void setPosition(int x, int y);
        void setSize(int _playerSize);
        Vector2f getPosition() { return position; }
        int getSize();
        int getPoints() { return points; }
        void addPoints(int val) { points += val; }
        void subtractLives(int val) { lives -= val; }
        bool isDead() { return dead; }
        void setColors(Color _color, Color _lineColor);
    protected:
    private:
        int playerSize;
        Vector2f position;
        Vector2f playerVector;
        RectangleShape rect;
        RectangleShape xLine;
        RectangleShape yLine;
        int collisionCounter;
        int points;
        int lives;
        bool dead;
        Color color;
        Color lineColor;
};
