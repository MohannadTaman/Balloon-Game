/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#include <cmath>
#include<GL/glut.h>
#include <stdio.h>
#include <unistd.h>
int phyWidth= 700;
int phyHeight= 700;
int logWidth=100;
int logHeight=100;
int centerX=logWidth/2;
int centerY=logHeight/2;
int sqWid=20;
int alphaX=0;
int alphaY=0;
int maxY=100;
int mouseXX;
int mouseYY;
int t=20;
int restart=0;
int score = 0;
int flag[20];
int x[20];
int color[3] = {0, 1 , 0};
int delta[20];
bool up[20];
bool pop[20];
int colorballoon[20];

void setflags(){
    for(int i=0; i<20; i++){
        flag[i]=0;
        pop[i]=false;
        delta[i]=0;
        up[i]=true;
    }
}

class Balloon{
public:
    float x, y, radius; // Position and radius of the balloon
    float red, green, blue; // Color of the balloon
public:
    Balloon(){}
    Balloon(float x, float y, float radius, float red, int green, float blue) {
        this->x = x;
        this->y = y;
        this->radius = radius;
        this->red = red;
        this->green = green;
        this->blue = blue;
    }
    void draw() {
        // Draw the balloon body
        glColor3f(red, green, blue);
        glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++) {
            float angle = i * 3.14159 / 180;
            glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
        }
        glEnd();
        // Draw the balloon knot
        glColor3f(red * 0.8, green * 0.8, blue * 0.8);
        glBegin(GL_POLYGON);
        glVertex2f(x - radius * 0.1, y - radius * 1.2);
        glVertex2f(x + radius * 0.1, y - radius * 1.2);
        glVertex2f(x + radius * 0.1, y - radius * 1.4);
        glVertex2f(x - radius * 0.1, y - radius * 1.4);
        glEnd();
    }
};
Balloon myBalloon[20];

void mouseClick(int btn, int state, int x, int y)
{
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        mouseXX = x;
        mouseXX=0.5+1.0*mouseXX*logWidth/phyWidth;
        mouseYY = phyHeight - y;
        mouseYY=0.5+1.0*mouseYY*logHeight/phyHeight;
    for(int i=0; i<20; i++)
        if(up[i])
            if( mouseXX>=(myBalloon[i].x-myBalloon[i].radius) && mouseXX<=(myBalloon[i].x+myBalloon[i].radius) && mouseYY >=(myBalloon[i].y-myBalloon[i].radius) && mouseYY <=(myBalloon[i].y+myBalloon[i].radius)){
                if(myBalloon[i].green == 1) score+=2;
                else score+=1;
            pop[i]=true;
            glutPostRedisplay();
            }
    }
}

bool basketCollision(){
    int basketMinX = 42 + alphaX;
    int basketMaxX = 60 + alphaX;
    int basketMinY = 5;
    int basketMaxY = 15;
    for(int i=0; i<20; i++){
        if(myBalloon[i].x >= basketMinX && myBalloon[i].x<=basketMaxX && myBalloon[i].y>=basketMinY && myBalloon[i].y<=basketMaxY && !up[i]  ){
        score++;
        pop[i]=true;
        }
    }
}

void specialKeyboard(int key, int x, int y){
    if(key == GLUT_KEY_LEFT)
        alphaX-=5;
    if(key== GLUT_KEY_RIGHT)
        alphaX+=5;
    if(restart== 1)
        if (key== GLUT_KEY_F1)
        {
            restart = 0;
            score = 0;
            t = 20;
        }
    glutPostRedisplay();
}

void Timeout(int value){
    glutTimerFunc(1000, Timeout, value);
    t--;
    if(t == 0) restart=1;
    glutPostRedisplay();
}

void Timer(int value){
    glutTimerFunc(25, Timer, value);
    glutPostRedisplay();
}

void init2D(){
    glClearColor( 1.0, 1.0, 1.0, 1.0);
    glMatrixMode( GL_PROJECTION);
    gluOrtho2D( 0.0, logWidth, 0.0, logHeight);
}

void printSome(char *str,int x,int y){
    glColor3f (1.0, 1.0, 1.0);
    glRasterPos2d(x,y);
    for (int i=0;i<strlen(str);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[i]);
    }
    glFlush();
}

void backround1(){
    glColor3f(0, 0, 1.0);
    glBegin(GL_QUADS);
    glVertex2i(0,100);
    glVertex2i(100,100);
    glVertex2i(100,0);
    glVertex2i(0,0);
    glEnd();
}

void backround2(){
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON); // Each set of 4 vertices form a quad
    glVertex2f(0, 0);     // x, y
    glVertex2f(0, 10);    // x, y
    glVertex2f(100, 10);  // x, y
    glVertex2f(100, 0);   // x, y
    glEnd();
}

void Basket(){
    glColor3f(0.75, 0.0, 0.0);
    glBegin(GL_POLYGON); // Each set of 4 vertices form a quad
    glVertex2f(45 + alphaX, 5); // x, y
    glVertex2f(42 + alphaX, 15); // x, y
    glVertex2f(60 + alphaX, 15); // x, y
    glVertex2f(57 + alphaX, 5); // x, y
    glEnd();
}

void RestartPage(){
    glColor3f(0, 0.7, 0.0);
    glBegin(GL_QUADS);
    glVertex2i(0,100);
    glVertex2i(100,100);
    glVertex2i(100,0);
    glVertex2i(0,0);
    glEnd();
}

void resetBalloons(){
    for(int i=0; i<20; i++)
        flag[i]=0;
    for(int i=0; i<20;i++){
        myBalloon[i].x = 0;
        myBalloon[i].y = 0;
        myBalloon[i].radius = 0;
        myBalloon[i].red = 0.0;
        myBalloon[i].green = 0.0;
        myBalloon[i].blue = 0.0;
        delta[i] = 0;
        up[i] = true;
        pop[i] = false;
    }
}

void Game(){
if(delta[0] == 0)
{
x[0]= rand()%90;
colorballoon[0] = color[rand()%2];
}

if(pop[0]==false){
myBalloon[0].x=x[0];
myBalloon[0].y=0+delta[0];
myBalloon[0].radius=2;
myBalloon[0].red=1.0;
myBalloon[0].green=colorballoon[0];
myBalloon[0].blue=0.0;
myBalloon[0].draw();
if (delta[0]>maxY){

        up[0]=!up[0];
        if(score>0)
        score--;
    }

if (up[0]) delta[0]+=1; else delta[0]-=2;
}
else if(pop[0]==true){
myBalloon[0].x=x[0];
myBalloon[0].y=0;
myBalloon[0].radius=0;
myBalloon[0].red=1.0;
myBalloon[0].green=colorballoon[0];
myBalloon[0].blue=0.0;
myBalloon[0].draw(); // Draw the updated balloon
}

if(myBalloon[0].y >= 30 || (myBalloon[0].y<=100 && !up[0] ) )
{
flag[0] = 1;
}

for(int i=1; i<=19; i++){
 if(flag[i-1] == 1){

if (delta[i] == 0) {
    x[i] = rand() % 90;
    colorballoon[i] = color[rand() % 2];
}

if (pop[i] == false) {
    myBalloon[i].x = x[i];
    myBalloon[i].y = 0 + delta[i];
    myBalloon[i].radius = 2;
    myBalloon[i].red = 1.0;
    myBalloon[i].green = colorballoon[i];
    myBalloon[i].blue = 0.0;
    myBalloon[i].draw(); // Draw the updated balloon
    if (delta[i] > maxY){
            up[i] = !up[i];
            if(score>0)
            score--;
        }
    if (up[i]) delta[i] += 1; else delta[i] -= 2;
} else if (pop[i] == true) {
    myBalloon[i].x = x[i];
    myBalloon[i].y = 0 ;
    myBalloon[i].radius = 0;
    myBalloon[i].red = 1.0;
    myBalloon[i].green = colorballoon[i];
    myBalloon[i].blue = 0.0;
    myBalloon[i].draw(); // Draw the updated balloon
}

if (myBalloon[i].y >= 30 || (myBalloon[i].y <= 100 && !up[i])) {
    flag[i] = 1;
}
}
}
}

void Display(){
    glClear( GL_COLOR_BUFFER_BIT);
    if(restart == 0){
        backround1();
        backround2();
        char _score[10];
        char _t[10];
        itoa(score,_score,10);
        printSome("Score: ",2,95);
        printSome(_score,8,95);
        itoa(t,_t,10);
        printSome("Timer: ", 85, 95);
        printSome(_t,94,95);
        Basket();
        basketCollision();
        Game();
    }
    else if(restart == 1){
        RestartPage();
        resetBalloons();
        printSome("GAME OVER!",40,55);
        char _score[10];
        itoa(score,_score,10);
        printSome("Score: ",40,50);
        printSome(_score,47,50);
        printSome("Press F1 to play again",40,45);
    }

    glutSwapBuffers();
    glFlush();
}

int main( int argc, char ** argv){
    glutInit( &argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition( 100, 100);
    glutInitWindowSize(phyWidth, phyHeight);
    glutCreateWindow( "Balloon Game");
    init2D();
    setflags();
    glutSpecialFunc(specialKeyboard);
    glutMouseFunc(mouseClick);
    glutDisplayFunc(Display);
    Timer(0);
    Timeout(1);
    glutMainLoop();
}
