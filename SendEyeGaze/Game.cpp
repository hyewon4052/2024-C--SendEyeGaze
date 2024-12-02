#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <set>
#include "Result.h"

using namespace sf;
using namespace std;


vector<string> newMenNames;

vector<string> getNewMenNames() {
    return newMenNames;
}

// 주인공 캐릭터
class Character {
public:
    const Sprite& getSprite() {
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
class Man {
public:
    float gaugeIncrease;  // 각 캐릭터의 게이지 증가 값

    Man(Texture& texture, float x, float y, const string& characterName)
        : speed(100.0f), directionChangeInterval(2.0f), elapsedTime(0.0f), stopped(false), gauge(100.0f), gaugeVisible(false), name(characterName) {
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);

        direction = (rand() % 2 == 0) ? -1 : 1;
        updateSpriteScale();

        if (!stoppedTexture.loadFromFile("graphics/ManStop.png")) {
            throw runtime_error("멈춤 이미지 로드 실패");
        }

        originalTexture = texture;

        // 랜덤으로 게이지 증가 값 설정 (2 ~ 17 범위)
        gaugeIncrease = static_cast<float>(rand() % 16 + 2); // 2 ~ 17 사이의 값

    }

    // 남자 캐릭터 업데이트
    void update(float deltaTime, float leftBoundary, float rightBoundary) {
        if (stopped) {
            if (stopClock.getElapsedTime().asSeconds() >= 0.2f) { // 멈춤 상태는 0.2초 후 해제
                stopped = false;
                sprite.setTexture(originalTexture); // 원래 텍스처로 복원
            }
            return;
        }

        // 게이지 0.1초마다 증가
        if (gaugeIncreaseClock.getElapsedTime().asSeconds() >= 0.1f) {
            gauge = min(100.0f, gauge + gaugeIncrease); // 0.1초마다 5만큼 증가
            gaugeIncreaseClock.restart();    // 게이지 증가 타이머 리셋
        }

        elapsedTime += deltaTime;
        gauge = max(0.0f, gauge - deltaTime * 10); // 게이지 자연 감소


        // elapsedTime : 방향 변경까지의 경과 시간
        // 방향 주기적으로 변경
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

    void setFont(const Font& f) {
        font = f;
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

    // 방향에 따라 이미지 좌우 반전 처리
    void updateSpriteScale() {
        sprite.setScale((direction == -1) ? -1.0f : 1.0f, 1.0f);
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

    string getName() const {
        return name;
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
    string name; // 캐릭터 이름
    Font font;
    float gauge; // 게이지 상태

    
};


int runGame() {

    Texture manTexture, streetBackgroundTexture;
    if (!manTexture.loadFromFile("graphics/man.png")) {
        cerr << "Man 사진 로드 실패" << endl;
        return -1;
    }
    if (!streetBackgroundTexture.loadFromFile("graphics/StreetBackground.png")) {
        cerr << "StreetBackground 사진 로드 실패" << endl;
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
        timerText.setFont(font); 

        Clock gameClock;
        const int timeLimit = 60; // 제한 시간

        Sprite spriteBackground;
        spriteBackground.setTexture(textureBackground);
        spriteBackground.setPosition(0, 0);

        // 여자 캐릭터 생성 (항상 화면 중앙에)
        Character character("graphics/girlRight.png", "graphics/girlLeft.png", 500, 280.0f);

        Clock clock;

        // 랜덤으로 주어질 남자 캐릭터들의 이름
        string names[] = {
            "Gaye", "Jisu", "Jiyoung", "Jihyun", "Minsun", "Yeji", "Minseo",
            "Sua", "Hayoung", "Minkyung", "Subin", "Yeoeun", "Dayul", "Eunseo",
            "Hyunji", "Hyewon", "Gaeun", "Yeonu", "Hanim", "Jaehee", "Yebin",
            "Onyu", "Chohee", "Jiwon", "Eunse", "Hani", "Yejin", "Suin",
            "Hanseol", "Hongjun", "Seojin", "Chaeun", "Yunbi", "Nayoung", "Soyun"
        };

        set<string> usedNames;  // 이미 사용된 이름
        vector<Man> men;

        // 난수 생성 시 시드 설정
        srand(time(0));

        auto createMen = [&]() {
            men.clear();  // 기존 남자 캐릭터 제거
            for (int i = 0; i < 5; i++) {
                string chosenName;
                do {
                    // 랜덤 인덱스를 선택 (배열의 크기 범위 내에서)
                    int randomIndex = rand() % (sizeof(names) / sizeof(names[0]));
                    chosenName = names[randomIndex];
                } while (usedNames.find(chosenName) != usedNames.end());  // 이름 중복 안되게

                usedNames.insert(chosenName);  // 사용한 이름을 집합에 추가

                float randomX = static_cast<float>(rand() % 1300); 
                men.emplace_back(manTexture, randomX, 450, chosenName); 
                cout << "남자 캐릭터 " << i + 1 << "의 이름: " << chosenName << endl;
            }
            cout << "" << endl;
        };
        createMen();

        float backgroundSpeed = 0.8f;
        float backgroundWidth = textureBackground.getSize().x;
        float windowWidth = window.getSize().x;

        bool streetBackgroundActive = false; // 배경 전환 상태

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
                result();  
                window.close();  // 게임 창을 닫음
            }

            // 텍스트 업데이트
            timerText.setString("Time: " + to_string(remainingTime) + "s");

            // 왼쪽 키보드 눌렀을 때
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                character.changeLeft();
                if (spriteBackground.getPosition().x + backgroundSpeed <= 0) {
                    offsetX = backgroundSpeed;
                    spriteBackground.move(offsetX, 0);
                }
            }

            // 오른쪽 키보드 눌렀을 때
            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                character.changeRight();
                if (spriteBackground.getPosition().x - backgroundSpeed >= -(backgroundWidth - windowWidth)) {
                    offsetX = -backgroundSpeed;
                    spriteBackground.move(offsetX, 0);
                }
                else if (spriteBackground.getPosition().x - backgroundSpeed < -(backgroundWidth - windowWidth)) {
                    // 배경 끝에 도달한 경우
                    spriteBackground.setTexture(streetBackgroundActive ? textureBackground : streetBackgroundTexture); // 배경 전환
                    spriteBackground.setPosition(0, 0); // 초기 위치로 설정
                    createMen(); // 새로운 남자 캐릭터 생성
                    streetBackgroundActive = !streetBackgroundActive; // 배경 상태 전환
                }
            }

            // 남자 캐릭터 업데이트
            float leftBoundary = spriteBackground.getPosition().x;
            float rightBoundary = spriteBackground.getPosition().x + textureBackground.getSize().x;
            for (auto& man : men) {
                man.update(deltaTime, leftBoundary, rightBoundary);
                man.moveWithBackground(offsetX);
            }

       
            vector<Man> remainingMen;  // 죽지 않은 남자 캐릭터들을 저장할 벡터

            for (auto& man : men) {
                if (man.getGauge() <= 0.0f) {
                    cout << "내 새로운 친구 : " << man.getName() << endl;
                    newMenNames.push_back(man.getName());
                }
                else {
                    // 게이지가 0 이하가 아니면 남자 캐릭터 유지
                    remainingMen.push_back(man);
                }
            }

            // men 벡터를 남은 캐릭터들로 업데이트
            men = remainingMen;


            window.clear();
            window.draw(spriteBackground);
            character.draw(window);

            // 남은 시간 표시
            window.draw(timerText);

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