//Constant representations of the five shape types
const int BLOCK = 1, Z = 2, STRAIGHT = 3, L = 4, T = 5;

//Constant representations of the colors available in the game
const int EMPTY_CELL = 0;
const int RED = 1, GREEN = 2, BLUE = 3, PURPLE = 4, YELLOW = 5, CYAN = 6;
const int FLASH_CELL = 7;

//Constant representations of the orientations of the shape. This is used to rotate the current shape. 
//(This is also used as directions for collision detection)
const int UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3;

// A structure representing the location of a cell on the tetrisBoard
struct Position{
	int y;
	int x;
};

/* A structure representing a shape in the game. Each shape consists of 4 cells.
 *
 * Type:			Straight (I), Block, T, L or Z (All have integer representations above)
 * Color:			Red, Green, Blue, Purple, Yellow, Cyan (All have integer representations above)
 * Orientation:		Up, Right, Down, Left (All have integer representations above) 
 *					(Not all shapes have 4 orientations)
 * Cell Positions:	The position of the 4 cells of the shape on the board
 *
 * Rightmost Cells:	The cells that can collide with the board's edges or other shapes on the right
 * Leftmost Cells:	The cells that can collide with the board's edges or other shapes on the left
 * Lower Cells:		The cells that can collide with the board's bottom edge or other shapes while falling
 *
 * (In the collision arrays, the nth element is the location of the collision detection cell of the nth cell.
 *  if the nth cell is not in that specific collision array, then the nth element is assigned to a fixed cell
 *  at the top of the screen, denoting that that cell shouldn't be taking into account when moving into that
 *  respective direction)
 */
struct Shape{
	int type;
	int color;
	int orientation;
	Position cellPositions[4];

	Position rightmostCells[4];
	Position leftmostCells[4];
	Position lowerCells[4];
};

//The current shape being manipulated on the tetrisBoard 
Shape currentShape;

//The next shape that will be used.
Shape nextShape;

//Sets the position of the cells of the current shape to the passed in points
void changeCurrentShapePosition(int y0, int x0, int y1, int x1, int y2, int x2, int y3, int x3){
	currentShape.cellPositions[0].y = y0;
	currentShape.cellPositions[0].x = x0;
	currentShape.cellPositions[1].y = y1;
	currentShape.cellPositions[1].x = x1;
	currentShape.cellPositions[2].y = y2;
	currentShape.cellPositions[2].x = x2;
	currentShape.cellPositions[3].y = y3;
	currentShape.cellPositions[3].x = x3;
}

//Sets the collision cells of the respective cell if the passed parameter is 1,
//or to the fixedCollisionCell if the passed parameter is 0;
void setCollisionCells(int direction, int cell0, int cell1, int cell2, int cell3){
	
	//This position points to a fixed cell that is away from collision.
	//It is used as a substitute for the cells that aren't in the collision array in the given direction
	//This is done so that the shape can correctly identify whether it will collide with something or not
	Position fixedCollisionCell;
	fixedCollisionCell.y = 28;
	fixedCollisionCell.x = 5;

	int i;
	int collisionCells[4] = {cell0, cell1, cell2, cell3};

	switch(direction){
	case DOWN:
		for(i = 0; i < 4; i++){
			if(collisionCells[i] == 1){
				currentShape.lowerCells[i] = currentShape.cellPositions[i];
			} else {
				currentShape.lowerCells[i] = fixedCollisionCell;
			}
		}
		break;
	case RIGHT:
		for(i = 0; i < 4; i++){
			if(collisionCells[i] == 1){
				currentShape.rightmostCells[i] = currentShape.cellPositions[i];
			} else {
				currentShape.rightmostCells[i] = fixedCollisionCell;
			}
		}
		break;
	case LEFT:
		for(i = 0; i < 4; i++){
			if(collisionCells[i] == 1){
				currentShape.leftmostCells[i] = currentShape.cellPositions[i];
			} else {
				currentShape.leftmostCells[i] = fixedCollisionCell;
			}
		}
		break;
	}
}

//Updates the location of the collision of the shape, based on the type and orientation of the shape
void updateShapeCollision(){
	switch(currentShape.type){
	case STRAIGHT:
		if(currentShape.orientation == UP){
			setCollisionCells(DOWN, 1, 0, 0, 0);
			setCollisionCells(RIGHT, 1, 1, 1, 1);
			setCollisionCells(LEFT, 1, 1, 1, 1);
		} else {
			setCollisionCells(DOWN, 1, 1, 1, 1);
			setCollisionCells(RIGHT, 0, 0, 0, 1);
			setCollisionCells(LEFT, 1, 0, 0, 0);
		}
		break;
	case BLOCK:
			setCollisionCells(DOWN, 1, 1, 0, 0);
			setCollisionCells(RIGHT, 0, 1, 0, 1);
			setCollisionCells(LEFT, 1, 0, 1, 0);
		break;
	case Z:
		if(currentShape.orientation == UP){
			setCollisionCells(DOWN, 1, 0, 1, 0);
			setCollisionCells(RIGHT, 1, 1, 0, 1);
			setCollisionCells(LEFT, 1, 0, 1, 1);
		} else {
			setCollisionCells(DOWN, 1, 1, 0, 1);
			setCollisionCells(RIGHT, 0, 1, 0, 1);
			setCollisionCells(LEFT, 1, 0, 1, 0);
		}
		break;
	case L:
		switch(currentShape.orientation){
		case RIGHT:
			setCollisionCells(DOWN, 1, 1, 0, 0);
			setCollisionCells(RIGHT, 0, 1, 1, 1);
			setCollisionCells(LEFT, 1, 0, 1, 1);
			break;
		case DOWN:
			setCollisionCells(DOWN, 1, 0, 1, 1);
			setCollisionCells(RIGHT, 1, 0, 0, 1);
			setCollisionCells(LEFT, 1, 1, 0, 0);
			break;
		case LEFT:
			setCollisionCells(DOWN, 1, 0, 0, 1);
			setCollisionCells(RIGHT, 1, 1, 1, 0);
			setCollisionCells(LEFT, 1, 1, 0, 1);
			break;
		case UP:
			setCollisionCells(DOWN, 1, 1, 1, 0);
			setCollisionCells(RIGHT, 0, 0, 1, 1);
			setCollisionCells(LEFT, 1, 0, 0, 4);
			break;
		}
		break;
	case T:
		switch(currentShape.orientation){
		case RIGHT:
			setCollisionCells(DOWN, 1, 0, 1, 0);
			setCollisionCells(RIGHT, 1, 0, 1, 1);
			setCollisionCells(LEFT, 1, 1, 0, 1);
			break;
		case DOWN:
			setCollisionCells(DOWN, 1, 1, 0, 1);
			setCollisionCells(RIGHT, 1, 0, 0, 1);
			setCollisionCells(LEFT, 1, 1, 0, 0);
			break;
		case LEFT:
			setCollisionCells(DOWN, 1, 1, 0, 0);
			setCollisionCells(RIGHT, 1, 0, 1, 1);
			setCollisionCells(LEFT, 1, 1, 0, 1);
			break;
		case UP:
			setCollisionCells(DOWN, 1, 1, 1, 0);
			setCollisionCells(RIGHT, 0, 0, 1, 1);
			setCollisionCells(LEFT, 1, 0, 0, 1);
			break;
		}
		break;
	}
}
