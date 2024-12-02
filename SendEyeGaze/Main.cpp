#include "StartScreen.h"
#include "Game.h"
#include <iostream>
using namespace std;

int main() {
    bool startSuccess = startGame();
    if (startSuccess) {
        cout << "Start 버튼이 눌렸습니다. 게임을 시작합니다." << endl;
        runGame();
    }
    else {
        cout << "게임 시작 실패: startGame()이 false를 반환했습니다." << endl;
    }
    return 0;
}
