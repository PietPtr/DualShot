#include <SFML/Graphics.hpp>

using namespace sf;

class LineGraph
{
    public:
        LineGraph(Vector2f _position, Color _lineColor, Color _dotColor, int _yMin, int _yMax, Texture* _smallNumbers);
        void draw(RenderWindow* window); //draws the graph (axes, lines)
        void update(float value); //Shifts all values 1 to the left, deletes 0th value, appends new value, deafult val = 0
        void setPosition(Vector2f val) { position = val; }
    protected:
    private:
        float values[50];
        Vector2f position;
        Color lineColor;
        Color dotColor;
        int yMin;
        int yMax;
        Texture* smallNumbersTexture;
};
