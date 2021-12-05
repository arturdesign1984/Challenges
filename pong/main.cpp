#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <thread>

//#include <vector>

using namespace std;

void boardPosition(int, char*);
void ballPosition(int*, char*);
void buildField(char*);
void printFrame(char*);
int changeBoardPosition(int, int);
int changeBallPosition(int*, char*);

int8_t delayBall = 0;
int32_t score = 0;
int16_t countUpScore = 0;
int16_t fail = 0;
int8_t speed = 1;
int8_t moveBoard = 0;
int16_t h = 30;
int16_t w = 80;
int16_t hw = h*w;

int main()
{
    // задаем размер консоли
    system("MODE CON COLS=80 LINES=33");

    // скрываем курсор
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cci);
    cci.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cci);

    // создаем поле игры
    char* gameField;
    gameField = new char[hw];

    // заполняем поле рамкой и выводим клавиши управления

    cout << " Controls: Left: LEFT | Right: RIGHT | Exit: ESCAPE | Press Enter to start. " <<endl;

    buildField(gameField);

    // ставим доску в стартовую позицию

    int defaultBoardPosX = w/2-4;
    int boardPosX = defaultBoardPosX;
    int* bpx = &boardPosX;
    boardPosition(boardPosX, gameField);

    // савим шар в стартовую позицию

    int defaultBallPositionXY[2] = {(defaultBoardPosX+5),(h-3)*2+1};
    int ballPosXY[2] = {defaultBallPositionXY[0], defaultBallPositionXY[1]};
    int* bpxy = ballPosXY;
    ballPosition(bpxy, gameField);

    // выводим все на экран
    printFrame(gameField);

    // ждем Enter

    cin.get();

    delayBall = speed;

    while(1){

        cout << " Controls: Left: LEFT | Right: RIGHT | Exit: ESCAPE | Score: "
             << score << endl;
        buildField(gameField);

        if(moveBoard == -1 || moveBoard == 1){

            boardPosX = changeBoardPosition(boardPosX, moveBoard);
            moveBoard = 0;
        }

        boardPosition(boardPosX, gameField);

        delayBall++;
        if(delayBall == 4){
            countUpScore = changeBallPosition(ballPosXY, gameField);
            delayBall = speed;
        }

        switch (score) {

        case 30: speed = 2;
            break;
        case 60: speed = 3;
            break;
        default: ;
            break;
        }

        score = countUpScore*10;

        if(ballPosXY[1] == (h-2)*2+1){
            fail++;
            if(fail > 0){
                system("cls");
                cout << " Controls: Left: LEFT | Right: RIGHT | Exit: ESCAPE | Score: "
                     << score << endl;
                cout << "\n\n\n\n\n\t\t\tGAME OVER" << endl;
                break;

            }
        }


        ballPosition(bpxy, gameField);

        printFrame(gameField);


        if(GetAsyncKeyState(VK_LEFT)) {
            moveBoard = -1;
        }else if (GetAsyncKeyState(VK_RIGHT)){
            moveBoard = 1;
        }else if (GetAsyncKeyState(VK_ESCAPE)){
            break;
        }
        /*   код ниже работает экстрамедленно как в текстовом редакторе
       // if(kbhit()){

            char getKey = getch();
            if(getKey == myConontrols->left){
                moveBoard = -1;
            }else if(getKey == myConontrols->right){
                moveBoard = 1;
            }else if (getKey == 'q'){
                break;
            }
        }
        */
        // int currentTime = GetTickCount();
        std::this_thread::sleep_for(std::chrono::nanoseconds(10000000));
        system("cls");

    }
    //char i;
    //cout << char (i = -36) << char (i = -33);
    delete[] gameField;
    cin.get();

    return 0;
}

void buildField(char*gf){

    for (int i=0; i < (h-1); i++){
        for (int j=0; j < (w-1); j++){
            if(i == 0){
                gf[(i*w+j)] = '_';
            }else if(j == 0){
                gf[(i*w+j)] = ']';
            }else if(j == (w-2)){
                gf[(i*w+j)] = '[';
            }else{
                gf[(i*w+j)] = ' ';
            }
        }
    }
}

void printFrame(char* gf){

    string buf;
    string bufStr = "";
    for (int i=0; i < (h-1); i++){
        for(int j=0; j < (w-1); j++){
            buf = gf[(i*w+j)];
            bufStr += buf;
        }
        cout << bufStr << endl;
        bufStr = "";
    }

}
/*
void setControls(controls* con){

    cout << "Enter control keys:" << endl;
    cout << "Left: ";
    cin >> con->left;
    cout << "Right: ";
    cin >> con->right;
    system("cls");
}
*/
void boardPosition(int x, char* gf){

    for(int i = 0; i < 8; i++){
        gf[w*(h-2)+x+i] = -33;
    }
}

void ballPosition(int* xy, char* gf){

    if((xy[1]%2) == 1){
        gf[w*((xy[1])/2)+xy[0]] = -36;
    }else{
        gf[w*((xy[1])/2)+xy[0]] = -33;
    }
}

int changeBoardPosition(int posX, int shift){

    if(posX > 1 && shift < 0) {
        posX += shift;
    }else if(posX < (w-10) && shift > 0){
        posX += shift;
    }
    return posX;
}

int changeBallPosition(int* xy, char* gf){

    static int xx = 1;
    static int yy = -1;
    static int counter = 0;

    if(gf[w*(((xy[1])/2)+yy)+xy[0]] == -33){
        counter++;
    }
    if(gf[w*(((xy[1])/2)+yy)+xy[0]] == ' '){
        xy[1] += yy;
    }else {
        xy[1] += yy;
        yy = -yy;
    }
    if(gf[w*((xy[1])/2)+(xy[0]+xx)] == ' '){
        xy[0] += xx;
    }else {
        xx = -xx;
        xy[1] += xx;
    }
    return counter;
}
