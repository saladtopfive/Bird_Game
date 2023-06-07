// ---SFML--- //
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

// ---BASIC LIBS--- //
#include <vector>
#include <ctime>

// ---DEBUG--- //
#include <iostream>

// ---CLASS--- //
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

    // ---CONSTRUCTOR--- ///
    sf::Vector2f velocity = { 2,0 };
    AnimatedSprite(sf::Texture& texture, const std::vector<sf::IntRect>& frames, float frameDuration, sf::RenderWindow& window)
        : spriteFrames(frames), currentFrame(0), frameDuration(frameDuration), elapsedTime(0.0f), window(window) {
        setTexture(texture);
        setFrame(0);
    }

    // ---METHODS--- //
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
            setScale(2.0f, 2.0f); 
        }
        else {
            setScale(-2.0f, 2.0f); 
        }
    }

    void resetPosition() {
        startPosition = { 50.0f, std::rand() % 31 + 50.0f };
        setPosition(startPosition);
        velocity = sf::Vector2f(0.0f, 0.0f);
        acceleration = sf::Vector2f(0.0f, 0.0f);
        setScale(2.0f, 2.0f);
    }

};

int main() {
    std::srand(std::time(NULL)); // ---RANDOM--- //
    
    // ---TEXTURES--- //
    sf::RenderWindow window(sf::VideoMode(900, 600), "Catch the Fish"); // ---WINDOW--- //
    window.setFramerateLimit(60);

    sf::Texture birdTexture; // ---BIRD--- //
    if (!birdTexture.loadFromFile("bird_1.png")) {
        return 1;
    }

    sf::Texture fishTexture; // ---FISH--- //
    if (!fishTexture.loadFromFile("fish.png")) {
        return 1;
    }

    std::vector<sf::IntRect> birdFrames; // ---BIRD ANIMATION--- //
    birdFrames.emplace_back(0, 0, 48, 48);
    birdFrames.emplace_back(48, 0, 48, 48);
    birdFrames.emplace_back(96, 0, 48, 48);
    birdFrames.emplace_back(144, 0, 48, 48);
    AnimatedSprite bird(birdTexture, birdFrames, 1.0f / birdFrames.size(), window);
    bird.setOrigin(bird.getLocalBounds().width / 2, bird.getLocalBounds().height / 2);
    bird.setScale(2, 2);

    std::vector<sf::IntRect> fishFrames; // ---FISH ANIMATION--- //
    fishFrames.emplace_back(0, 0, 48, 19);

    
    AnimatedSprite fish(fishTexture, fishFrames, 1.0f / fishFrames.size(), window);
    fish.setScale(2, 2);
    fish.setOrigin(fish.getGlobalBounds().left + fish.getGlobalBounds().width / 2, fish.getGlobalBounds().top + fish.getGlobalBounds().height / 2);

    // ---BACKGROUND TEXTURES--- //
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

    sf::Sprite loseScreen;
    sf::Texture loseScreenTex;
    if (!loseScreenTex.loadFromFile("losecondition.jpg")){
        return 1;
    }
    loseScreen.setTexture(loseScreenTex);
    loseScreen.setPosition(0, 0);

    sf::Sprite sun;
    sf::Texture sunTex;
    if (!sunTex.loadFromFile("sun.png")) {
        return 1;
    }
    sun.setTexture(sunTex);
    sun.setPosition(window.getSize().x / 2, 20);
    sun.setScale(0.25, 0.25);

    // ---TEXT AND FONTS--- //
    sf::Font font;
    if (!font.loadFromFile("FONT.TTF")) {
        return 1;
    }
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setOrigin(scoreText.getGlobalBounds().left + scoreText.getGlobalBounds().width / 2, scoreText.getGlobalBounds().top + scoreText.getGlobalBounds().height / 2);
    scoreText.setPosition(10, 20);

    sf::Text winText;
    winText.setFont(font);
    winText.setCharacterSize(60);
    winText.setFillColor(sf::Color::Green);
    winText.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    winText.setScale(1.5, 1.5);

    sf::Text timeText;
    timeText.setFont(font);
    timeText.setCharacterSize(30);
    timeText.setFillColor(sf::Color::Black);
    timeText.setPosition(window.getSize().x/2 + 100  , 20);
    // ---TEXTURES END--- //

    // ---SOME VARIABLES--- //
    int score = 0;
    float birdSpeed = 5.0f;
    float fishSpeed = birdSpeed / 2.0f;
    float elapsedTime = 0.0f;
    float scoreTime = 10.0f;
    bool winCondition = false;
    bool loseCondition = false;
    bool pause = false;
    sf::Clock clock;

    // ---FISH & BIRD POSITIONS--- //
    bird.setPosition(window.getSize().x/2,80);
    fish.setPosition(std::rand() % 601 + 150.0f, window.getSize().y - 125.0f - std::rand() % 21);

    // ---MAIN LOOP--- //
    while (window.isOpen()) {
        int random1 = rand() % 256;
        int random2 = rand() % 256;
        int random3 = rand() % 256;
        // ---DEBUG--- //
        std::cout << pause << std::endl;
        sf::Event event;

    // ---EVENTS--- //
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        float deltaTime = clock.restart().asSeconds();
        elapsedTime += deltaTime;

        // ---ESCAPE--- ///
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        // ---BIRD MOVEMENT--- //
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)|| sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            bird.velocity.x = birdSpeed;
            bird.switchDirection();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)||sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            bird.velocity.x = -birdSpeed;
            bird.switchDirection();
        }
        else {
            bird.velocity.x = 0.0f;
        }

        // ---BIRD DIVE--- ///
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

        // ---BIRD ANIMATION--- //
        bird.moveAnimation(deltaTime);

        // ---FISH ANIMATION--- 
        fish.move(-fishSpeed * deltaTime, 0.0f);

        // ---BIRD FISH COLLISION--- //
        if (bird.getGlobalBounds().intersects(fish.getGlobalBounds())) {
            //pause = !pause;
            fish.setPosition(std::rand() % 601 + 150.0f, window.getSize().y - 125.0f);
            bird.setPosition(std::rand() % 601 + 150.0f, 80);
            score++;
            if (score == 10)
            {
                winCondition = 1;
            }
        }
        if (bird.getGlobalBounds().top + bird.getGlobalBounds().height > 600)
        {
            bird.setPosition(30, 30);
            pause = !pause;
        }

        // ---TIME--- //
        timeText.setString("TIME: " + std::to_string(elapsedTime));
        if (elapsedTime >= scoreTime) {
            pause = 0;
            bird.resetPosition();
            elapsedTime = 0.0f;
            score--;
            if (score < 0)
            {
                loseCondition = 1;
            }
        }
       
        // ---SCORE--- //
        scoreText.setString("SCORE: " + std::to_string(score));       
        if (pause == 1)
        {
            bird.move(0, bird.velocity.y);
            if (bird.getGlobalBounds().intersects(fish.getGlobalBounds()))
            {
                pause = !pause;
            }
        }
        if (winCondition)
        {
            scoreTime = 1000000.0f;
            window.clear(sf::Color(random1,random2,random3));
            //window.clear(sf::Color::Black);
            winText.setString("WYGRANA !!!");
            winText.setOrigin(winText.getLocalBounds().width / 2, winText.getLocalBounds().height / 2);
            winText.setPosition(window.getSize().x / 2, window.getSize().y / 2);                      
            winText.setFillColor(sf::Color(random3, random1, random2));                            
            window.draw(winText);    
            window.display();
        }
        if (loseCondition)
        {
            scoreTime = 1000000.0f;
            window.clear(sf::Color::Black);
            loseScreen.setPosition(70,0);
            loseScreen.setScale(2, 2);
            window.draw(loseScreen);
            window.display();
        }

        // ---DRAW--- //
        else
        {
            window.clear(sf::Color::Black);
            window.draw(background);
            window.draw(background2);
            window.draw(ground);
            window.draw(sun);
            window.draw(scoreText);
            window.draw(timeText);
            window.draw(tree1);
            window.draw(tree2);
            window.draw(bird);
            for (int i = 0; i < 3; i++) {
                window.draw(fish);
            }
            window.display();
        }
    }

    return 0;
}
