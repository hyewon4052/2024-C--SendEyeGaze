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
    const Sprite& getSprite() const {
        return sprite;
    }

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

    // 캐릭터를 왼쪽으로
    void moveLeft() {
        sprite.move(-0.2f, 0);
    }

    // 캐릭터가 오른쪽을 보게
    void changeRight() {
        sprite.setTexture(textureRight);
    }

    // 캐릭터를 오른쪽으로
    void moveRight() {
        sprite.move(0.2f, 0);
    }

    // 캐릭터 화면에 그리기
    void draw(RenderWindow& window) {
        window.draw(sprite);
    }

    // 캐릭터의 현재 x 좌표
    float getX() const {
        return sprite.getPosition().x;
    }

    // 캐릭터의 위치 설정
    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

private:
    Texture textureRight;  // 오른쪽 방향 이미지
    Texture textureLeft;   // 왼쪽 방향 이미지
    Sprite sprite;
};

// 남자 캐릭터
// 남자 캐릭터
class Man {
public:
    Man(Texture& texture, float x, float y, Texture& diedTexture)
        : speed(100.0f), directionChangeInterval(2.0f), elapsedTime(0.0f), stopped(false), gauge(100.0f), gaugeVisible(false) {
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);

        direction = (rand() % 2 == 0) ? -1 : 1;
        updateSpriteScale();

        if (!stoppedTexture.loadFromFile("graphics/ManStop.png")) {
            throw runtime_error("멈춤 이미지 로드 실패");
        }

        originalTexture = texture;
    }

    // 남자 캐릭터 업데이트
    void update(float deltaTime, float leftBoundary, float rightBoundary, vector<Sprite>& diedSprites, const Sprite& girlSprite) {
        if (stopped) {
            if (stopClock.getElapsedTime().asSeconds() >= 0.2f) { // 멈춤 상태는 0.2초 후 해제
                stopped = false;
                sprite.setTexture(originalTexture); // 원래 텍스처로 복원
            }
            return;
        }

        // 게이지 0.1초마다 증가
        if (gaugeIncreaseClock.getElapsedTime().asSeconds() >= 0.1f) {
            gauge = min(100.0f, gauge + 5.0f); // 0.1초마다 5만큼 증가
            gaugeIncreaseClock.restart();    // 게이지 증가 타이머 리셋
        }

        elapsedTime += deltaTime;
        gauge = max(0.0f, gauge - deltaTime * 10); // 게이지 자연 감소

        // 방향 주기적으로 변경 (랜덤)
        if (elapsedTime >= directionChangeInterval) {
            direction = (rand() % 2 == 0) ? -1 : 1;
            updateSpriteScale();
            elapsedTime = 0.0f;
        }

        float currentX = sprite.getPosition().x;
        sprite.move(speed * direction * deltaTime, 0);

        // 경계를 벗어나면 방향 반전
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

    // 배경 이동에 따라 위치 변경
    void moveWithBackground(float offsetX) {
        sprite.move(offsetX, 0); // 배경 이동량만큼 스프라이트 이동
    }

    // 남자 캐릭터 화면에 표시
    void draw(RenderWindow& window) {
        window.draw(sprite);

        // 게이지가 활성화된 경우에만 표시
        if (gaugeVisible) {
            RectangleShape gaugeBar(Vector2f(100.0f, 10.0f));
            gaugeBar.setFillColor(Color::White);
            gaugeBar.setPosition(sprite.getPosition().x - 50, sprite.getPosition().y - 130);

            RectangleShape gaugeFill(Vector2f(gauge, 10.0f)); // 게이지 크기 조정
            gaugeFill.setFillColor(Color::Red);
            gaugeFill.setPosition(sprite.getPosition().x - 50, sprite.getPosition().y - 130);

            window.draw(gaugeBar);
            window.draw(gaugeFill);
        }
    }

    // 남자 캐릭터 클릭 시
    void handleMouseClick(const Vector2f& mousePosition) {
        if (sprite.getGlobalBounds().contains(mousePosition)) {
            stopped = true; // 멈춘 상태로 전환
            sprite.setTexture(stoppedTexture); // 멈춤 이미지로 변경
            stopClock.restart(); // 멈춤 상태 지속 시간 타이머 리셋
            gauge = max(0.0f, gauge - 3.0f); // 클릭 시 게이지 감소량을 5로 설정

            // 클릭 시 게이지 활성화
            gaugeVisible = true;
        }
    }

    float getGauge() const {
        return gauge;
    }

private:
    Sprite sprite;
    Texture originalTexture;
    Texture stoppedTexture;
    float speed;                // 이동 속도
    int direction;              // 이동 방향 (-1: 왼쪽, 1: 오른쪽)
    float directionChangeInterval; // 방향 변경 간격 (초 단위)
    float elapsedTime;          // 방향 변경까지의 경과 시간
    bool stopped;               // 멈춘 상태 여부
    bool gaugeVisible;          // 게이지 활성화 여부
    Clock stopClock;            // 멈춘 상태 지속 시간 측정
    Clock gaugeIncreaseClock;
    float gauge; // 게이지 상태

    // 방향에 따라 이미지 좌우 반전 처리
    void updateSpriteScale() {
        sprite.setScale((direction == -1) ? -1.0f : 1.0f, 1.0f);
    }
};

int runGame() {
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
        Texture diedTexture;
        if (!diedTexture.loadFromFile("graphics/ManDied.png")) {
            cerr << "ManDied 사진 로드 실패" << endl;
            return -1;
        }

        // 텍스트 설정
        Text timerText;
        timerText.setCharacterSize(24);
        timerText.setFillColor(Color::White);
        timerText.setPosition(10.0f, 10.0f);

        Font font;
        if (!font.loadFromFile("fonts/arial.ttf")) { // 폰트 파일 경로
            cerr << "폰트 로드 실패" << endl;
            return -1;
        }
        timerText.setFont(font); // 텍스트에 폰트 설정

        Clock gameClock; // 게임 시작 시각
        const int timeLimit = 60; // 제한 시간(초)
            
        Sprite spriteBackground;
        spriteBackground.setTexture(textureBackground);
        spriteBackground.setPosition(0, 0);

        // 여자 캐릭터 생성
        Character character("graphics/girlRight.png", "graphics/girlLeft.png", 500, 280.0f);

        // 남자 캐릭터 랜덤 위치에 생성
        vector<Man> men;
        vector<Sprite> diedSprites; // 사망한 캐릭터 저장
        for (int i = 0; i < 5; i++) {
            float randomX = static_cast<float>(rand() % static_cast<int>(textureBackground.getSize().x));
            men.emplace_back(manTexture, randomX, 450, diedTexture); // diedTexture 추가
        }

        float backgroundSpeed = 0.3f;
        float backgroundWidth = textureBackground.getSize().x;
        float windowWidth = window.getSize().x;
        float centerPositionX = windowWidth / 2.0f; // 화면 중앙 위치

        Clock clock;    // 시간 측정용 SFML Clock

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

            // 남은 시간 계산
            float elapsedTime = gameClock.getElapsedTime().asSeconds();
            int remainingTime = max(0, timeLimit - static_cast<int>(elapsedTime));

            // 남은 시간이 0이면 게임 종료
            if (remainingTime <= 0) {
                window.close();
            }

            // 텍스트 업데이트
            timerText.setString("Time: " + to_string(remainingTime) + "s");

            // 왼쪽 키보드 눌렀을 때
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

            // 오른쪽 키보드 눌렀을 때
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
                man.update(deltaTime, leftBoundary, rightBoundary, diedSprites, character.getSprite());
                man.moveWithBackground(offsetX);
            }

            // 리스트에서 사망한 캐릭터 제거
            men.erase(
                remove_if(men.begin(), men.end(), [](const Man& man) { return man.getGauge() <= 0.0f; }),
                men.end()
            );

            window.clear();
            window.draw(spriteBackground);
            character.draw(window);

            // 남은 시간 표시
            window.draw(timerText);

            for (auto& sprite : diedSprites) {
                window.draw(sprite);
            }

            for (auto& man : men) {
                man.draw(window);
            }
            window.display();
        }
        return 0;
    }
    catch (const exception& e) {
        cerr << "오류 발생: " << e.what() << endl;
    }

    return -1;
}