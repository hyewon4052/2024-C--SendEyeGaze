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

    // 캐릭터가 왼쪽을 보게
    void changeLeft() {
        sprite.setTexture(textureLeft);
    }

    // 캐릭터 위치 왼쪽으로
    void moveLeft() {
        sprite.move(-0.2f, 0);
    }

    // 캐릭터가 오른쪽을 보게
    void changeRight() {
        sprite.setTexture(textureRight);
    }

    // 캐릭터 위치 오른쪽으로
    void moveRight() {
        sprite.move(0.2f, 0);
    }

    // 캐릭터 화면에 표시
    void draw(RenderWindow& window) {
        window.draw(sprite);
    }

    // 캐릭터의 현재 x 좌표
    float getX() const {
        return sprite.getPosition().x;
    }

    // 캐릭터 위치
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
    Man(Texture& texture, float x, float y) : speed(100.0f), directionChangeInterval(2.0f), elapsedTime(0.0f) {
        sprite.setTexture(texture);
        sprite.setPosition(x, y);

        // 초기 방향 설정 (-1: 왼쪽, 1: 오른쪽) -> 남자 캐릭터가 랜덤으로 움직이게
        direction = (rand() % 2 == 0) ? -1 : 1;
    }

    // 남자 캐릭터 이동
    void update(float deltaTime, float leftBoundary, float rightBoundary) {
        elapsedTime += deltaTime;

        // 방향 주기적으로 변경
        if (elapsedTime >= directionChangeInterval) {
            direction = (rand() % 2 == 0) ? -1 : 1;  // 랜덤하게 방향 설정
            elapsedTime = 0.0f;                      // 타이머 초기화
        }

        float currentX = sprite.getPosition().x;

        // 현재 방향으로 이동
        sprite.move(speed * direction * deltaTime, 0);

        // 경계를 벗어나면 방향 반전
        if (currentX < leftBoundary) {
            sprite.setPosition(leftBoundary, sprite.getPosition().y);
            direction = 1;  // 오른쪽으로 이동
        }
        else if (currentX > rightBoundary) {
            sprite.setPosition(rightBoundary, sprite.getPosition().y);
            direction = -1; // 왼쪽으로 이동
        }
    }

    // 배경 이동에 따라 위치 변경 (배경에 고정된 것처럼 보이도록)
    void moveWithBackground(float offsetX) {
        sprite.move(offsetX, 0);
    }

    // 남자 캐릭터를 화면에 표시
    void draw(RenderWindow& window) {
        window.draw(sprite);
    }

private:
    Sprite sprite;              
    float speed;                // 이동 속도
    int direction;              // 이동 방향 (-1: 왼쪽, 1: 오른쪽)
    float directionChangeInterval; // 방향 변경 간격 (초 단위)
    float elapsedTime;          // 방향 변경까지의 경과 시간
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

        // 여자 캐릭터 생성
        Character character("graphics/girlRight.png", "graphics/girlLeft.png", 500, 280);

        // 남자 캐릭터 랜덤 위치에 생성
        vector<Man> men;
        for (int i = 0; i < 5; i++) {
            float randomX = static_cast<float>(rand() % static_cast<int>(textureBackground.getSize().x)); // 배경 범위에서 랜덤 위치
            men.emplace_back(manTexture, randomX, 350);
        }

        float backgroundSpeed = 0.3f;                // 배경 이동 속도
        float backgroundWidth = textureBackground.getSize().x; // 배경 너비
        float windowWidth = window.getSize().x;     // 윈도우 너비
        float centerPositionX = windowWidth / 2.0f; // 화면 중앙 위치

        Clock clock; // 시간 측정용 SFML Clock

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
            }

            float deltaTime = clock.restart().asSeconds(); // 프레임 간 경과 시간
            float offsetX = 0.0f;                          // 배경 이동량

            // 왼쪽 방향키 입력
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                character.changeLeft();
                if (character.getX() > centerPositionX && character.getX() > 0) {
                    character.moveLeft();
                }
                else if (spriteBackground.getPosition().x + backgroundSpeed <= 0) {
                    offsetX = backgroundSpeed;  // 배경 왼쪽으로 이동
                    spriteBackground.move(offsetX, 0);
                }
                else if (character.getX() > 0) {
                    character.moveLeft();
                }
            }

            // 오른쪽 방향키 입력
            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                character.changeRight();
                if (character.getX() < centerPositionX && character.getX() < windowWidth - 50) {
                    character.moveRight();
                }
                else if (spriteBackground.getPosition().x - backgroundSpeed >= -(backgroundWidth - windowWidth)) {
                    offsetX = -backgroundSpeed; // 배경 오른쪽으로 이동
                    spriteBackground.move(offsetX, 0);
                }
                else if (character.getX() < windowWidth - 100) {
                    character.moveRight();
                }
            }

            // 남자 캐릭터의 이동 범위 설정 (배경 범위에 맞춤)
            float leftBoundary = spriteBackground.getPosition().x;
            float rightBoundary = spriteBackground.getPosition().x + textureBackground.getSize().x;

            // 남자 캐릭터 업데이트
            for (auto& man : men) {
                man.update(deltaTime, leftBoundary, rightBoundary);
                man.moveWithBackground(offsetX);
            }

            // 화면 그리기
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
