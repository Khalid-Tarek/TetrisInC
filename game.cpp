#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

//Constant integer values denoting the size of the window and the size of each square cell.
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int CELL_SIZE = 40;

//Constant integer values used to denote the current game state.
const int GAME_OVER = 0;
const int PLAYING = 1;
int gameState = PLAYING;

//These offsets are used to position the views in the window
int xOffset = 5 * CELL_SIZE;
int yOffset = 11 * CELL_SIZE;

//The tetris board is kept in a 2d array. Cells start from the bottom left (0,0) to the top right (24, 10).
int board[24][10];

//Constant representations of the five shapes
const int BLOCK = 0, Z = 1, STRAIGHT = 2, L = 3, T = 4;

//Constant representations of the colors available in the game
const int RED = 0, GREEN = 1, BLUE = 2, PURPLE = 3, YELLOW = 4, CYAN = 5;
const int EMPTY_CELL = 6;
const int FLASH_CELL = 7;
const int BLACK = 8;

//Constant representations of the orientations of the shape. This is used to rotate the current shape. 
//(This is also used as directions for collision detection)
const int UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3;

struct Shape{
	int type;
	int color;
	int position[2];
	int orientation;
};

Shape currentShape;
Shape nextShape;

void initBoard(){
	for(int i = 0; i < 24; i++){
		for(int j = 0; j < 10; j++){
			board[i][j] = 6;
		}
	}
}

void drawBoard(){

	int x1, y1, x2, y2;

	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 10; j++){
			switch(board[i][j]){
				case 0:
					glColor3f(1.0f, 0.0f, 0.0f);
					break;
				case 1:
					glColor3f(0.0f, 1.0f, 0.0f);
					break;
				case 2:
					glColor3f(0.0f, 0.0f, 1.0f);
					break;
				case 3:
					glColor3f(1.0f, 0.0f, 1.0f);
					break;
				case 4:
					glColor3f(1.0f, 1.0f, 0.0f);
					break;
				case 6:
					glColor3f(1.0f, 1.0f, 1.0f);
					break;
				default:
					glColor3f(0.0f, 1.0f, 1.0f);
					break;
			}
			x1 = j * CELL_SIZE - xOffset;
			y1 = i * CELL_SIZE - yOffset;
			x2 = (j + 1) * CELL_SIZE - xOffset;
			y2 = (i + 1) * CELL_SIZE - yOffset;
			glRecti(x1, y1, x2, y2);
		}
	}
	glFlush();
}

void drawNextShapeView(){
	int x1 = -5 * CELL_SIZE - xOffset;
	int y1 = 8 * CELL_SIZE - yOffset;
	int x2 = -1 * CELL_SIZE - xOffset;
	int y2 = 12 * CELL_SIZE - yOffset;

	glColor3f(1.0f, 1.0f, 1.0f);
	glRecti(x1, y1, x2, y2);

	switch(nextShape.color){
		case 0:
			glColor3f(1.0f, 0.0f, 0.0f);
			break;
		case 1:
			glColor3f(0.0f, 1.0f, 0.0f);
			break;
		case 2:
			glColor3f(0.0f, 0.0f, 1.0f);
			break;
		case 3:
			glColor3f(1.0f, 0.0f, 1.0f);
			break;
		case 4:
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

void tetrisDisplay();

void getNextShape(){
	if(board[19][5] != 6){
		gameState = GAME_OVER;
		return;
	}

	currentShape = nextShape;
	currentShape.position[0] = 20;
	currentShape.position[1] = 5;

	board[20][5] = currentShape.color;
	switch(currentShape.type){
		case STRAIGHT:
			board[21][5] = currentShape.color;
			board[22][5] = currentShape.color;
			board[23][5] = currentShape.color;
			break;
		case BLOCK:
			board[21][5] = currentShape.color;
			board[20][6] = currentShape.color;
			board[21][6] = currentShape.color;
			break;
		case Z:
			board[21][5] = currentShape.color;
			board[21][4] = currentShape.color;
			board[22][4] = currentShape.color;
			break;
		case L:
			board[20][6] = currentShape.color;
			board[21][5] = currentShape.color;
			board[22][5] = currentShape.color;
			break;
		case T:
			board[21][5] = currentShape.color;
			board[21][6] = currentShape.color;
			board[22][5] = currentShape.color;
			break;
	}

	nextShape.type = rand() % 5;
	nextShape.color = rand() % 6;
	switch(nextShape.type){
		case STRAIGHT:
		case Z:
			nextShape.orientation = UP;
			break;
		default:
			nextShape.orientation = RIGHT;
	}

	tetrisDisplay();
}


bool willShapeCollide(int direction){
	bool collide = false;

	int cellUnder, cellLeft, cellRight, xPosition, yPosition;

	switch(direction){
		//Check collision if the shape is falling
		case DOWN:
			cellUnder = currentShape.position[0] - 1;
			xPosition = currentShape.position[1];
			if(cellUnder < 0) {return true;}

			switch(currentShape.type){
				
				case STRAIGHT:
					switch(currentShape.orientation){
						case UP:	
						case DOWN:
							if(board[cellUnder][xPosition] != EMPTY_CELL) {return true;}
							break;
						case LEFT:
						case RIGHT:
							for(int i = 0; i < 4; i++){
								if(board[cellUnder][xPosition + i] != EMPTY_CELL) {return true;}
							}
							break;
					}
					break;

				case BLOCK:
					if(board[cellUnder][xPosition] != EMPTY_CELL 
						|| board[cellUnder][xPosition + 1] != EMPTY_CELL) {return true;}
					break;

				case Z:
					switch(currentShape.orientation){
						case UP:
						case DOWN:
							if(board[cellUnder][xPosition] != EMPTY_CELL 
								|| board[cellUnder + 1][xPosition - 1] != EMPTY_CELL) {return true;}
							break;
						case RIGHT:
						case LEFT:
							if(board[cellUnder][xPosition] != EMPTY_CELL
							|| board[cellUnder][xPosition + 1] != EMPTY_CELL
							|| board[cellUnder + 1][xPosition + 2] != EMPTY_CELL) {return true;}
							break;
					}
					break;

				case L:
					switch(currentShape.orientation){
						case UP:
							if(board[cellUnder][xPosition] != EMPTY_CELL
							|| board[cellUnder][xPosition + 1] != EMPTY_CELL 
							|| board[cellUnder][xPosition + 2] != EMPTY_CELL) {return true;}
							break;
						case DOWN:
							if(board[cellUnder][xPosition] != EMPTY_CELL
							|| board[cellUnder + 1][xPosition + 1] != EMPTY_CELL
							|| board[cellUnder + 1][xPosition + 2] != EMPTY_CELL) {return true;}
							break;
						case LEFT:
							if(board[cellUnder][xPosition] != EMPTY_CELL
								|| board[cellUnder + 2][xPosition - 1] != EMPTY_CELL ) {return true;}
							break;
						case RIGHT:
							if(board[cellUnder][xPosition] != EMPTY_CELL 
								|| board[cellUnder][xPosition + 1] != EMPTY_CELL) {return true;}
					}
					break;

				case T:
					switch(currentShape.orientation){
						case UP:
							if(board[cellUnder][xPosition] != EMPTY_CELL
								|| board[cellUnder][xPosition + 1] != EMPTY_CELL
								|| board[cellUnder][xPosition + 2] != EMPTY_CELL) {return true;} 
							break;
						case DOWN:
							if(board[cellUnder][xPosition] != EMPTY_CELL
								|| board[cellUnder + 1][xPosition - 1] != EMPTY_CELL
								|| board[cellUnder + 1][xPosition + 1] != EMPTY_CELL) {return true;} 
							break;
						case LEFT:
							if(board[cellUnder][xPosition] != EMPTY_CELL
								|| board[cellUnder + 1][xPosition - 1] != EMPTY_CELL) {return true;} 
							break;
						case RIGHT:
							if(board[cellUnder][xPosition] != EMPTY_CELL
								|| board[cellUnder + 1][xPosition - 1] != EMPTY_CELL
								|| board[cellUnder + 1][xPosition + 1] != EMPTY_CELL) {return true;}
							break;
					}
					break;
			}
			break;

		//Check collision if the shape is moving left
		case LEFT:
			yPosition = currentShape.position[0];
			cellLeft = currentShape.position[1] - 1;
			if(cellLeft < 0) {return true;}

			switch(currentShape.type){
				case STRAIGHT:
					switch(currentShape.orientation){
						case UP:
						case DOWN:
							for(int i = 0; i < 4; i++){
								if(board[yPosition + i][cellLeft] != EMPTY_CELL) {return true;}
							}
							break;
						case LEFT:
						case RIGHT:
							if(board[yPosition][cellLeft] != EMPTY_CELL) {return true;}
							break;
					}
					break;
				case BLOCK:
					if(board[yPosition][cellLeft] != EMPTY_CELL
						|| board[yPosition + 1][cellLeft] != EMPTY_CELL) {return true;}
					break;
				case Z:
					switch(currentShape.orientation){
						case UP:
						case DOWN:
							if(cellLeft > 0 && (board[yPosition][cellLeft] != EMPTY_CELL
								|| board[yPosition + 1][cellLeft - 1] != EMPTY_CELL
								|| board[yPosition + 2][cellLeft - 1] != EMPTY_CELL)) {return true;}
							break;
						case LEFT:
						case RIGHT:
							if(board[yPosition][cellLeft] != EMPTY_CELL 
								|| board[yPosition + 1][cellLeft + 1] != EMPTY_CELL) {return true;}
							break;
					}
					break;
				case L:
					switch(currentShape.orientation){
						case UP:
							if(board[yPosition][cellLeft] != EMPTY_CELL 
								|| board[yPosition + 1][cellLeft + 2] != EMPTY_CELL) {return true;}
							break;
						case DOWN:
							if(board[yPosition][cellLeft] != EMPTY_CELL 
								|| board[yPosition + 1][cellLeft] != EMPTY_CELL) {return true;}
							break;
						case LEFT:
							if(cellLeft > 0 && (board[yPosition][cellLeft] != EMPTY_CELL 
								|| board[yPosition + 1][cellLeft] != EMPTY_CELL
								|| board[yPosition + 2][cellLeft - 1])) {return true;}
							break;
						case RIGHT:
							if(board[yPosition][cellLeft] != EMPTY_CELL 
								|| board[yPosition + 1][cellLeft] != EMPTY_CELL
								|| board[yPosition + 2][cellLeft]) {return true;}
							break;
					}
					break;
				case T:
					switch(currentShape.orientation){
						case UP:
							if(board[yPosition][cellLeft] != EMPTY_CELL 
								|| board[yPosition + 1][cellLeft + 1] != EMPTY_CELL) {return true;}
							break;
						case DOWN:
							if(cellLeft > 0 && (board[yPosition][cellLeft] != EMPTY_CELL
								|| board[yPosition + 1][cellLeft - 1])) {return true;}
							break;
						case LEFT:
							if(cellLeft > 0 && (board[yPosition][cellLeft] != EMPTY_CELL 
								|| board[yPosition + 1][cellLeft - 1] != EMPTY_CELL
								|| board[yPosition + 2][cellLeft])) {return true;}
							break;
						case RIGHT:
							if(board[yPosition][cellLeft] != EMPTY_CELL 
								|| board[yPosition + 1][cellLeft] != EMPTY_CELL
								|| board[yPosition + 2][cellLeft]) {return true;}
							break;
					}
					break;
			}
			break;
		
		//Check collision if the shape is moving right
		case RIGHT:
			yPosition = currentShape.position[0];
			cellRight = currentShape.position[1] + 1;
			if(cellRight > 9) {return true;}

			switch(currentShape.type){
				case STRAIGHT:
					switch(currentShape.orientation){
						case UP:
						case DOWN:
							for(int i = 0; i < 4; i++){
								if(board[yPosition + i][cellRight] != EMPTY_CELL) {return true;}
							}
							break;
						case LEFT:
						case RIGHT:
							if( cellRight < 7 && board[yPosition][cellRight + 3] != EMPTY_CELL) {return true;}
							break;
					}
					break;
				case BLOCK:
					if(cellRight < 9 && (board[yPosition][cellRight + 1] != EMPTY_CELL
						|| board[yPosition + 1][cellRight + 1] != EMPTY_CELL)) {return true;}
					break;
				case Z:
					switch(currentShape.orientation){
						case UP:
						case DOWN:
							if(board[yPosition][cellRight] != EMPTY_CELL
								|| board[yPosition + 1][cellRight] != EMPTY_CELL
								|| board[yPosition + 2][cellRight - 1] != EMPTY_CELL) {return true;}
							break;
						case LEFT:
						case RIGHT:
							if(cellRight < 8 && (board[yPosition][cellRight + 1] != EMPTY_CELL
								|| board[yPosition + 1][cellRight + 2] != EMPTY_CELL)) {return true;}
							break;
					}
					break;
				case L:
					switch(currentShape.orientation){
						case UP:
							if(cellRight < 8 && (board[yPosition][cellRight + 2] != EMPTY_CELL
								|| board[yPosition + 1][cellRight + 2] != EMPTY_CELL)) {return true;}
							break;
						case DOWN:
							if(cellRight < 8 && (board[yPosition][cellRight] != EMPTY_CELL
								|| board[yPosition + 1][cellRight + 2] != EMPTY_CELL)) {return true;}
							break;
						case LEFT:
							if(board[yPosition][cellRight] != EMPTY_CELL
								|| board[yPosition + 1][cellRight] != EMPTY_CELL
								|| board[yPosition + 2][cellRight] != EMPTY_CELL) {return true;}
							break;
						case RIGHT:
							if(cellRight < 9 && (board[yPosition][cellRight + 1] != EMPTY_CELL
								|| board[yPosition + 1][cellRight] != EMPTY_CELL
								|| board[yPosition + 2][cellRight] != EMPTY_CELL)) {return true;}
							break;
					}
					break;
				case T:
					switch(currentShape.orientation){
						case UP:
							if(cellRight < 8 && (board[yPosition][cellRight + 2] != EMPTY_CELL
								|| board[yPosition + 1][cellRight + 1] != EMPTY_CELL)) {return true;}
							break;
						case DOWN:
							if(cellRight < 9 && (board[yPosition][cellRight] != EMPTY_CELL
								|| board[yPosition + 1][cellRight + 1] != EMPTY_CELL)) {return true;}
							break;
						case LEFT:
							if(board[yPosition][cellRight] != EMPTY_CELL
								|| board[yPosition + 1][cellRight] != EMPTY_CELL
								|| board[yPosition + 2][cellRight] != EMPTY_CELL) {return true;}
							break;
						case RIGHT:
							if(cellRight < 9 && (board[yPosition][cellRight] != EMPTY_CELL
								|| board[yPosition + 1][cellRight + 1] != EMPTY_CELL
								|| board[yPosition + 2][cellRight] != EMPTY_CELL)) {return true;}
							break;
					}
					break;
			}
			break;
	}

	return collide;
}

void shapeFall(){
	if(!willShapeCollide(DOWN)){
		switch(currentShape.type){
			case STRAIGHT:
				if(currentShape.orientation == UP || currentShape.orientation == DOWN){
					board[currentShape.position[0] + 3][currentShape.position[1]] = EMPTY_CELL;
					board[currentShape.position[0] - 1][currentShape.position[1]] = currentShape.color;
				} else if(currentShape.orientation == LEFT || currentShape.orientation == RIGHT){
					for(int i = 0; i < 4; i++){
						board[currentShape.position[0]][currentShape.position[1] + i] = EMPTY_CELL;
						board[currentShape.position[0] - 1][currentShape.position[1] + i] = currentShape.color;
					}
				}
				break;
			case BLOCK:
				board[currentShape.position[0] + 1][currentShape.position[1]] = EMPTY_CELL;
				board[currentShape.position[0] + 1][currentShape.position[1] + 1] = EMPTY_CELL;
				board[currentShape.position[0] - 1][currentShape.position[1]] = currentShape.color;
				board[currentShape.position[0] - 1][currentShape.position[1] + 1] = currentShape.color;
				break;
			case Z:
				if(currentShape.orientation == UP || currentShape.orientation == DOWN){
					board[currentShape.position[0] + 1][currentShape.position[1]] = EMPTY_CELL;
					board[currentShape.position[0] + 2][currentShape.position[1] + 1] = EMPTY_CELL;
					board[currentShape.position[0] - 1][currentShape.position[1]] = currentShape.color;
					board[currentShape.position[0]][currentShape.position[1] + 1] = currentShape.color;
				} else if(currentShape.orientation == LEFT || currentShape.orientation == RIGHT){
					board[currentShape.position[0]][currentShape.position[1]] = EMPTY_CELL;
					board[currentShape.position[0] + 1][currentShape.position[1] + 1] = EMPTY_CELL;
					board[currentShape.position[0] + 1][currentShape.position[1] + 2] = EMPTY_CELL;
					board[currentShape.position[0] - 1][currentShape.position[1]] = currentShape.color;
					board[currentShape.position[0] - 1][currentShape.position[1] + 1] = currentShape.color;
					board[currentShape.position[0]][currentShape.position[1] + 2] = currentShape.color;
				}
				break;
			case L:
				switch(currentShape.orientation){
					case UP:
						board[currentShape.position[0]][currentShape.position[1]] = EMPTY_CELL;
						board[currentShape.position[0]][currentShape.position[1] + 1] = EMPTY_CELL;
						board[currentShape.position[0] + 1][currentShape.position[1] + 2] = EMPTY_CELL;
						board[currentShape.position[0] - 1][currentShape.position[1]] = currentShape.color;
						board[currentShape.position[0] - 1][currentShape.position[1] + 1] = currentShape.color;
						board[currentShape.position[0] - 1][currentShape.position[1] + 2] = currentShape.color;
						break;
					case DOWN:
						board[currentShape.position[0] + 1][currentShape.position[1]] = EMPTY_CELL;
						board[currentShape.position[0] + 1][currentShape.position[1] + 1] = EMPTY_CELL;
						board[currentShape.position[0] + 1][currentShape.position[1] + 2] = EMPTY_CELL;
						board[currentShape.position[0]][currentShape.position[1] + 1] = currentShape.color;
						board[currentShape.position[0]][currentShape.position[1] + 2] = currentShape.color;
						board[currentShape.position[0] - 1][currentShape.position[1]] = currentShape.color;
						break;
					case LEFT:
						board[currentShape.position[0] + 2][currentShape.position[1]] = EMPTY_CELL;
						board[currentShape.position[0] + 2][currentShape.position[1] - 1] = EMPTY_CELL;
						board[currentShape.position[0] - 1][currentShape.position[1]] = currentShape.color;
						board[currentShape.position[0] + 1][currentShape.position[1] - 1] = currentShape.color;
						break;
					case RIGHT:
						board[currentShape.position[0] + 2][currentShape.position[1]] = EMPTY_CELL;
						board[currentShape.position[0]][currentShape.position[1] + 1] = EMPTY_CELL;
						board[currentShape.position[0] - 1][currentShape.position[1]] = currentShape.color;
						board[currentShape.position[0] - 1][currentShape.position[1] + 1] = currentShape.color;
						break;
				}
				break;
			case T:
				switch(currentShape.orientation){
					case UP:
						board[currentShape.position[0]][currentShape.position[1]] = EMPTY_CELL;
						board[currentShape.position[0] + 1][currentShape.position[1] + 1] = EMPTY_CELL;
						board[currentShape.position[0]][currentShape.position[1] + 2] = EMPTY_CELL;
						board[currentShape.position[0] - 1][currentShape.position[1]] = currentShape.color;
						board[currentShape.position[0] - 1][currentShape.position[1] + 1] = currentShape.color;
						board[currentShape.position[0] - 1][currentShape.position[1] + 2] = currentShape.color;
						break;
					case DOWN:
						board[currentShape.position[0] + 1][currentShape.position[1] - 1] = EMPTY_CELL;
						board[currentShape.position[0] + 1][currentShape.position[1]] = EMPTY_CELL;
						board[currentShape.position[0] + 1][currentShape.position[1] + 1] = EMPTY_CELL;
						board[currentShape.position[0]][currentShape.position[1] - 1] = currentShape.color;
						board[currentShape.position[0] - 1][currentShape.position[1]] = currentShape.color;
						board[currentShape.position[0]][currentShape.position[1] + 1] = currentShape.color;
						break;
					case LEFT:
						board[currentShape.position[0] + 1][currentShape.position[1] - 1] = EMPTY_CELL;
						board[currentShape.position[0] + 2][currentShape.position[1]] = EMPTY_CELL;
						board[currentShape.position[0]][currentShape.position[1] - 1] = currentShape.color;
						board[currentShape.position[0] - 1][currentShape.position[1]] = currentShape.color;
						break;
					case RIGHT:
						board[currentShape.position[0] + 2][currentShape.position[1]] = EMPTY_CELL;
						board[currentShape.position[0] + 1][currentShape.position[1] + 1] = EMPTY_CELL;
						board[currentShape.position[0] - 1][currentShape.position[1]] = currentShape.color;
						board[currentShape.position[0]][currentShape.position[1] + 1] = currentShape.color;
						break;
				}
				break;
		}
		currentShape.position[0]--;
	} else {
		getNextShape();
	}
}

void moveLeft(){
	
}

void moveRight(){
	
}

void rotate(){
	
}

void tetrisDisplay(){
	if(gameState = PLAYING){
		drawBoard();
		drawNextShapeView();
	}
}

void reshape(int width, int height){
	glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	tetrisDisplay();
}

void timer(int value){
	int newValue = value;
	if(gameState == PLAYING){
		
		if (newValue < 5){
			newValue++;
		} else {
			shapeFall();
			newValue = 0;
		}

		glutPostRedisplay();
		glutTimerFunc(50, timer, newValue);
	}
}

void keyboard(int key, int x, int y){
	switch(key){
		case GLUT_KEY_DOWN:
			shapeFall();
			break;
		case GLUT_KEY_UP:
			getNextShape();
			rotate();
			break;
		case GLUT_KEY_LEFT:
			moveLeft();
			break;
		case GLUT_KEY_RIGHT:
			moveRight();
			break;
		case 27:
			exit(0);
			break;
	}
}

int main(int argc, char* argv[]){

	initBoard();
	nextShape.type = STRAIGHT;
	nextShape.color = rand() % 6;
	nextShape.orientation = UP;
	getNextShape();

	glutInit(&argc, argv);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Tetris");

	glOrtho(-WINDOW_WIDTH, WINDOW_WIDTH, -WINDOW_HEIGHT, WINDOW_HEIGHT,-1, 1);

	glutReshapeFunc(reshape);
	glutDisplayFunc(tetrisDisplay);
	glutTimerFunc(500, timer, 0);
	glutSpecialFunc(keyboard);

	glutMainLoop();
	return 0;

}