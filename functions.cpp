#include "headers.h"

int countship = 0;
int cntShot = 0;
bool myStep = true;
bool shipPut = false;
bool UMy[10][10]; // field with info where I have already shot to
bool UBot[10][10]; // field with info where bot has already shot to
bool TEST = true;

bool IsOk(pair<int, int> a) {
    if (a.first >= 0 && a.first < 10) {
        if (a.second >= 0 && a.second < 10)
            return true;
    }
    return false;
}
bool CheckingDiag(bool f[10][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (f[i][j] == true) {
                pair<int, int> lv = { i - 1, j - 1 }, ln = { i + 1, j - 1 }, pv = { i - 1, j + 1 }, pn = {i+1,j+1};
                for (pair<int, int> x : {lv,ln,pv,pn}) {
                    if (IsOk(x)) {
                        if (f[x.first][x.second] == true)
                            return false;
                    }
                }
            }
        }
    }
    return true;
}
void MakeGrid(byte f[FRAME_HEIGHT][FRAME_WIDTH][3]) {
    for (int i = 0; i < FRAME_HEIGHT; i++) {
        for (int j = 0; j < FRAME_WIDTH; j++) {
            f[i][j][0] = 220;
            f[i][j][1] = 50;
            f[i][j][2] = 220;
        }
    }
    for (int i = 0; i < FRAME_HEIGHT; ++i)
    {
        for (int j = 9; j < FRAME_WIDTH; j += 10)
        {
            f[i][j][0] = 0;
            f[i][j][1] = 0;
            f[i][j][2] = 0;
        }
    }
    for (int i = 9; i < FRAME_HEIGHT; i += 10)
    {
        for (int j = 0; j < FRAME_WIDTH; j++)
        {
            f[i][j][0] = 0;
            f[i][j][1] = 0;
            f[i][j][2] = 0;
        }
    }
}
void Display()
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear field
    glRasterPos2d(-1, 1); // Axes orientation
    glPixelZoom(SIZE, -SIZE); // How many pixels is one cell of array
    glDrawPixels(FRAME_WIDTH, FRAME_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, Frame); // draw by matrix
    glFinish(); // show result
}

void Display2()
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear field
    glRasterPos2d(-1, 1); // Axes orientation
    glPixelZoom(SIZE, -SIZE); // How many pixels is one cell of array
    glDrawPixels(FRAME_WIDTH, FRAME_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, Frame2); // draw by matrix
    glFinish(); // show result
}
void Fill(int x, int y, int r, int g, int b, byte f[FRAME_HEIGHT][FRAME_WIDTH][3]) {
    f[y][x][0] = r;
    f[y][x][1] = g;
    f[y][x][2] = b;
}

void FillPixel(int x, int y, byte f[FRAME_HEIGHT][FRAME_WIDTH][3], bool field[10][10]) {
    if (f[y * 10][x * 10][1] != 255)
    {
        if (countship == 20) {
            cout << "TO MANY SHIPS OR DETAILS OF THEM!" << endl;
            return;
        }
        field[y][x] = true;
        countship++;
        for (int i = x * 10; i < x * 10 + 9; i++) {
            for (int j = y * 10; j < y * 10 + 9; j++) {
                Fill(i, j, 0, 255, 0, f);
            }
        }
    }
    else
    {
        field[y][x] = false;
        countship--;
        for (int i = x * 10; i < x * 10 + 9; i++) {
            for (int j = y * 10; j < y * 10 + 9; j++) {
                Fill(i, j, 220, 50, 220, f);
            }
        }
    }
}

void FillHit(int x, int y, byte f[FRAME_HEIGHT][FRAME_WIDTH][3]) {
    for (int i = x * 10, j = y * 10; i < x * 10 + 9 && j < y * 10 + 9; j++, i++) {
        Fill(i, j,255,0, 0, f);
    }
    for (int i = x * 10, j = y * 10 + 8; i < x * 10 + 9 && j >= y * 10; j--, i++) {
        Fill(i, j, 255,0, 0, f);
    }
}

void FillKill(int x, int y, byte f[FRAME_HEIGHT][FRAME_WIDTH][3]) {
    for (int i = x * 10; i < x * 10 + 9; i++) {
        for (int j = y * 10; j < y * 10 + 9; j++) {
            Fill(i, j, 255, 0, 0, f);
        }
    }
}

void FillMiss(int x, int y, byte f[FRAME_HEIGHT][FRAME_WIDTH][3]) {
    for (int i = x * 10 + 3; i < x * 10 + 6; i++) {
        for (int j = y * 10 + 3; j < y * 10 + 6; j++) {
            Fill(i, j, 255, 255, 255, f);
        }
    }
}

void Mouse1(int button, int state, int x, int y) {
    if (shipPut)
        return;
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            if (!((x / 5) % 10 == 9 || (y / 5) % 10 == 9)) {
                FillPixel(x / 5 / 10, y / 5 / 10, Frame, MyField);
            }
        }
    }
}

void End() {
    // if (TEST)
       // return;
    if (countBot >= 20) {
        cout << "YOU LOSE!" << endl;
        exit(0);
        return;
    }
    else if (countMy >= 20) {
        cout << "YOU WON!" << endl;
        exit(0);
        return;
    }
}

void Mouse2(int button, int state, int x, int y) {
    if (!myStep)
        return;
    if (UMy[x / 5 / 10][y / 5 / 10]) { // true <=> this cell has been already shot
        return;
    }
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            if (!((x / 5) % 10 == 9 || (y / 5) % 10 == 9)) {
                if (EnemyField[y / 5 / 10][x / 5 / 10] == true) { // shot
                    FillHit(x / 5 / 10, y / 5 / 10, Frame2);
                    countMy++;
                }
                else {
                    FillMiss(x / 5 / 10, y / 5 / 10, Frame2);
                    myStep = !myStep;
                }
                UMy[x / 5 / 10][y / 5 / 10] = true;
            }
        }
    }
    End();
    /*if (!myStep) {
        easy_bot();
    }*/
}
bool used[10][10];

void MakeSecW() {
    MakeGrid(Frame2);
    glutInitWindowSize(FRAME_WIDTH * SIZE, FRAME_HEIGHT * SIZE);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_RGB);
    glutCreateWindow("ENEMY'S FIELD");
    glMatrixMode(GL_PROJECTION);
    glutDisplayFunc(Display2);
    glutReshapeFunc(Reshape);
    glutMouseFunc(Mouse2);
    glutIdleFunc(Idle);
}
int search(int i, int j, int depth, bool f[10][10]) {
    used[i][j] = true;
    if (i - 1 >= 0) {
        if (used[i-1][j]!=true && f[i-1][j]==true)
            return search(i - 1, j, depth + 1, f);
    }
    if (i + 1 < 10) {
        if (used[i + 1][j] != true && f[i + 1][j] == true)
            return search(i + 1, j, depth + 1, f);
    }
    if (j - 1 >= 0) {
        if (used[i][j - 1] != true && f[i][j - 1] == true)
            return search(i, j - 1, depth + 1, f);
    }
    if (j + 1 < 10) {
        if (used[i][j+1] != true && f[i][j+1] == true)
            return search(i, j + 1, depth + 1, f);
    }
    return depth;
}

bool CheckingCnt(bool f[10][10]) {
    vector<int> type(5, 0);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++)
            used[i][j] = false;
    }
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (f[i][j] == true && used[i][j]!=true) {
                int size = search(i, j, 1, f);
                if (size > 4)
                    return false;
                type[size]++;
            }
        }
    }
    if (type[1] == 4 && type[2] == 3 && type[3] == 2 && type[4] == 1)
        return true;
    return false;
}

void Keyboard(unsigned char key, int x, int y)
{
    if (!shipPut) {
        if (key == 13) {
            if (TEST)
                countship = 20; // ONLY FOR TEST
            if (countship != 20) {
                cout << "NOT ENOUGH SHIPS!!" << endl;
                return;
            }
            bool OK = CheckingDiag(MyField);
            bool ok2 = CheckingCnt(MyField);
            if (OK && ok2 || TEST) {
                cout << "NICE SHIPS!" << endl;
                myStep = true;
                shipPut = true;
                MakeSecW(); // make new field
            }
            else cout << "BAD SHIPS :(" << endl;
        }
    }
}

void Reshape(int width, int height) {
}

void Idle()
{
    glutPostRedisplay();
}

