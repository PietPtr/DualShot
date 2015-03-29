#include <SFML/Graphics.hpp>

using namespace sf;

class Target
{
    public:
        Target(Vector2f _position, int _targetRadius, Color _outlineColor, Color _fillColor, Color _outerColor);
        void draw(RenderWindow* window);
        int update(float difficulty);
        int reset(int windowWidth, int windowHeight);
        Vector2f getPosition() { return position; }
        void Setposition(Vector2f val) { position = val; }
        float getRadius() { return targetRadius; }
        void setRadius(int val) { targetRadius = val; }
        void setHit(bool val) { hit = val; }
        bool getHit() { return hit; }
    protected:
    private:
        Vector2f position;
        float targetRadius;
        float oldRadius;
        CircleShape target;
        Color fillColor;
        Color outlineColor;
        Color outerColor;
        bool hit;
};
