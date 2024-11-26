#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace sf;
using namespace std;

// 주인공 캐릭터
class Character {
public:
    Character(const string& textureRightFile, const string& textureLeftFile, float x, float y) {
        if (!textureRight.loadFromFile(textureRightFile)) {
            throw runtime_error("오른쪽 사진 로드 실패");
        }
        if (!textureLeft.loadFromFile(textureLeftFile)) {
            throw runtime_error("왼쪽 사진 로드 실패");
        }

        sprite.setTexture(textureRight);  // 초기에는 오른쪽을 향한 이미지 사용
        sprite.setPosition(x, y);         // 초기 위치 설정
    }

    void changeLeft() {
        sprite.setTexture(textureLeft);
    }

    void moveLeft() {
        sprite.move(-0.2f, 0);
    }

    void changeRight() {
        sprite.setTexture(textureRight);
    }

    void moveRight() {
        sprite.move(0.2f, 0);
    }

    void draw(RenderWindow& window) {
        window.draw(sprite);
    }

    float getX() const {
        return sprite.getPosition().x;
    }

    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

private:
    Texture textureRight;  // 오른쪽 방향
    Texture textureLeft;   // 왼쪽 방향
    Sprite sprite;
};

// 남자 캐릭터
class Man {
public:
    Man(Texture& texture, float x, float y)
        : speed(100.0f), directionChangeInterval(2.0f), elapsedTime(0.0f), stopped(false) {
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);

        direction = (rand() % 2 == 0) ? -1 : 1;
        updateSpriteScale();

        if (!stoppedTexture.loadFromFile("graphics/ManStop.png")) {
            throw runtime_error("멈춤 이미지 로드 실패");
        }

        originalTexture = texture; // 원래 텍스처 저장
    }

    void update(float deltaTime, float leftBoundary, float rightBoundary) {
        if (stopped) {
            if (stopClock.getElapsedTime().asSeconds() >= 0.5f) {
                stopped = false; // 멈춘 상태 해제
                sprite.setTexture(originalTexture); // 원래 텍스처로 복원
            }
            return;
        }

        elapsedTime += deltaTime;

        if (elapsedTime >= directionChangeInterval) {
            direction = (rand() % 2 == 0) ? -1 : 1;
            updateSpriteScale();
            elapsedTime = 0.0f;
        }

        float currentX = sprite.getPosition().x;
        sprite.move(speed * direction * deltaTime, 0);

        if (currentX < leftBoundary) {
            sprite.setPosition(leftBoundary, sprite.getPosition().y);
            direction = 1;
            updateSpriteScale();
        }
        else if (currentX > rightBoundary) {
            sprite.setPosition(rightBoundary, sprite.getPosition().y);
            direction = -1;
            updateSpriteScale();
        }
    }

    void moveWithBackground(float offsetX) {
        sprite.move(offsetX, 0); // 배경 이동량만큼 스프라이트를 이동
    }

    void draw(RenderWindow& window) {
        window.draw(sprite);
    }

    void handleMouseClick(const Vector2f& mousePosition) {
        if (sprite.getGlobalBounds().contains(mousePosition)) {
            stopped = true; // 멈춘 상태로 전환
            sprite.setTexture(stoppedTexture); // 멈춤 텍스처로 변경
            stopClock.restart(); // 0.5초 타이머 시작
        }
    }

private:
    Sprite sprite;
    Texture originalTexture;    // 원래 텍스처
    Texture stoppedTexture;     // 멈춘 상태 텍스처
    float speed;                // 이동 속도
    int direction;              // 이동 방향 (-1: 왼쪽, 1: 오른쪽)
    float directionChangeInterval; // 방향 변경 간격 (초 단위)
    float elapsedTime;          // 방향 변경까지의 경과 시간
    bool stopped;               // 멈춘 상태 여부
    Clock stopClock;            // 멈춘 상태 지속 시간 측정

    void updateSpriteScale() {
        sprite.setScale((direction == -1) ? -1.0f : 1.0f, 1.0f);
    }
};

int main() {
    Texture manTexture;
    if (!manTexture.loadFromFile("graphics/man.png")) {
        cerr << "Man 사진 로드 실패" << endl;
        return -1;
    }

    try {
        srand(static_cast<unsigned>(time(0)));

        VideoMode vm(1000, 700);
        RenderWindow window(vm, "SendEyeGaze", Style::Default);

        Texture textureBackground;
        if (!textureBackground.loadFromFile("graphics/background.png")) {
            throw runtime_error("배경 사진 로드 실패");
        }

        Sprite spriteBackground;
        spriteBackground.setTexture(textureBackground);
        spriteBackground.setPosition(0, 0);

        Character character("graphics/girlRight.png", "graphics/girlLeft.png", 500, 280.0f);

        vector<Man> men;
        for (int i = 0; i < 5; i++) {
            float randomX = static_cast<float>(rand() % static_cast<int>(textureBackground.getSize().x));
            men.emplace_back(manTexture, randomX, 450);
        }

        float backgroundSpeed = 0.3f;
        float backgroundWidth = textureBackground.getSize().x;
        float windowWidth = window.getSize().x;
        float centerPositionX = windowWidth / 2.0f;

        Clock clock;

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePosition(event.mouseButton.x, event.mouseButton.y);

                    for (auto& man : men) {
                        man.handleMouseClick(mousePosition);
                    }
                }
            }

            float deltaTime = clock.restart().asSeconds();
            float offsetX = 0.0f;

            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                character.changeLeft();
                if (character.getX() > centerPositionX && character.getX() > 0) {
                    character.moveLeft();
                }
                else if (spriteBackground.getPosition().x + backgroundSpeed <= 0) {
                    offsetX = backgroundSpeed;
                    spriteBackground.move(offsetX, 0);
                }
                else if (character.getX() > 0) {
                    character.moveLeft();
                }
            }

            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                character.changeRight();
                if (character.getX() < centerPositionX && character.getX() < windowWidth - 50) {
                    character.moveRight();
                }
                else if (spriteBackground.getPosition().x - backgroundSpeed >= -(backgroundWidth - windowWidth)) {
                    offsetX = -backgroundSpeed;
                    spriteBackground.move(offsetX, 0);
                }
                else if (character.getX() < windowWidth - 100) {
                    character.moveRight();
                }
            }

            float leftBoundary = spriteBackground.getPosition().x;
            float rightBoundary = spriteBackground.getPosition().x + textureBackground.getSize().x;

            for (auto& man : men) {
                man.update(deltaTime, leftBoundary, rightBoundary);
                man.moveWithBackground(offsetX);
            }

            window.clear();
            window.draw(spriteBackground);
            character.draw(window);
            for (auto& man : men) {
                man.draw(window);
            }
            window.display();
        }
    }
    catch (const exception& e) {
        cerr << "오류 발생: " << e.what() << endl;
    }

    return 0;
}