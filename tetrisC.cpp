#include <stdlib.h>
#include <GL/glut.h>
#include "tetrisBoard.h"
#include "tetrisTextures.h"

//Constant integer values denoting the size of the window.
const int WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 720;

//Constant integer value denoting the size of each cell. This is independant of the window size.
const int CELL_SIZE = 50;

//These offsets are used to move the orthogonal view and center the panels in relation to the window size
const int xOffset = 5 * CELL_SIZE, yOffset = 11 * CELL_SIZE;

//Constant integer. The shape falls once every *delay* milliseconds
const int delay = 80;

//Draws the board. This happens every frame while the gameState is PLAYING
void drawTetrisBoard(){

	int x1, y1, x2, y2;

	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 10; j++){
			switch(tetrisBoard[i][j]){
				case RED:
					glColor3f(1.0f, 0.0f, 0.0f);
					break;
				case GREEN:
					glColor3f(0.0f, 1.0f, 0.0f);
					break;
				case BLUE:
					glColor3f(0.0f, 0.0f, 1.0f);
					break;
				case PURPLE:
					glColor3f(1.0f, 0.0f, 1.0f);
					break;
				case YELLOW:
					glColor3f(1.0f, 1.0f, 0.0f);
					break;
				case EMPTY_CELL:
					glColor3f(1.0f, 1.0f, 1.0f);
					break;
				case FLASH_CELL:
					glColor3f(0.5f, 0.5f, 0.5f);
					break;
				default:
					glColor3f(0.0f, 1.0f, 1.0f);
					break;
			}
			x1 = j * CELL_SIZE;
			y1 = i * CELL_SIZE;
			x2 = (j + 1) * CELL_SIZE;
			y2 = (i + 1) * CELL_SIZE;
			glRecti(x1, y1, x2, y2);
		}
	}
	glFlush();
}

//Draws the side panel that shows the next shape. This is also drawn every frame.
//A more efficient way would be if it was drawn when getNextShape() is called.
//But this is not done because that function is in tetrisBoard.h
void drawNextShapePanel(){
	int x1 = -5 * CELL_SIZE;
	int y1 = 8 * CELL_SIZE;
	int x2 = -1 * CELL_SIZE;
	int y2 = 12 * CELL_SIZE;

	glColor3f(1.0f, 1.0f, 1.0f);
	glRecti(x1, y1, x2, y2);

	switch(nextShape.color){
		case RED:
			glColor3f(1.0f, 0.0f, 0.0f);
			break;
		case GREEN:
			glColor3f(0.0f, 1.0f, 0.0f);
			break;
		case BLUE:
			glColor3f(0.0f, 0.0f, 1.0f);
			break;
		case PURPLE:
			glColor3f(1.0f, 0.0f, 1.0f);
			break;
		case YELLOW:
			glColor3f(1.0f, 1.0f, 0.0f);
			break;
		default:
			glColor3f(0.0f, 1.0f, 1.0f);
			break;
	}

	switch(nextShape.type){
		case BLOCK:
			glRecti(x1 + CELL_SIZE, y1 + CELL_SIZE, x2 - CELL_SIZE, y2 - CELL_SIZE);
			break;
		case Z:
			glRecti(x1 + CELL_SIZE, y2, x1 + 2 * CELL_SIZE , y1 + 2 * CELL_SIZE);
			glRecti(x1 + 2* CELL_SIZE, y2 - CELL_SIZE, x2 - CELL_SIZE , y1 + CELL_SIZE);
			break;
		case STRAIGHT:
			glRecti(x1 + CELL_SIZE, y2, x1 + 2 * CELL_SIZE , y1);
			break;
		case L:
			glRecti(x1 + CELL_SIZE, y2, x1 + 2 * CELL_SIZE , y1 + CELL_SIZE);
			glRecti(x1 + 2* CELL_SIZE, y1 + 2 * CELL_SIZE, x2 - CELL_SIZE , y1 + CELL_SIZE);
			break;
		default:
			glRecti(x1 + CELL_SIZE, y2, x1 + 2 * CELL_SIZE , y1 + CELL_SIZE);
			glRecti(x1 + 2* CELL_SIZE, y2 - CELL_SIZE, x2 - CELL_SIZE , y1 + 2 * CELL_SIZE);
	}

	glFlush();
}

//Draws the game over screen
void drawGameOver(){
	int x1 = -5.1 * CELL_SIZE;
	int y1 = 15.1 * CELL_SIZE;
	int x2 = 15.1 * CELL_SIZE;
	int y2 = 4.9 * CELL_SIZE;

	glColor3f(1.0f, 1.0f, 1.0f);
	glRecti(x1, y1, x2, y2);

	x1 = -5 * CELL_SIZE;
	y1 = 15 * CELL_SIZE;
	x2 = 15 * CELL_SIZE;
	y2 = 5 * CELL_SIZE;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[10]);
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex2i(x1, y1);
		glTexCoord2f(1, 0);
		glVertex2i(x2, y1);
		glTexCoord2f(1, 1);
		glVertex2i(x2, y2);
		glTexCoord2f(0, 1);
		glVertex2i(x1, y2);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	glFlush();
}

//Draws the Score
void drawScore(){
	
}

//Draws the "How to Play" and "Made By" pictures
void drawInfoPanel(){
	
}

//The GLUT Special key function
void arrows(int key, int x, int y){
	switch(key){
		case GLUT_KEY_DOWN:
			shapeFall();
			break;
		case GLUT_KEY_UP:
			rotate();
			break;
		case GLUT_KEY_LEFT:
			moveLeft();
			break;
		case GLUT_KEY_RIGHT:
			moveRight();
			break;
	}
}

//The GLUT keyboard function. 
void exit(unsigned char key, int x, int y){
	if(key == 27){
		exit(0);
	}
}

//The GLUT timer function. Calls the shapeFall() function once every *delay* calls of this method
void timer(int value){
	int newValue = value;
	if(gameState == PLAYING){
		if (newValue++ < delay){}
		else {
			shapeFall();
			newValue = 0;
		}
		if(clearRowsFlag) {
			clearFullRows();
			newValue--;
		}
		glutPostRedisplay();
		glutTimerFunc(1, timer, newValue);
	} else {
		drawGameOver();
	}
}

//The GLUT display function
void tetrisDisplay(){
	drawTetrisBoard();
	drawNextShapePanel();
	drawScore();
	drawInfoPanel();
}

//The GLUT reshape function. This sets the window back to it's default size if the user tries to resize it
void lockResizing(int width, int height){
	glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	tetrisDisplay();
}

int main(int argc, char* argv[]){

	initBoard();
	nextShape.type = (rand() % 5) + 1;
	nextShape.color = (rand() % 6) + 1;
	nextShape.orientation = UP;
	getNextShape();

	glutInit(&argc, argv);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Tetris");

	glOrtho(-WINDOW_WIDTH + xOffset, WINDOW_WIDTH + xOffset, 
		-WINDOW_HEIGHT + yOffset, WINDOW_HEIGHT + yOffset,-1, 1);

	loadTextures();
	glutReshapeFunc(lockResizing);
	glutDisplayFunc(tetrisDisplay);
	glutTimerFunc(1, timer, 0);
	glutSpecialFunc(arrows);
	glutKeyboardFunc(exit);

	glutMainLoop();
	return 0;

}