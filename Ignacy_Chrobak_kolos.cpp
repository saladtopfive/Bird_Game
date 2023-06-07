#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <ctime>

class AnimatedSprite : public sf::Sprite {
private:
    sf::Vector2f acceleration;
    sf::Vector2f startPosition;
    std::vector<sf::IntRect> spriteFrames;
    int currentFrame;
    float frameDuration;
    float elapsedTime;
    sf::RenderWindow& window;
public:
    sf::Vector2f velocity = { 2,0 };
    AnimatedSprite(sf::Texture& texture, const std::vector<sf::IntRect>& frames, float frameDuration, sf::RenderWindow& window)
        : spriteFrames(frames), currentFrame(0), frameDuration(frameDuration), elapsedTime(0.0f), window(window) {
        setTexture(texture);
        setFrame(0);
    }

    void setFrame(int frameIndex) {
        currentFrame = frameIndex % spriteFrames.size();
        setTextureRect(spriteFrames[currentFrame]);
    }

    void switchFrame() {
        setFrame(currentFrame + 1);
    }

    void setSpeed(float speed) {
        frameDuration = speed / spriteFrames.size();
    }

    void moveAnimation(float deltaTime) {
        move(velocity.x,velocity.y);
        elapsedTime += deltaTime;
        if (elapsedTime >= frameDuration) {
            switchFrame();
            elapsedTime = 0.0f;
        }
    }

    void move(float offsetX, float offsetY) {
        sf::Vector2f newPosition = getPosition() + sf::Vector2f(offsetX, offsetY);
       
        if (newPosition.x < 0 ) {
            velocity.x = std::abs(velocity.x);
            switchDirection();
        }
        if (newPosition.x + getGlobalBounds().width > window.getSize().x) {
            velocity.x = -std::abs(velocity.x);
            switchDirection();
        }
        sf::Sprite::move(velocity.x, velocity.y);
        
    }



    void fall(float accelerationX, float accelerationY) {
        velocity.x += accelerationX;
        velocity.y += accelerationY;
        move(velocity.x, velocity.y);
    }

    void handleWindowCollision(const sf::Vector2u& windowSize) {
        if (getPosition().x < 0 || getPosition().x + getGlobalBounds().width > windowSize.x) {
            velocity.x = -velocity.x;
            switchDirection();
        }
        if (getPosition().y < 0 || getPosition().y + getGlobalBounds().height > windowSize.y) {
            velocity.y = -velocity.y;
        }
    }

    void switchDirection() {
        if (velocity.x > 0) {
            setScale(1.0f, 1.0f); // odwrócenie w prawo
        }
        else {
            setScale(-1.0f, 1.0f); // odwrócenie w lewo
        }
    }

    void resetPosition() {
        startPosition = { 50.0f, std::rand() % 31 + 50.0f };
        setPosition(startPosition);
        velocity = sf::Vector2f(0.0f, 0.0f);
        acceleration = sf::Vector2f(0.0f, 0.0f);
        setScale(1.0f, 1.0f);
    }

};

int main() {
    std::srand(std::time(NULL)); // Inicjalizacja generatora pseudolosowego
    
    sf::RenderWindow window(sf::VideoMode(900, 600), "Catch the Fish");
    window.setFramerateLimit(60);
    sf::Texture birdTexture;
    if (!birdTexture.loadFromFile("bird_1.png")) {
        return 1;
    }

    sf::Texture fishTexture;
    if (!fishTexture.loadFromFile("fish.png")) {
        return 1;
    }

    std::vector<sf::IntRect> birdFrames;
    birdFrames.emplace_back(0, 0, 48, 48);
    birdFrames.emplace_back(48, 0, 48, 48);
    birdFrames.emplace_back(96, 0, 48, 48);
    birdFrames.emplace_back(144, 0, 48, 48);
    AnimatedSprite bird(birdTexture, birdFrames, 1.0f / birdFrames.size(), window);
    bird.setOrigin(bird.getLocalBounds().width / 2, bird.getLocalBounds().height / 2);
    
    

    std::vector<sf::IntRect> fishFrames;
    fishFrames.emplace_back(0, 0, 48, 19);
    AnimatedSprite fish(fishTexture, fishFrames, 1.0f / fishFrames.size(), window);


    sf::Sprite background;
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("gradient.png")) {
        return 1;
    }
    background.setTexture(backgroundTexture);

    sf::Sprite ground;
    sf::Texture groundTexture;
    if (!groundTexture.loadFromFile("water2.png")) {
        return 1;
    }
    ground.setTexture(groundTexture);
    ground.setPosition(0, window.getSize().y - ground.getGlobalBounds().height);
    
    sf::Sprite background2;
    sf::Texture groundTexture2;
    if (!groundTexture2.loadFromFile("bg3.png")) {
        return 1;
    }
    background2.setTexture(groundTexture2);
    background2.setPosition(0, window.getSize().y/2);
    //background2.setScale(0.2, 0.2);
    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        return 1;
    }
    sf::Sprite tree1;
    sf::Texture treeTex1;
    if (!treeTex1.loadFromFile("tree_4.png")) {
        return 1;
    }
    tree1.setTexture(treeTex1);
    tree1.setPosition(200, window.getSize().y / 4);

    sf::Sprite tree2;
    sf::Texture treeTex2;
    if (!treeTex2.loadFromFile("tree_6.png")) {
        return 1;
    }
    tree2.setTexture(treeTex2);
    tree2.setPosition(window.getSize().x/1.5, window.getSize().y / 2);

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(window.getSize().x - 200, 20);

    sf::Text winText;
    winText.setFont(font);
    winText.setCharacterSize(60);
    winText.setFillColor(sf::Color::Green);
    winText.setPosition(window.getSize().x / 2, window.getSize().y / 2);

    int score = 0;
    float birdSpeed = 10.0f;
    float fishSpeed = birdSpeed / 2.0f;

    float elapsedTime = 0.0f;
    sf::Clock clock;

    // Ustawienie pocz¹tkowych pozycji ptaka i ryby
    bird.setPosition(30,30);
    fish.setPosition(std::rand() % 601 + 150.0f, window.getSize().y - 125.0f - std::rand() % 21);
    bool pause = 0;
    while (window.isOpen()) {
        std::cout << pause << std::endl;
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();
        elapsedTime += deltaTime;

        // Sterowanie ruchem ptaka
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            bird.velocity.x = birdSpeed;
            bird.switchDirection();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            bird.velocity.x = -birdSpeed;
            bird.switchDirection();
        }
        else {
            bird.velocity.x = 0.0f;
        }

        // Lot nurkowy ptaka
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            pause = 1;
                      
        }
        if (pause)
        {
            bird.velocity.y = birdSpeed;
        }
        else {
            bird.velocity.y = 0.0f;
        }

        // Poruszanie ptakiem
        bird.moveAnimation(deltaTime);

        // Poruszanie ryb¹
        fish.move(-fishSpeed * deltaTime, 0.0f);

        // Sprawdzenie kolizji ptaka z ryb¹
        if (bird.getGlobalBounds().intersects(fish.getGlobalBounds())) {
            pause = !pause;
            fish.setPosition(std::rand() % 601 + 150.0f, window.getSize().y - 125.0f);
            bird.setPosition(30, 30);
            score++;
        }
        if (bird.getGlobalBounds().top + bird.getGlobalBounds().height > 600)
        {
            bird.setPosition(30, 30);
            pause = !pause;
        }

        // Powrót ptaka na pozycjê pocz¹tkow¹ po 6 sekundach
        if (elapsedTime >= 6.0f) {
            bird.resetPosition();
            elapsedTime = 0.0f;
            score--;
        }

        // Wyœwietlanie wyniku
        scoreText.setString("Score: " + std::to_string(score));

        // Wyœwietlanie elementów
        if (pause == 1)
        {
            bird.move(0, bird.velocity.y);
            if (bird.getGlobalBounds().intersects(fish.getGlobalBounds()))
            {
                pause = !pause;
            }
        }

        if (score == 1)
        {
            window.clear(sf::Color::Black);
            winText.setString("WYGRANA !!!");
        }
        window.clear();
        window.draw(background);
        window.draw(background2);
        window.draw(ground);
        window.draw(bird);
        window.draw(fish);
        window.draw(scoreText);
        window.draw(tree1);
        window.draw(tree2);
        window.display();
    }

    return 0;
}
