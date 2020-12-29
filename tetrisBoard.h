#include "tetrisShape.h"

//Constant integer values used to denote the current game state.
const int GAME_OVER = 0;
const int PLAYING = 1;
int gameState = PLAYING;

//Integer score. Initialized to 0 at the start of the game.
int score = 0;

//The tetris tetrisBoard is kept in a 2d array. Cells start from the bottom left (0,0) to the top right (30, 10).
int tetrisBoard[30][10];

//Boolean flag that lights up when there is a row to clear
bool clearRowsFlag = false;

//Initializes the board to empty cells
void initBoard(){
	for(int i = 0; i < 30; i++){
		for(int j = 0; j < 10; j++){
			tetrisBoard[i][j] = EMPTY_CELL;
		}
	}
}

//Clears the current shape's colors from the board. The current shape's position stays the same.
void clearShape(){
	int y, x;
	for(int i = 0; i < 4; i++){
		y = currentShape.cellPositions[i].y;
		x = currentShape.cellPositions[i].x;
		tetrisBoard[y][x] = EMPTY_CELL;
	}
}

//Fills the color of the current shape on the board
void recolorShape(){
	int y, x;
	for(int i = 0; i < 4; i++){
			y = currentShape.cellPositions[i].y;
			x = currentShape.cellPositions[i].x;
			tetrisBoard[y][x] = currentShape.color;
	}
}

void recreateCell(int cellToBeMoved, int referenceCell, int yCellOffset, int xCellOffset){
	currentShape.cellPositions[cellToBeMoved].y = currentShape.cellPositions[referenceCell].y + yCellOffset;
	currentShape.cellPositions[cellToBeMoved].x = currentShape.cellPositions[referenceCell].x + xCellOffset;
}

void recreateCell(int cellToBeMoved, int referenceCell){
	recreateCell(cellToBeMoved, referenceCell, 0, 0);
}

//Gets the next shape as the current shape, then generate a new next shape
void getNextShape(){
	
	//Checks the game has ended. Game ends when shapes can't spawn anymore
	if(tetrisBoard[19][5] != EMPTY_CELL || tetrisBoard[19][4] != EMPTY_CELL){
		gameState = GAME_OVER;
		return;
	}

	//Sets the next shape to be the current shape and spawns it at the top of the board
	currentShape = nextShape;
	switch(currentShape.type){
		case STRAIGHT:
			changeCurrentShapePosition(20, 4, 21, 4, 22, 4, 23, 4);
			break;
		case BLOCK:
			changeCurrentShapePosition(20, 4, 20, 5, 21, 4, 21, 5);
			break;
		case Z:
			changeCurrentShapePosition(20, 4, 21, 4, 21, 3, 22, 3);
			break;
		case L:
			changeCurrentShapePosition(20, 4, 20, 5, 21, 4, 22, 4);
			break;
		case T:
			changeCurrentShapePosition(20, 4, 21, 4, 21, 5, 22, 4);
			break;
	}
	recolorShape();
	updateShapeCollision();

	nextShape.type = (rand() % 5) + 1;
	nextShape.color = (rand() % 6) + 1;
	switch(nextShape.type){
		case STRAIGHT:
		case Z:
			nextShape.orientation = UP;
			break;
		default:
			nextShape.orientation = RIGHT;
	}
}

//Checks if the current shape can move an *amount* of cells in that *direction*
bool willCollide(int direction, int amount){

	int y, x, i;

	switch(direction){
	case DOWN:
		for(i = 0; i < 4; i++){
			y = currentShape.lowerCells[i].y;
			x = currentShape.lowerCells[i].x;
			//if the cell under this is (out of bounds) OR (not empty), the cell will collide
			if(y <= (-1 + amount) || tetrisBoard[y - amount][x] != EMPTY_CELL) {return true;}
		}
		break;
	case RIGHT:
		for(i = 0; i < 4; i++){
			y = currentShape.rightmostCells[i].y;
			x = currentShape.rightmostCells[i].x;
			//if the cell to the right is (out of bounds) OR (not empty), the cell will collide
			if(x >= (10 - amount) || tetrisBoard[y][x + amount] != EMPTY_CELL) {return true;}
		}
		break;
	case LEFT:
		for(i = 0; i < 4; i++){
			y = currentShape.leftmostCells[i].y;
			x = currentShape.leftmostCells[i].x;
			//if the cell to the left is (out of bounds) OR (not empty), the cell will collide
			if(x <= (-1 + amount) || tetrisBoard[y][x - amount] != EMPTY_CELL) {return true;}
		}
		break;
	}

	return false;
}

//Checks if the current shape can move 1 cell in that *direction*
bool willCollide(int direction){
	return willCollide(direction, 1);
}

//Moves the shape 1 cell to the left, if the shape won't collide with something.
void moveLeft(){
	if(!willCollide(LEFT)){
		clearShape();
		for(int i = 0; i < 4; i++){
			currentShape.cellPositions[i].x--;
		}
		recolorShape();
		updateShapeCollision();
	}	
}

//Moves the shape 1 cell to the right, if the shape won't collide with something.
void moveRight(){
	int y, x;
	if(!willCollide(RIGHT)){
		clearShape();
		for(int i = 0; i < 4; i++){
			y = currentShape.cellPositions[i].y;
			x = currentShape.cellPositions[i].x;
			currentShape.cellPositions[i].x++;
		}
		recolorShape();
		updateShapeCollision();
	}	
}

//Rotates the shape. This happens by clearing the shape from the board then recreating the cells again.
void rotate(){
	switch(currentShape.type){
	case STRAIGHT:
		if(currentShape.orientation == UP && !willCollide(RIGHT, 2) && !willCollide(LEFT)){
			clearShape();
			recreateCell(1, 0);
			recreateCell(0, 1, 0, -1);
			recreateCell(2, 1, 0, 1);
			recreateCell(3, 1, 0, 2);
			currentShape.orientation = RIGHT;
		} else if(currentShape.orientation == RIGHT){
			clearShape();
			recreateCell(0, 1);
			recreateCell(1, 0, 1, 0);
			recreateCell(2, 0, 2, 0);
			recreateCell(3, 0, 3, 0);
			currentShape.orientation = UP;
		}
		break;
	case Z:
		if(currentShape.orientation == UP && !willCollide(RIGHT)){
			clearShape();
			recreateCell(0, 2);
			recreateCell(2, 1, 1, 0);
			recreateCell(3, 1, 1, 1);
			currentShape.orientation = RIGHT;
		} else if(currentShape.orientation == RIGHT && !willCollide(DOWN)){
			clearShape();
			recreateCell(2, 0);
			recreateCell(0, 1, -1, 0);
			recreateCell(3, 1, 1, -1);
			currentShape.orientation = UP;
		}
		break;
	case L:
		switch(currentShape.orientation){
		case RIGHT:
			if(!willCollide(RIGHT)){
				clearShape();
				recreateCell(1, 2);
				recreateCell(2, 1, 0, 1);
				recreateCell(3, 1, 0, 2);
				currentShape.orientation = DOWN;
			}
			break;
		case DOWN:
			if(!willCollide(DOWN)){
				clearShape();
				recreateCell(1, 2);
				recreateCell(0, 1, -1, 0);
				recreateCell(2, 1, 1, 0);
				recreateCell(3, 1, 1, -1);
				currentShape.orientation = LEFT;
			}
			break;
		case UP:
			clearShape();
			recreateCell(0, 1);
			recreateCell(1, 2);
			recreateCell(2, 0, 1, 0);
			recreateCell(3, 0, 2, 0);
			currentShape.orientation = RIGHT;
			break;
		case LEFT:
			if(!willCollide(LEFT)){
				clearShape();
				recreateCell(3, 2);
				recreateCell(2, 1);
				recreateCell(1, 2, 0, -1);
				recreateCell(0, 2, 0, -2);
				currentShape.orientation = UP;
			}
			break;
		}
		break;
	case T:
		switch(currentShape.orientation){
		case RIGHT:
			if(!willCollide(LEFT)){
				clearShape();
				recreateCell(3, 2);
				recreateCell(2, 1);
				recreateCell(1, 2, 0, -1);
				currentShape.orientation = DOWN;
			}
			break;
		case DOWN:
			clearShape();
			recreateCell(3, 2, 1, 0);
			currentShape.orientation = LEFT;
			break;
		case LEFT:
			if(!willCollide(RIGHT)){
				clearShape();
				recreateCell(0, 1);
				recreateCell(1, 2);
				recreateCell(2, 1, 0, 1);
				currentShape.orientation = UP;
			}
			break;
		case UP:
			if(!willCollide(DOWN)){
				clearShape();
				recreateCell(0, 1, -1, 0);
				currentShape.orientation = RIGHT;
			}
			break;
		}
		break;
	}
	recolorShape();
	updateShapeCollision();
}

//Checks if there is a filled row on the board
void checkForFullRow(){
	bool lineFilled;

	for(int i = 0; i < 20; i++){
		lineFilled = true;
		for(int j = 0; j < 10; j++ ){
			if(tetrisBoard[i][j] == EMPTY_CELL) {lineFilled = false;}
		}
		if(lineFilled){
			score += 100;
			for(int j = 0; j < 10; j++){
				tetrisBoard[i][j] = FLASH_CELL;
				clearRowsFlag = true;
			}
		}
	}
}

//Moves the shape 1 cell down, if the shape won't collide with something.
void shapeFall(){
	if(!willCollide(DOWN)){
		clearShape();
		for(int i = 0; i < 4; i++){
			currentShape.cellPositions[i].y--;
		}
		recolorShape();
		updateShapeCollision();
	} else {
		checkForFullRow();
		getNextShape();
	}
}

//Makes the column fall. This function the cells of a full row
void columnFall(int rowNumber, int columnNumber){
	for(int i = rowNumber; i < 19; i++){
		tetrisBoard[i][columnNumber] = tetrisBoard[i + 1][columnNumber];
		tetrisBoard[i + 1][columnNumber] = EMPTY_CELL;
	}
}

//Clears full rows. This function is called only if the clearRowsFlag is true
void clearFullRows(){
	clearRowsFlag = false;
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 19; j++){
			if(tetrisBoard[j][i] == FLASH_CELL){
				columnFall(j, i);
			}
		}
	}
	checkForFullRow();
}
