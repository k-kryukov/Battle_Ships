#include "headers.h"
byte Frame[FRAME_HEIGHT][FRAME_WIDTH][3];
byte Frame2[FRAME_HEIGHT][FRAME_WIDTH][3];
bool MyField[10][10];
bool EnemyField[10][10];

int main(int argc, char** argv) {  
    //posBot = {};
    //  https://www.opengl.org.ru/coding/glut/glut2.html
    FillBot();
    // First stage
    glutInit(&argc, argv); // Initialization
    // First window **********************
    // Second stage
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            possible.push_back({ i,j });
        }
    }
    srand(time(0));
    random_shuffle(possible.begin(), possible.end());
    MakeGrid(Frame);
    glutInitWindowSize(FRAME_WIDTH*SIZE, FRAME_HEIGHT*SIZE); // Width and length in pixels
    glutInitWindowPosition(0, 0); // Window position (left up by default)
    glutInitDisplayMode(GLUT_RGB); // Image type (GLUT const are given by arguments)
    // Third stage
    glutCreateWindow("MY FIELD");
    // Fourth stage
    glMatrixMode(GL_PROJECTION);
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutMouseFunc(Mouse1);
    glutIdleFunc(Idle);
    glutKeyboardFunc(Keyboard);
    // Second window ********************** 
    glutMainLoop();
    return 0;
}