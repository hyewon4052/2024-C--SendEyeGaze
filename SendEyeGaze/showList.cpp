#include "Game.h"
#include "Result.h"
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

int showList() {
    string resultFriend = getRandomFriend();  // 한 번만 호출하여 랜덤 친구 이름을 설정
    cout << "Random Friend: " << resultFriend << endl;  // 디버깅: 랜덤 친구 확인

    // 창 생성
    VideoMode vm(1000, 700);
    RenderWindow window(vm, "Result", Style::Default);

    // 배경 이미지 로드
    Texture textBackground;
    if (!textBackground.loadFromFile("graphics/textBackground.png")) {
        cerr << "배경 로드 실패" << endl;
        return -1;
    }
    Sprite backgroundSprite(textBackground);  // 이미지 스프라이트로 만들기

    // newMenNames를 가져오는 함수 호출
    vector<string> newMenNames = getNewMenNames();

    // 디버깅: newMenNames 확인
    cout << "New Men Names: ";
    for (const auto& name : newMenNames) {
        cout << name << " ";
    }
    cout << endl;

    // My Friend List 출력
    string friendList = "My Friend List: \n";
    for (size_t i = 0; i < newMenNames.size(); ++i) {
        friendList += newMenNames[i];
        if (i < newMenNames.size() - 1) {
            friendList += "\n";  // 친구 이름들 사이에 쉼표 추가
        }
    }

    // 텍스트 설정
    Font font;
    if (!font.loadFromFile("fonts/나눔고딕.TTF")) {  // 폰트 로드
        cerr << "폰트 로드 실패" << endl;
        return -1;
    }

    Text friendListText;
    friendListText.setFont(font);
    friendListText.setCharacterSize(30);
    friendListText.setFillColor(Color::Black);
    friendListText.setString(friendList);
    friendListText.setPosition(20, 110);

    // 랜덤 친구 이름 메시지
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
        window.draw(backgroundSprite);  // 배경 그리기
        window.draw(friendListText);    // 친구 목록 텍스트 그리기
        window.draw(messageText);       // 메시지 텍스트 그리기
        window.display();
    }

    return 0;
}
