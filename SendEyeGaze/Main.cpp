#include "StartScreen.h"
#include "Game.h"
#include <iostream>
using namespace std;

int main() {
    bool startSuccess = startGame();
    if (startSuccess) {
        cout << "Start ��ư�� ���Ƚ��ϴ�. ������ �����մϴ�." << endl;
        runGame();
    }
    else {
        cout << "���� ���� ����: startGame()�� false�� ��ȯ�߽��ϴ�." << endl;
    }
    return 0;
}
