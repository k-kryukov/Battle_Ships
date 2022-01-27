//#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <iostream>
#include <glut.h>
#include <cstdio>
#include <ctime>
#include <vector>
#include <conio.h>
#include <algorithm>
using namespace std;
const int FRAME_HEIGHT = 100;
const int FRAME_WIDTH = 100;
const int SIZE = 5;
typedef unsigned char byte; // [0; 255]
extern byte Frame[FRAME_HEIGHT][FRAME_WIDTH][3];
extern byte Frame2[FRAME_HEIGHT][FRAME_WIDTH][3];
extern bool MyField[10][10]; // my field with ships
extern bool EnemyField[10][10]; // bot's field with ships
extern bool UBot[10][10]; // field with info where bot has already shot to
extern bool UMy[10][10]; // field with info where I have already shot to
extern bool allowed[10][10];
//extern vector<bool[][10]> posBot;
extern int countMy;
extern int countBot;
extern int prevStep;
extern bool TEST;
extern int oneByOne;
extern int cntShot;
extern bool myStep;
extern bool shipPut;
extern int prevx, prevy;
extern vector<pair<int, int>> possible;
void MakeGrid(byte f[FRAME_HEIGHT][FRAME_WIDTH][3]);
void step_bot();
void FillBot();
void Display2();
void End();
void Display();
void Mouse2(int button, int state, int x, int y);
void Mouse1(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void Reshape(int width, int height);
void Idle();
void FillKill(int x, int y, byte f[FRAME_HEIGHT][FRAME_WIDTH][3]);
bool CheckingDiag(bool f[10][10]);
bool CheckingCnt(bool f[10][10]);
void FillPixel(int x, int y, byte f[FRAME_HEIGHT][FRAME_WIDTH][3], bool field[10][10]);
void FillHit(int x, int y, byte f[FRAME_HEIGHT][FRAME_WIDTH][3]);
bool CheckingCnt(bool f[10][10]);
void FillMiss(int x, int y, byte f[FRAME_HEIGHT][FRAME_WIDTH][3]);
bool IsOk(pair<int,int> x);
void End();
void easy_bot();