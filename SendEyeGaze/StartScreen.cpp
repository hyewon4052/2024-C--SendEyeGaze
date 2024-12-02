#include "StartScreen.h"

using namespace sf;

bool startGame() {
    RenderWindow window(VideoMode(1000, 700), "Start Screen");

    // ��� �̹��� �ε�
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("graphics/StartBackground.png")) {
        return false;
    }
    Sprite backgroundSprite(backgroundTexture);

    // Start ��ư ����
    
    RectangleShape startButton(Vector2f(150, 50));
    startButton.setPosition(525, 220);

    Font font;
    if (!font.loadFromFile("fonts/Pretendard-Bold.otf")) {
        return false;
    }

    Text startText("Start", font, 24);
    startText.setFillColor(Color::Black);
    startText.setPosition(575, 230); // ��ư ���ο� �ؽ�Ʈ�� ��ġ
    startButton.setFillColor(Color::Transparent);
    startButton.setOutlineThickness(3);          // �׵θ� �β� ����
    startButton.setOutlineColor(Color::Black);   // �׵θ� ���� ����

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                if (startButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    return true; // Start ��ư Ŭ��
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
