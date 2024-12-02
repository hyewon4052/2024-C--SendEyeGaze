#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "showList.h"

using namespace sf;
using namespace std;

string resultFriend;

string getRandomFriend() {
    return resultFriend;
}

int result() {

    string names[] = {
       "Gaye", "Jisu", "Jiyoung", "Jihyun", "Minsun", "Yeji", "Minseo",
       "Sua", "Hayoung", "Minkyung", "Subin", "Yeoeun", "Dayul", "Eunseo",
       "Hyunji", "Hyewon", "Gaeun", "Yeonu", "Hanim", "Jaehee", "Yebin",
       "Onyu", "Chohee", "Jiwon", "Eunse", "Hani", "Yejin", "Suin",
       "Hanseol", "Hongjun", "Seojin", "Chaeun", "Yunbi", "Nayoung", "Soyun"
    };

    // 랜덤으로 친구 이름 선택
    srand(static_cast<unsigned>(time(0)));  // 시드 설정
    int randomIndex = rand() % (sizeof(names) / sizeof(names[0]));  // 배열의 크기 범위 내에서 랜덤 인덱스 선택
    resultFriend = names[randomIndex];  // 전역 변수에 랜덤 친구 이름 할당

    // 창 생성
    VideoMode vm(1000, 700);
    RenderWindow window(vm, "Result", Style::Default);

    // 텍스트 설정
    Font font;
    if (!font.loadFromFile("fonts/나눔고딕.TTF")) {  // 폰트 로드
        cerr << "폰트 로드 실패" << endl;
        return -1;
    }

    // 배경 이미지 로드
    Texture textBackground;
    if (!textBackground.loadFromFile("graphics/textBackground.png")) {
        cerr << "배경 로드 실패" << endl;
        return -1;
    }
    Sprite backgroundSprite(textBackground);  // 이미지 스프라이트로 만들기

    // 텍스트 설정
    Text resultText;
    resultText.setFont(font);
    resultText.setCharacterSize(70);
    resultText.setFillColor(Color::Black);
    resultText.setString("My Real Best Friend is..... \n" + resultFriend + "..........!!");
    resultText.setPosition(70, 250);

    // "Next" 버튼 설정
    
    Text nextButtonText("Next", font, 24);
    nextButtonText.setPosition(800, 510);
    nextButtonText.setFillColor(Color::Black);

    RectangleShape nextButton(Vector2f(150, 50));
    nextButton.setPosition(750, 500);
    nextButton.setFillColor(Color::Transparent);
    nextButton.setOutlineThickness(3);          // 테두리 두께 설정
    nextButton.setOutlineColor(Color::Black);   // 테두리 색상 설정

    while (window.isOpen()) {

        // 화면에 배경과 텍스트 표시
        window.clear();
        window.draw(backgroundSprite);  // 배경 그리기
        window.draw(resultText);  // 텍스트 그리기
        window.draw(nextButton);  // 버튼 그리기
        window.draw(nextButtonText);  // 버튼 텍스트 그리기
        window.display();

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            // 버튼 클릭 이벤트 처리
            if (event.type == Event::MouseButtonPressed) {
                if (nextButton.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                    // "Next" 버튼 클릭 시 showList 호출
                    showList();
                    window.close();
                }
            }
        }
    }

    return 0;
}
