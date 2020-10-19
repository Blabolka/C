#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define UP	1
#define DOWN 2
#define LEFT	3
#define RIGHT	4

#define AREASIZEX	18
#define AREASIZEY	10		

typedef struct{	//Structure for snake parameters
	int way;
	int x,y;
}Snake;

typedef struct{ //Structure for food parameters
	int x,y;
}Food;

void show_game_area(Snake *snake, int sizeOfSnake, int sizeOfSnakeArray);	//Render function
Snake* move(Snake *snake, int sizeOfSnake);	//Snake control function
Food spawn_food(Snake *snake, Food food, int sizeOfSnake);	//Food spawn function
void setcur(int x, int y);	//Console cursor position setting function
void change_background_color();	//Background and text change function
void change_keyboard_language();	//Keyboard layout change function
void change_console_settings();	//Console font change function
void disable_cursor_blinking();	//Cursor blink off function

const unsigned int miliseconds = 300;	//Turn Allowed Time (in milliseconds)

int main(){
	
	change_background_color();
	change_keyboard_language();
    change_console_settings();
    disable_cursor_blinking();
    
    SetConsoleTitle("Snake");	//Changes the name of the name of the opened window
    
	Snake *snake;
	int sizeOfSnakeArray = 5;	//The size of the dynamically allocated memory for the snake
	snake = (Snake*)malloc(sizeOfSnakeArray*sizeof(Snake));
	int sizeOfSnake = 2;
	
	srand(time(NULL));
	
	snake[0].way = RIGHT;
	snake[0].x = 4;
	snake[0].y = 2;
	snake[1].x = 3;
	snake[1].y = 2;

	show_game_area(snake,sizeOfSnake,sizeOfSnakeArray);
	
	return 0;
}

void show_game_area(Snake *snake, int sizeOfSnake, int sizeOfSnakeArray){
	
	Food food;
	food = spawn_food(snake,food,sizeOfSnake);
	
	int i;
	int flag1=0; //Responsible for touching the edge of the map
	int flag2=0; //Responsible for checking render the seal of the snake piece
	int flag3=0; //Responsible for the touch of the snake to food
	int xFact=0, yFact=0; //The actual draw position of the field (will be compared with x, y to set the point of the object)
		
	while(1){
	    setcur(0,0);
		for(i=0 ; i<=AREASIZEX+1 ; i++){
			printf("-");
		}
		printf("\n");
		for(yFact=0 ; yFact<AREASIZEY ; yFact++){
			printf("|");
			for(xFact=0 ; xFact<AREASIZEX ; xFact++){
				
				/*For checking the collision of the snake's head with tail*/
				for(i=1 ; i<sizeOfSnake ; i++){
					if(snake[0].x == snake[i].x && snake[0].y == snake[i].y){
						system("cls");
						printf("YOU DIED!!!\n");
						system("pause > nul");
						return;
					}
				}
				
				/*For drawing the snake*/
				for(i=0 ; i<sizeOfSnake ; i++){
					if(snake[i].x == xFact && snake[i].y == yFact){
						printf("@");
						if(i==0){
							flag1=1;
						}
						flag2=1;
					}
				}
				
				/*For drawing food, checking for touching the snake's head and increasing the snake array*/
				if(food.x == snake[0].x && food.y == snake[0].y){
					food = spawn_food(snake,food,sizeOfSnake);
					if(sizeOfSnake == sizeOfSnakeArray){
						snake = (Snake*)realloc(snake,2*sizeOfSnakeArray*sizeof(Snake));
						sizeOfSnakeArray *= 2;
					}
					sizeOfSnake++;
				}
				if(food.x == xFact && food.y == yFact){
					printf("*");
					flag2=1;
				}
				
				if(flag2==0){ //Place a space if there is nothing to draw
					printf(" ");
				}
				flag2=0;	
				
			}
			printf("|\n");
		}
		
		for(i=0 ; i<=AREASIZEX+1 ; i++){
			printf("-");
		}
		if(flag1==0){
			system("cls");
			printf("YOU DIED!!!\n");
			system("pause > nul");
			return;
		}
		flag1=0;
		snake = move(snake,sizeOfSnake);
	}
}

Snake* move(Snake *snake, int sizeOfSnake){
	
	char pov;
	int i;
	int tempX=snake[0].x,tempY=snake[0].y,tempWay=snake[0].way;
	
	clock_t period = clock() + miliseconds;
	while(period > clock()){
		fflush(stdin);
		if(_kbhit()){
			pov = _getch();
			if(pov == 'w' && snake[0].way != UP && snake[0].way != DOWN){
				snake[0].way = UP;
				snake[0].y--;
			}else if(pov == 's' && snake[0].way != DOWN && snake[0].way != UP){
				snake[0].way = DOWN;
				snake[0].y++;
			}else if(pov == 'd' && snake[0].way != RIGHT && snake[0].way != LEFT){
				snake[0].way = RIGHT;
				snake[0].x++;
			}else if(pov == 'a' && snake[0].way != LEFT && snake[0].way != RIGHT){
				snake[0].way = LEFT;
				snake[0].x--;
			}else{
				break;
			}
			
			for(i=sizeOfSnake-1 ; i>0 ; i--){
				if(i==1){
					snake[i].way = tempWay;
					snake[i].x = tempX;
					snake[i].y = tempY;
				}else{
					snake[i].way = snake[i-1].way;
					snake[i].x = snake[i-1].x;
					snake[i].y = snake[i-1].y;
				}
			}
			return snake;
		}
	}
	if(snake[0].way == UP){
		snake[0].y--;
	}else if(snake[0].way == DOWN){
		snake[0].y++;
	}else if(snake[0].way == LEFT){
		snake[0].x--;
	}else{
		snake[0].x++;
	}
	
	for(i=sizeOfSnake-1 ; i>0 ; i--){ //Changing the coordinates of the snake's body (not counting the head)
		if(i==1){
			snake[i].x = tempX;
			snake[i].y = tempY;
		}else{
			snake[i].way = snake[i-1].way;
			snake[i].x = snake[i-1].x;
			snake[i].y = snake[i-1].y;
		}
	}
	return snake;
}

Food spawn_food(Snake *snake, Food food, int sizeOfSnake){
	
	int i,flag=0;
	
	while(1){
		food.x = rand()%AREASIZEX;
		food.y = rand()%AREASIZEY;
		for(i=0 ; i<sizeOfSnake ; i++){
			if(food.x == snake[i].x && food.y == snake[i].y){
				flag=1;
			}
		}
		if(flag==0){
			break;
		}
		flag=0;
	}
	
	return food;
}

void setcur(int x, int y){
	/*Changes cursor position (works like clearing faster than system("cls"))*/
	COORD coord;
    coord.X = 0;
	coord.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void change_background_color(){
	system("color 0F");	//Changes the text color to black and the background to white
	/*The first value is responsible for the background color, the second for the text color*/
}

void change_keyboard_language(){
	/*Changes the keyboard layout for the console window to English*/
	PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, 1, 0x04090409);	
}

void change_console_settings(){
	/*Changes the font, its size and thickness*/
    CONSOLE_FONT_INFOEX cfi;	
    cfi.cbSize          = sizeof(CONSOLE_FONT_INFOEX);
    cfi.nFont           = 6;
    cfi.dwFontSize.X    = 7;
    cfi.dwFontSize.Y    = 16;
    cfi.FontFamily      = 54;
    cfi.FontWeight      = 400;
    wcscpy(cfi.FaceName, L"Lucida Console");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), 0, &cfi);
}

void disable_cursor_blinking(){
	/*Disable cursor blinking (blinking cursor is clearly visible on Windows 10)*/
	CONSOLE_CURSOR_INFO cci;
    cci.dwSize=99;
    cci.bVisible=0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cci);
}




