#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <optional>
#include <cmath>
using namespace std;
using namespace sf;

const int windowWidth = 800;
const int windowHeight = 600;
const float paddleWidth = 50.f;
const float paddleHeight = 50.f;
const float ballRadius = 10.f;

class Character
{
    private:
        RectangleShape rectangle;
        Vector2f velocity;
    public:
        Character()
        {velocity = Vector2f(0.0f, 0.0f);}
        bool isMouseCollision(Vector2f pos)
        {
            return rectangle.getGlobalBounds().contains(pos);
        }
        Character(Vector2f Size, Color Color, Vector2f Position) 
        {
            rectangle.setSize(Size);
            rectangle.setFillColor(Color);
            rectangle.setPosition(Position);
            rectangle.setOrigin({Size.x/2, Size.y/2});
            velocity = Vector2f(0.0f, 0.0f);
        }
        Character(Vector2f Size, Vector2f Position) 
        {
            rectangle.setSize(Size);
            rectangle.setPosition(Position);
            rectangle.setOrigin({Size.x/2, Size.y/2});
            velocity = Vector2f(0.0f, 0.0f);
        }

        void setColor(Color color)
        {
            rectangle.setFillColor(color);
        }
        /*bool isCollision(Character* character)
        {
            Vector2f characterPos = character->getPosition();
            if(rectangle.getPosition().x + 10 == characterPos.x - 10 || rectangle.getPosition().x - 10 == characterPos.x + 10)
                return true;
            return false;
        }*/
        bool isCollision(Character* character) {
            // Get the global bounds of both characters
            sf::FloatRect bounds1 = rectangle.getGlobalBounds();
            sf::FloatRect bounds2 = character->rectangle.getGlobalBounds();
        
            // Check for intersection
            std::optional<sf::FloatRect> intersection = bounds1.findIntersection(bounds2);
            return intersection.has_value();
        }

        void drawShape(RenderWindow &window)
        {
            window.draw(rectangle);
        }
        FloatRect getGlobalBounds()
        {
            return rectangle.getGlobalBounds();
        }
        void offset(Vector2f ofst)
        {
            rectangle.move(ofst);
        }

        Vector2f getPosition()
        {
            return rectangle.getPosition();
        }

        void setPosition(Vector2f pos)
        {
            rectangle.setPosition(pos);
        }

        void setVelocity(Vector2f vel)
        {
            velocity = vel;
        }

        Vector2f getVelocity()
        {
            return velocity;
        }

        void move()
        {
            rectangle.move(velocity);
        }

        void move(Vector2f vel)
        {
            rectangle.move(vel);
        }

};

int main()
{
    RenderWindow window(VideoMode(Vector2u(windowWidth, windowHeight)), "SFML Ping Pong");
    window.setFramerateLimit(120);
    srand(time(NULL));
    vector < Character > v;
    int characterAmount = 0;
    bool isMouseReleased = true;
    bool isCollision = false;

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
            }
            if(const auto* keypressed = event->getIf<Event::MouseButtonReleased>())
            {
                if(keypressed->button == Mouse::Button::Left)
                {
                    isMouseReleased = true;
                }
                
            }
            else if (const auto *keyPressed = event->getIf<Event::KeyPressed>())
            {
                if (keyPressed->scancode == Keyboard::Scancode::Escape)
                    window.close();
            }
        }

        Vector2f localPosition = static_cast<Vector2f>(Mouse::getPosition(window));
        for(auto &i : v)
        {
            Vector2f characterPos = i.getPosition();
            if(localPosition.x > characterPos.x - 20 && localPosition.x < characterPos.x + 20 && localPosition.y > characterPos.y - 20 && localPosition.y < characterPos.y + 20)
            {
                isCollision = true;
                break;
            }
            isCollision = false;
        }

        if(Mouse::isButtonPressed(Mouse::Button::Left) && isMouseReleased && !isCollision)
        {
            Character shape({20, 20}, Color::Blue, localPosition);
            
            if(characterAmount % 6 == 0)
                shape.setColor(Color::Blue);
            else if(characterAmount % 6 == 1)
                shape.setColor(Color::Red);
            else if(characterAmount % 6 == 2)
                shape.setColor(Color::Green);
            else if(characterAmount % 6 == 3)
                shape.setColor(Color::Cyan);
            else if(characterAmount % 6 == 4)
                shape.setColor(Color::Magenta);
            else if(characterAmount % 6 == 5)
                shape.setColor(Color::Yellow);
            
            shape.setVelocity({0.9f, 0.0f});
            characterAmount++;
            v.push_back(shape);
            isMouseReleased = false;                       
        }

        for (auto& i : v) {
            i.move();
            sf::FloatRect bounds = i.getGlobalBounds();
        
            // Check for collisions with window boundaries
            if (bounds.position.x + 10 >= windowWidth || bounds.position.x - 10 <= 0) {
                i.setVelocity({-i.getVelocity().x, i.getVelocity().y});
            }
        }
        for(int i = 0; i < characterAmount; i++)
        {
            for(int j = 0; j < characterAmount; j++)
            {
                if(i != j && v[i].isCollision(&v[j]))
                {
    
                    v[i].setVelocity({-v[i].getVelocity().x, v[i].getVelocity().y});
                    v[j].setVelocity({-v[j].getVelocity().x, v[j].getVelocity().y});
                    v[j].move();
                    v[i].move();
                }
         
            }
        }
        window.clear();
        for(auto &i : v)
        {
            i.drawShape(window);
        }
        window.display();
        
    }

    return 0;
}
