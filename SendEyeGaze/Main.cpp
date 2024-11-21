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

    // ĳ���� ��ġ ��������
    void moveLeft() {
        sprite.move(-0.2f, 0);
    }

    // ĳ���Ͱ� �������� ����
    void changeRight() {
        sprite.setTexture(textureRight);
    }

    // ĳ���� ��ġ ����������
    void moveRight() {
        sprite.move(0.2f, 0);
    }

    // ĳ���� ȭ�鿡 ǥ��
    void draw(RenderWindow& window) {
        window.draw(sprite);
    }

    // ĳ������ ���� x ��ǥ
    float getX() const {
        return sprite.getPosition().x;
    }

    // ĳ���� ��ġ
    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

private:
    Texture textureRight;  // ������ ����
    Texture textureLeft;   // ���� ����
    Sprite sprite;         
};

// ���� ĳ����
class Man {
public:
    Man(Texture& texture, float x, float y) : speed(100.0f), directionChangeInterval(2.0f), elapsedTime(0.0f) {
        sprite.setTexture(texture);
        sprite.setPosition(x, y);

        // �ʱ� ���� ���� (-1: ����, 1: ������) -> ���� ĳ���Ͱ� �������� �����̰�
        direction = (rand() % 2 == 0) ? -1 : 1;
    }

    // ���� ĳ���� �̵�
    void update(float deltaTime, float leftBoundary, float rightBoundary) {
        elapsedTime += deltaTime;

        // ���� �ֱ������� ����
        if (elapsedTime >= directionChangeInterval) {
            direction = (rand() % 2 == 0) ? -1 : 1;  // �����ϰ� ���� ����
            elapsedTime = 0.0f;                      // Ÿ�̸� �ʱ�ȭ
        }

        float currentX = sprite.getPosition().x;

        // ���� �������� �̵�
        sprite.move(speed * direction * deltaTime, 0);

        // ��踦 ����� ���� ����
        if (currentX < leftBoundary) {
            sprite.setPosition(leftBoundary, sprite.getPosition().y);
            direction = 1;  // ���������� �̵�
        }
        else if (currentX > rightBoundary) {
            sprite.setPosition(rightBoundary, sprite.getPosition().y);
            direction = -1; // �������� �̵�
        }
    }

    // ��� �̵��� ���� ��ġ ���� (��濡 ������ ��ó�� ���̵���)
    void moveWithBackground(float offsetX) {
        sprite.move(offsetX, 0);
    }

    // ���� ĳ���͸� ȭ�鿡 ǥ��
    void draw(RenderWindow& window) {
        window.draw(sprite);
    }

private:
    Sprite sprite;              
    float speed;                // �̵� �ӵ�
    int direction;              // �̵� ���� (-1: ����, 1: ������)
    float directionChangeInterval; // ���� ���� ���� (�� ����)
    float elapsedTime;          // ���� ��������� ��� �ð�
};

int main() {

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

        Sprite spriteBackground;
        spriteBackground.setTexture(textureBackground);
        spriteBackground.setPosition(0, 0);

        // ���� ĳ���� ����
        Character character("graphics/girlRight.png", "graphics/girlLeft.png", 500, 280);

        // ���� ĳ���� ���� ��ġ�� ����
        vector<Man> men;
        for (int i = 0; i < 5; i++) {
            float randomX = static_cast<float>(rand() % static_cast<int>(textureBackground.getSize().x)); // ��� �������� ���� ��ġ
            men.emplace_back(manTexture, randomX, 350);
        }

        float backgroundSpeed = 0.3f;                // ��� �̵� �ӵ�
        float backgroundWidth = textureBackground.getSize().x; // ��� �ʺ�
        float windowWidth = window.getSize().x;     // ������ �ʺ�
        float centerPositionX = windowWidth / 2.0f; // ȭ�� �߾� ��ġ

        Clock clock; // �ð� ������ SFML Clock

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
            }

            float deltaTime = clock.restart().asSeconds(); // ������ �� ��� �ð�
            float offsetX = 0.0f;                          // ��� �̵���

            // ���� ����Ű �Է�
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                character.changeLeft();
                if (character.getX() > centerPositionX && character.getX() > 0) {
                    character.moveLeft();
                }
                else if (spriteBackground.getPosition().x + backgroundSpeed <= 0) {
                    offsetX = backgroundSpeed;  // ��� �������� �̵�
                    spriteBackground.move(offsetX, 0);
                }
                else if (character.getX() > 0) {
                    character.moveLeft();
                }
            }

            // ������ ����Ű �Է�
            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                character.changeRight();
                if (character.getX() < centerPositionX && character.getX() < windowWidth - 50) {
                    character.moveRight();
                }
                else if (spriteBackground.getPosition().x - backgroundSpeed >= -(backgroundWidth - windowWidth)) {
                    offsetX = -backgroundSpeed; // ��� ���������� �̵�
                    spriteBackground.move(offsetX, 0);
                }
                else if (character.getX() < windowWidth - 100) {
                    character.moveRight();
                }
            }

            // ���� ĳ������ �̵� ���� ���� (��� ������ ����)
            float leftBoundary = spriteBackground.getPosition().x;
            float rightBoundary = spriteBackground.getPosition().x + textureBackground.getSize().x;

            // ���� ĳ���� ������Ʈ
            for (auto& man : men) {
                man.update(deltaTime, leftBoundary, rightBoundary);
                man.moveWithBackground(offsetX);
            }

            // ȭ�� �׸���
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
        cerr << "���� �߻�: " << e.what() << endl;
    }

    return 0;
}
