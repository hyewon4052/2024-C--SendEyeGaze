#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace sf;
using namespace std;

// ���ΰ� ĳ����
class Character {
public:
    const Sprite& getSprite() const {
        return sprite;
    }

    Character(const string& textureRightFile, const string& textureLeftFile, float x, float y) {
        if (!textureRight.loadFromFile(textureRightFile)) {
            throw runtime_error("������ ���� �ε� ����");
        }
        if (!textureLeft.loadFromFile(textureLeftFile)) {
            throw runtime_error("���� ���� �ε� ����");
        }

        sprite.setTexture(textureRight);  // �ʱ⿡�� �������� ���� �̹��� ���
        sprite.setPosition(x, y);         // �ʱ� ��ġ ����
    }

    // ĳ���Ͱ� ������ ����
    void changeLeft() {
        sprite.setTexture(textureLeft);
    }

    // ĳ���͸� ��������
    void moveLeft() {
        sprite.move(-0.2f, 0);
    }

    // ĳ���Ͱ� �������� ����
    void changeRight() {
        sprite.setTexture(textureRight);
    }

    // ĳ���͸� ����������
    void moveRight() {
        sprite.move(0.2f, 0);
    }

    // ĳ���� ȭ�鿡 �׸���
    void draw(RenderWindow& window) {
        window.draw(sprite);
    }

    // ĳ������ ���� x ��ǥ
    float getX() const {
        return sprite.getPosition().x;
    }

    // ĳ������ ��ġ ����
    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

private:
    Texture textureRight;  // ������ ���� �̹���
    Texture textureLeft;   // ���� ���� �̹���
    Sprite sprite;
};

// ���� ĳ����
// ���� ĳ����
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
            throw runtime_error("���� �̹��� �ε� ����");
        }

        originalTexture = texture;
    }

    // ���� ĳ���� ������Ʈ
    void update(float deltaTime, float leftBoundary, float rightBoundary, vector<Sprite>& diedSprites, const Sprite& girlSprite) {
        if (stopped) {
            if (stopClock.getElapsedTime().asSeconds() >= 0.2f) { // ���� ���´� 0.2�� �� ����
                stopped = false;
                sprite.setTexture(originalTexture); // ���� �ؽ�ó�� ����
            }
            return;
        }

        // ������ 0.1�ʸ��� ����
        if (gaugeIncreaseClock.getElapsedTime().asSeconds() >= 0.1f) {
            gauge = min(100.0f, gauge + 5.0f); // 0.1�ʸ��� 5��ŭ ����
            gaugeIncreaseClock.restart();    // ������ ���� Ÿ�̸� ����
        }

        elapsedTime += deltaTime;
        gauge = max(0.0f, gauge - deltaTime * 10); // ������ �ڿ� ����

        // ���� �ֱ������� ���� (����)
        if (elapsedTime >= directionChangeInterval) {
            direction = (rand() % 2 == 0) ? -1 : 1;
            updateSpriteScale();
            elapsedTime = 0.0f;
        }

        float currentX = sprite.getPosition().x;
        sprite.move(speed * direction * deltaTime, 0);

        // ��踦 ����� ���� ����
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

    // ��� �̵��� ���� ��ġ ����
    void moveWithBackground(float offsetX) {
        sprite.move(offsetX, 0); // ��� �̵�����ŭ ��������Ʈ �̵�
    }

    // ���� ĳ���� ȭ�鿡 ǥ��
    void draw(RenderWindow& window) {
        window.draw(sprite);

        // �������� Ȱ��ȭ�� ��쿡�� ǥ��
        if (gaugeVisible) {
            RectangleShape gaugeBar(Vector2f(100.0f, 10.0f));
            gaugeBar.setFillColor(Color::White);
            gaugeBar.setPosition(sprite.getPosition().x - 50, sprite.getPosition().y - 130);

            RectangleShape gaugeFill(Vector2f(gauge, 10.0f)); // ������ ũ�� ����
            gaugeFill.setFillColor(Color::Red);
            gaugeFill.setPosition(sprite.getPosition().x - 50, sprite.getPosition().y - 130);

            window.draw(gaugeBar);
            window.draw(gaugeFill);
        }
    }

    // ���� ĳ���� Ŭ�� ��
    void handleMouseClick(const Vector2f& mousePosition) {
        if (sprite.getGlobalBounds().contains(mousePosition)) {
            stopped = true; // ���� ���·� ��ȯ
            sprite.setTexture(stoppedTexture); // ���� �̹����� ����
            stopClock.restart(); // ���� ���� ���� �ð� Ÿ�̸� ����
            gauge = max(0.0f, gauge - 3.0f); // Ŭ�� �� ������ ���ҷ��� 5�� ����

            // Ŭ�� �� ������ Ȱ��ȭ
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
    float speed;                // �̵� �ӵ�
    int direction;              // �̵� ���� (-1: ����, 1: ������)
    float directionChangeInterval; // ���� ���� ���� (�� ����)
    float elapsedTime;          // ���� ��������� ��� �ð�
    bool stopped;               // ���� ���� ����
    bool gaugeVisible;          // ������ Ȱ��ȭ ����
    Clock stopClock;            // ���� ���� ���� �ð� ����
    Clock gaugeIncreaseClock;
    float gauge; // ������ ����

    // ���⿡ ���� �̹��� �¿� ���� ó��
    void updateSpriteScale() {
        sprite.setScale((direction == -1) ? -1.0f : 1.0f, 1.0f);
    }
};

int runGame() {
    Texture manTexture;
    if (!manTexture.loadFromFile("graphics/man.png")) {
        cerr << "Man ���� �ε� ����" << endl;
        return -1;
    }

    try {
        srand(static_cast<unsigned>(time(0)));

        VideoMode vm(1000, 700);
        RenderWindow window(vm, "SendEyeGaze", Style::Default);

        Texture textureBackground;
        if (!textureBackground.loadFromFile("graphics/background.png")) {
            throw runtime_error("��� ���� �ε� ����");
        }
        Texture diedTexture;
        if (!diedTexture.loadFromFile("graphics/ManDied.png")) {
            cerr << "ManDied ���� �ε� ����" << endl;
            return -1;
        }

        // �ؽ�Ʈ ����
        Text timerText;
        timerText.setCharacterSize(24);
        timerText.setFillColor(Color::White);
        timerText.setPosition(10.0f, 10.0f);

        Font font;
        if (!font.loadFromFile("fonts/arial.ttf")) { // ��Ʈ ���� ���
            cerr << "��Ʈ �ε� ����" << endl;
            return -1;
        }
        timerText.setFont(font); // �ؽ�Ʈ�� ��Ʈ ����

        Clock gameClock; // ���� ���� �ð�
        const int timeLimit = 60; // ���� �ð�(��)
            
        Sprite spriteBackground;
        spriteBackground.setTexture(textureBackground);
        spriteBackground.setPosition(0, 0);

        // ���� ĳ���� ����
        Character character("graphics/girlRight.png", "graphics/girlLeft.png", 500, 280.0f);

        // ���� ĳ���� ���� ��ġ�� ����
        vector<Man> men;
        vector<Sprite> diedSprites; // ����� ĳ���� ����
        for (int i = 0; i < 5; i++) {
            float randomX = static_cast<float>(rand() % static_cast<int>(textureBackground.getSize().x));
            men.emplace_back(manTexture, randomX, 450, diedTexture); // diedTexture �߰�
        }

        float backgroundSpeed = 0.3f;
        float backgroundWidth = textureBackground.getSize().x;
        float windowWidth = window.getSize().x;
        float centerPositionX = windowWidth / 2.0f; // ȭ�� �߾� ��ġ

        Clock clock;    // �ð� ������ SFML Clock

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

            // ���� �ð� ���
            float elapsedTime = gameClock.getElapsedTime().asSeconds();
            int remainingTime = max(0, timeLimit - static_cast<int>(elapsedTime));

            // ���� �ð��� 0�̸� ���� ����
            if (remainingTime <= 0) {
                window.close();
            }

            // �ؽ�Ʈ ������Ʈ
            timerText.setString("Time: " + to_string(remainingTime) + "s");

            // ���� Ű���� ������ ��
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

            // ������ Ű���� ������ ��
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

            // ����Ʈ���� ����� ĳ���� ����
            men.erase(
                remove_if(men.begin(), men.end(), [](const Man& man) { return man.getGauge() <= 0.0f; }),
                men.end()
            );

            window.clear();
            window.draw(spriteBackground);
            character.draw(window);

            // ���� �ð� ǥ��
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
        cerr << "���� �߻�: " << e.what() << endl;
    }

    return -1;
}