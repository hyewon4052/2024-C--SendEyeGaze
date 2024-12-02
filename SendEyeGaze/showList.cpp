#include "Game.h"
#include "Result.h"
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

int showList() {
    string resultFriend = getRandomFriend();  // �� ���� ȣ���Ͽ� ���� ģ�� �̸��� ����
    cout << "Random Friend: " << resultFriend << endl;  // �����: ���� ģ�� Ȯ��

    // â ����
    VideoMode vm(1000, 700);
    RenderWindow window(vm, "Result", Style::Default);

    // ��� �̹��� �ε�
    Texture textBackground;
    if (!textBackground.loadFromFile("graphics/textBackground.png")) {
        cerr << "��� �ε� ����" << endl;
        return -1;
    }
    Sprite backgroundSprite(textBackground);  // �̹��� ��������Ʈ�� �����

    // newMenNames�� �������� �Լ� ȣ��
    vector<string> newMenNames = getNewMenNames();

    // �����: newMenNames Ȯ��
    cout << "New Men Names: ";
    for (const auto& name : newMenNames) {
        cout << name << " ";
    }
    cout << endl;

    // My Friend List ���
    string friendList = "My Friend List: \n";
    for (size_t i = 0; i < newMenNames.size(); ++i) {
        friendList += newMenNames[i];
        if (i < newMenNames.size() - 1) {
            friendList += "\n";  // ģ�� �̸��� ���̿� ��ǥ �߰�
        }
    }

    // �ؽ�Ʈ ����
    Font font;
    if (!font.loadFromFile("fonts/�������.TTF")) {  // ��Ʈ �ε�
        cerr << "��Ʈ �ε� ����" << endl;
        return -1;
    }

    Text friendListText;
    friendListText.setFont(font);
    friendListText.setCharacterSize(30);
    friendListText.setFillColor(Color::Black);
    friendListText.setString(friendList);
    friendListText.setPosition(20, 110);

    // ���� ģ�� �̸� �޽���
    string message;
    if (find(newMenNames.begin(), newMenNames.end(), resultFriend) != newMenNames.end()) {
        message = "WOW~~ " + resultFriend + " is My Friend~~!!";
    }
    else {
        message = "MMMM...... Who are You.....?";
    }

    Text messageText;
    messageText.setFont(font);
    messageText.setCharacterSize(40);
    messageText.setFillColor(Color::Black);
    messageText.setString(message);
    messageText.setPosition(370, 315);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(backgroundSprite);  // ��� �׸���
        window.draw(friendListText);    // ģ�� ��� �ؽ�Ʈ �׸���
        window.draw(messageText);       // �޽��� �ؽ�Ʈ �׸���
        window.display();
    }

    return 0;
}
