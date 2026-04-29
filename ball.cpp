#include <SFML/Graphics.hpp>
#include <optional>
#include <cstdlib>
#include <ctime>
#include<iostream>

using namespace std;
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
        shape.setOutlineThickness(30.f);
        shape.setRadius(150.f);
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
    sf::RectangleShape shape;

public:
    RectangleShapeObj(float x, float y, sf::Color color)
    {
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(color);
        shape.setOutlineThickness(30.f);
        shape.setSize({ 250.f, 250.f });
        shape.setOrigin({ 125.f, 125.f });
        shape.setPosition({ x, y });
    }
    void draw(sf::RenderWindow& window) override
    {
        window.draw(shape);
    }
};
class TriangleShapeObj : public Shape
{
private:
    sf::CircleShape shape;

public:
    TriangleShapeObj(float x, float y, sf::Color color)
    {
   
        shape = sf::CircleShape(200.f, 3);
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(color);
        shape.setOutlineThickness(30.f);
        shape.setOrigin({ 200.f, 200.f });
        shape.setPosition({ x, y });
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(shape);
    }
};

class HorizontalLine : public Shape
{
private:
    sf::RectangleShape line;

public:
    HorizontalLine(float y2, float width, sf::Color color)
    {
        line.setSize({ width, 20.f });
        line.setFillColor(color);

        line.setOrigin({ width / 2.f, 10.f });
        line.setPosition({ width / 2.f, y2 });
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(line);
    }
};
class PlusShape : public Shape
{
private:
    sf::RectangleShape horizontal;
    sf::RectangleShape vertical;

public:
    PlusShape(float x, float y, float size, sf::Color color)
    {
        // Horizontal line
        horizontal.setSize({ size, 20.f });
        horizontal.setFillColor(color);
        horizontal.setOrigin({ size / 2.f, 10.f });
        horizontal.setPosition({ x, y });

        // Vertical line
        vertical.setSize({ 20.f, size });
        vertical.setFillColor(color);
        vertical.setOrigin({ 10.f, size / 2.f });
        vertical.setPosition({ x, y });
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(horizontal);
        window.draw(vertical);
    }
};
class StarCollectible : public Shape
{
private:
    sf::Texture texture;
    sf::Sprite sprite;

public:
    StarCollectible(float x, float y):sprite(texture){
        if (!texture.loadFromFile("Star.png"))
        {
            std::cout << "❌ Star NOT loaded\n";
        }
        else
        {
            std::cout << "✅ Star loaded successfully\n";
        }
        sprite.setTexture(texture);

        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
        sprite.setColor(sf::Color(255, 255, 0));
        sprite.setScale({ 0.3f, 0.3f });
        sprite.setPosition({ x, y });
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(sprite);
    }
};
class Ball
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
        cameraFollowSpeed = 0.05f;
    }

    sf::View& getView()
    {
        return view;
    }

    // functions for camera setup and movement
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

void handleEvents(sf::RenderWindow& window)
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

void handleInput(Ball& ball, float jumpStrength)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space))
    {
        ball.setVelocityY(jumpStrength);
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
        addShape(shapes, count, capacity, new CircleShapeObj(x, y, sf::Color::Red));

    else if (type == 1)
        addShape(shapes, count, capacity, new RectangleShapeObj(x, y, sf::Color::Green));

    else if (type == 2)
        addShape(shapes, count, capacity, new TriangleShapeObj(x, y, sf::Color::Yellow));

    else if (type == 3)
        addShape(shapes, count, capacity, new HorizontalLine(y, width, sf::Color::White));

    else
    {
        float gapX = 200.f;
        addShape(shapes, count, capacity, new PlusShape(x - gapX / 2.f, y, 150.f, sf::Color::Cyan));
        addShape(shapes, count, capacity, new PlusShape(x + gapX / 2.f, y, 150.f, sf::Color::Magenta));
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
    float jumpStrength = -6.f;     // speed gained by ball on pressing spacebar

    sf::RenderWindow window(sf::VideoMode({ width, height }), "SFML Window");
    window.setFramerateLimit(60);
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
    addShape(shapes, count, capacity, new StarCollectible(width / 2.f, 600.f));
    Camera camera((float)width, (float)height);
    while (window.isOpen())
    {
        handleEvents(window);
        handleInput(ball, jumpStrength);
        applyGravity(ball, gravity);
        moveBall(ball);
        resetBallOnGround(ball, groundY);
        updateCamera(camera, ball, width, height);
        if (ball.getPosition().y < lastSpawnY + gap)
        {
            lastSpawnY -= gap;
            spawnShape(shapes, count, capacity, centerX, lastSpawnY, width);
        }
        window.setView(camera.getView());
        drawWindow(window, ball,shapes,count);
    }
    for (int i = 0; i < count; i++)
        delete shapes[i];

    delete[] shapes;

    return 0;
}
