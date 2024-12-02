#include "StartScreen.h"

using namespace sf;

bool startGame() {
    RenderWindow window(VideoMode(1000, 700), "Start Screen");

    // 배경 이미지 로드
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("graphics/StartBackground.png")) {
        return false;
    }
    Sprite backgroundSprite(backgroundTexture);

    // Start 버튼 생성
    
    RectangleShape startButton(Vector2f(150, 50));
    startButton.setPosition(525, 220);

    Font font;
    if (!font.loadFromFile("fonts/Pretendard-Bold.otf")) {
        return false;
    }

    Text startText("Start", font, 24);
    startText.setFillColor(Color::Black);
    startText.setPosition(575, 230); // 버튼 내부에 텍스트를 배치
    startButton.setFillColor(Color::Transparent);
    startButton.setOutlineThickness(3);          // 테두리 두께 설정
    startButton.setOutlineColor(Color::Black);   // 테두리 색상 설정

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                if (startButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    return true; // Start 버튼 클릭
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(startButton);
        window.draw(startText);
        window.display();
    }
    return false;
}
