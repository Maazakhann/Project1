#include <SFML/Graphics.hpp>
#include <optional>
#include <cstdlib>
#include <ctime>
#include<iostream>
#include<cmath>

using namespace std;
sf::Color palette[4] = {
    sf::Color(50, 226, 241),   // Cyan
    sf::Color(255, 232, 15),   // Yellow
    sf::Color(140, 18, 251),   // Violet
    sf::Color(255, 0, 128)     // Pink
};
sf::Color getRandomPaletteColor()
{
    return palette[rand() % 4];
}
void getShuffledColors(sf::Color colors[4])
{
    // Copy palette
    for (int i = 0; i < 4; i++)
        colors[i] = palette[i];

    // Shuffle (Fisher-Yates)
    for (int i = 3; i > 0; i--)
    {
        int j = rand() % (i + 1);
        std::swap(colors[i], colors[j]);
    }
}
class Shape
{
public:
    virtual void draw(sf::RenderWindow& window) = 0; // pure virtual
    virtual ~Shape() {}
};
class CircleShapeObj : public Shape
{
private:
    sf::CircleShape shape;

public:
    CircleShapeObj(float x, float y, sf::Color color)
    {
 
       
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(color);
        shape.setOutlineThickness(20.f);
        shape.setRadius(150.f);
        shape.setPointCount(100);
        shape.setOrigin({ 150.f, 150.f });
        shape.setPosition({ x, y });
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(shape);
    }
};

class RectangleShapeObj : public Shape
{
private:
    sf::RectangleShape top, bottom, left, right;

public:
    RectangleShapeObj(float x, float y)
    {
        float size = 250.f;
        float thick = 20.f;

        top.setSize({ size, thick });
        bottom.setSize({ size, thick });
        left.setSize({ thick, size });
        right.setSize({ thick, size });

        top.setOrigin({ size / 2.f, thick / 2.f });
        bottom.setOrigin({ size / 2.f, thick / 2.f });
        left.setOrigin({ thick / 2.f, size / 2.f });
        right.setOrigin({ thick / 2.f, size / 2.f });
        top.setPosition({ x, y - size / 2.f });
        bottom.setPosition({ x, y + size / 2.f });

        left.setPosition({ x - size / 2.f + thick / 2.f, y });
        right.setPosition({ x + size / 2.f - thick / 2.f, y });
sf::Color colors[4];
getShuffledColors(colors);

top.setFillColor(colors[0]);
bottom.setFillColor(colors[1]);
left.setFillColor(colors[2]);
right.setFillColor(colors[3]);
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(left);
        window.draw(right);
        window.draw(top);
        window.draw(bottom);
    }
};
class TriangleShapeObj : public Shape
{
private:
    sf::RectangleShape left, right, bottom;

public:
    TriangleShapeObj(float x, float y)
    {
        float base = 260.f;     
        float thickness = 20.f;
        float angle = 60.f;

        // Correct side length (same as base for equilateral triangle)
        float sideLength = base+5.f;

        sf::Color colors[4];
        getShuffledColors(colors);

        float baseY = y + 80.f;

        // Bottom
        bottom.setSize({ base, thickness });
        bottom.setOrigin({ base / 2.f, thickness / 2.f });
        bottom.setPosition({ x, baseY });
        bottom.setFillColor(colors[0]);

        // Left side
        left.setSize({ sideLength, thickness });
        left.setOrigin({ 0.f, thickness / 2.f });
        left.setPosition({ x - base / 2.f, baseY });
        left.setRotation(sf::degrees(-angle));
        left.setFillColor(colors[1]);

        // Right side
        right.setSize({ sideLength, thickness });
        right.setOrigin({ sideLength, thickness / 2.f });
        right.setPosition({ x + base / 2.f, baseY });
        right.setRotation(sf::degrees(angle));
        right.setFillColor(colors[2]);
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(left);
        window.draw(right);
        window.draw(bottom);
    }
};
class HorizontalLine : public Shape
{
private:
    sf::RectangleShape parts[4];

public:
    HorizontalLine(float y, float width)
    {
        float segmentWidth = width / 4.f;

        sf::Color colors[4];
        getShuffledColors(colors);

        for (int i = 0; i < 4; i++)
        {
            parts[i].setSize({ segmentWidth, 20.f });
            parts[i].setFillColor(colors[i]);

            parts[i].setOrigin({ segmentWidth / 2.f, 10.f });
            parts[i].setPosition({
                segmentWidth / 2.f + i * segmentWidth,
                y
                });
        }
    }

    void draw(sf::RenderWindow& window) override
    {
        for (int i = 0; i < 4; i++)
            window.draw(parts[i]);
    }
};
class PlusShape : public Shape
{
private:
    sf::RectangleShape left, right, top, bottom;

public:
    PlusShape(float x, float y, float size)
    {
        float thickness = 20.f;
        float half = size / 2.f;

        sf::Color colors[4];
        getShuffledColors(colors); // uses your palette

        // LEFT arm
        left.setSize({ half, thickness });
        left.setOrigin({ half, thickness / 2.f });
        left.setPosition({ x, y });
        left.setFillColor(colors[0]);

        // RIGHT arm
        right.setSize({ half, thickness });
        right.setOrigin({ 0.f, thickness / 2.f });
        right.setPosition({ x, y });
        right.setFillColor(colors[1]);

        // TOP arm
        top.setSize({ thickness, half });
        top.setOrigin({ thickness / 2.f, half });
        top.setPosition({ x, y });
        top.setFillColor(colors[2]);

        // BOTTOM arm
        bottom.setSize({ thickness, half });
        bottom.setOrigin({ thickness / 2.f, 0.f });
        bottom.setPosition({ x, y });
        bottom.setFillColor(colors[3]);
    }

    void draw(sf::RenderWindow& window) override
    {
        // IMPORTANT: order controls clean center
        window.draw(top);
        window.draw(bottom);
        window.draw(left);
        window.draw(right);
    }
};
class StarCollectible : public Shape
{
private:
    sf::Texture texture;
    sf::Sprite sprite;

public:
    StarCollectible(float x, float y) : sprite(texture)
    {
        if (!texture.loadFromFile("Star.png"))
        {
            cout << "Star NOT loaded\n";
        }
        else
        {
            cout << "Star loaded successfully\n";
        }

        sprite.setTexture(texture, true);

        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });

        sprite.setScale({ .3f, .3f });   // bigger for testing
        sprite.setPosition({ x, y });
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(sprite);
    }
}; class Ball
{
private:
    sf::CircleShape circle;
    float velocityY;

public:
    Ball(float radius, float startX, float startY)
    {
        circle.setRadius(radius);
        circle.setFillColor(sf::Color(128, 213, 111));
        circle.setOrigin({ radius, radius });
        circle.setPosition({ startX, startY });
        velocityY = 0.f;
    }
    // functions for ball positioning 

    void draw(sf::RenderWindow& window)
    {
        window.draw(circle);
    }


    void setPosition(float x, float y)
    {
        circle.setPosition({ x, y });
    }

    sf::Vector2f getPosition() const
    {
        return circle.getPosition();
    }

    // functions for ball movement

    void move(float dx, float dy)
    {
        circle.move({ dx, dy });
    }

    void setVelocityY(float v)
    {
        velocityY = v;
    }

    float getVelocityY() const
    {
        return velocityY;
    }

    void addVelocityY(float value)
    {
        velocityY += value;
    }
};

class Camera
{
private:
    sf::View view;
    float cameraVelocity;
    float cameraFollowSpeed;

public:
    Camera(float width, float height)
        : view(sf::FloatRect({ 0.f, 0.f }, { width, height }))
    {
        cameraVelocity = 0.f;
        cameraFollowSpeed = 0.04f;
    }

    sf::View& getView()
    {
        return view;
    }

    float getCameraVelocity() const
    {
        return cameraVelocity;
    }

    void setCameraVelocity(float v)
    {
        cameraVelocity = v;
    }

    float getCameraFollowSpeed() const
    {
        return cameraFollowSpeed;
    }

    void move(float dx, float dy)
    {
        view.move({ dx, dy });
    }

    void setCenter(float x, float y)
    {
        view.setCenter({ x, y });
    }

    sf::Vector2f getCenter() const
    {
        return view.getCenter();
    }
};

void handleEvents(sf::RenderWindow& window, Ball& ball, float jumpStrength)
{
    while (auto event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }

        if (const auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            if (key->scancode == sf::Keyboard::Scan::Space)
            {
                ball.setVelocityY(jumpStrength);
            }
        }
    }
}



void applyGravity(Ball& ball, float gravity)
{
    ball.addVelocityY(gravity);   // adding gravity to ball speed each frame
}

void moveBall(Ball& ball)
{
    ball.move(0.f, ball.getVelocityY());
}

void resetBallOnGround(Ball& ball, float groundY)
{
    // rest the ball if it reached the ground
    if (ball.getPosition().y >= groundY)
    {
        ball.setPosition(ball.getPosition().x, groundY);
        ball.setVelocityY(0.f);
    }
}

void updateCamera(Camera& camera, Ball& ball, int width, int height)
{
    float triggerLine = camera.getCenter().y;   // where the view started to follow the ball

    if (ball.getPosition().y < triggerLine)
    {
        float diff = ball.getPosition().y - triggerLine;   // distance between center of window and the ball
        camera.setCameraVelocity(diff * camera.getCameraFollowSpeed());   // converts the distance into a movement speed
    }
    else
    {
        camera.setCameraVelocity(0.f);   // if ball is below the center the camera doesnt follow it
    }

    camera.move(0.f, camera.getCameraVelocity());

    // stop camera below starting point
    if (camera.getCenter().y > height / 2.f)
    {
        camera.setCenter(width / 2.f, height / 2.f);
        camera.setCameraVelocity(0.f);
    }
}

void drawWindow(sf::RenderWindow& window, Ball& ball, Shape* shapes[], int shapeCount)
{
    window.clear(sf::Color(41, 41, 41));  // background colour only for test 

    for (int i = 0; i < shapeCount; i++)
    {
        shapes[i]->draw(window);
    }
    ball.draw(window);
    window.display();
}
void resizeArray(Shape**& shapes, int& capacity);
void addShape(Shape**& shapes, int& count, int& capacity, Shape* newShape);
void spawnShape(Shape**& shapes, int& count, int& capacity, float x, float y, float width);

void removeFirst(Shape**& shapes, int& count)
{
    delete shapes[0];

    for (int i = 1; i < count; i++)
        shapes[i - 1] = shapes[i];

    count--;
}
void addShape(Shape**& shapes, int& count, int& capacity, Shape* newShape)
{
    if (count >= capacity)
        resizeArray(shapes, capacity);

    shapes[count++] = newShape;
}
void spawnShape(Shape**& shapes, int& count, int& capacity, float x, float y, float width)
{
    int type = rand() % 5;

    if (type == 0) 
        addShape(shapes, count, capacity, new CircleShapeObj(x, y));

    else if (type == 1)
        addShape(shapes, count, capacity, new RectangleShapeObj(x, y));

    else if (type == 2)
        addShape(shapes, count, capacity, new TriangleShapeObj(x, y));

    else if (type == 3)
        addShape(shapes, count, capacity, new HorizontalLine(y, width));

    else
    {
        float gapX = 170.f;
        addShape(shapes, count, capacity, new PlusShape(x - gapX / 2.f, y, 150.f));
        addShape(shapes, count, capacity, new PlusShape(x + gapX / 2.f, y, 150.f));
    }
    addShape(shapes, count, capacity, new StarCollectible(x, y));
}
void resizeArray(Shape**& shapes, int& capacity)
{
    int newCapacity = capacity * 2;

    Shape** newArr = new Shape * [newCapacity];

    for (int i = 0; i < capacity; i++)
        newArr[i] = shapes[i];

    delete[] shapes;
    shapes = newArr;
    capacity = newCapacity;
}
int main()
{
    const int width = 800;
    const int height = 900;
    const float groundY = 880.f;   // where the ball is placed initaially

    float gravity = 0.5f;          // speed of gravity
    float jumpStrength = -10.f;     // speed gained by ball on pressing spacebar

    sf::RenderWindow window(sf::VideoMode({ width, height }), "SFML Window");
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);
    srand(time(0));
    float centerX = width / 2.f;

    float x1 = centerX;
    float x2 = centerX;
    float x3 = centerX;

    float baseY = 100.f;
    float gap = 700.f;

    float y1 = baseY + (rand() % 50);
    float y2 = baseY - gap + (rand() % 50);
    float y3 = baseY - 2 * gap + (rand() % 50);
    float y4 = baseY - 3 * gap + (rand() % 50);
    float y5 = baseY - 4 * gap + (rand() % 50);
    float gapX = 200.f;   // distance between the two +
    Shape** shapes = new Shape * [10];  // initial capacity
    int count = 0;
    int capacity = 10;

    float lastSpawnY = baseY;

    for (int i = 0; i < 5; i++)
    {
        spawnShape(shapes, count, capacity, centerX, lastSpawnY, width);
        lastSpawnY -= gap;
    }
    Ball ball(20.0f, width / 2.f, groundY);
    Camera camera((float)width, (float)height);
    while (window.isOpen())
    {
        handleEvents(window, ball, jumpStrength);
        applyGravity(ball, gravity);
        moveBall(ball);
        resetBallOnGround(ball, groundY);
        updateCamera(camera, ball, width, height);
        if (ball.getPosition().y < lastSpawnY + gap)
        {
            spawnShape(shapes, count, capacity, centerX, lastSpawnY, width);
            lastSpawnY -= gap;
        }
        window.setView(camera.getView());
        drawWindow(window, ball,shapes,count);
    }
    for (int i = 0; i < count; i++)
        delete shapes[i];

    delete[] shapes;

    return 0;
}
