/*
 \file		P.c
 \author	${user}
 \date		${date}
 \brief		Simple Hello World! for the Ev3
*/

#include <ev3.h>

#define X_LENGTH 16
#define Y_LENGTH 12
#define OBS_SIDE 0.305
#define MAX_OBSTACLES 25
#define K_V 0.08
#define TURN_ANGLE 183
#define FWD_ANGLE 350

int START_DIRECT = 1;
int x,y,dist;

typedef struct node{
	int x;
	int y;
	int distance;
	bool goal;
	bool start;
	struct node* next;
}node;

struct Queue {
	struct node *front;
	struct node *back;
};

int error_B = 0; //Left wheel
int error_C = 0; //Right wheel

void create_grid(node a[X_LENGTH][Y_LENGTH]){
	node temp;
	int xc;
	int i = 0;
	int j = 0;

	for(j = 0; j < Y_LENGTH; j++){
		for(i = 0; i < X_LENGTH/2; i++){
			temp = a[i][j];
			a[i][j] = a [X_LENGTH - i - 1][j];
			a[X_LENGTH - i - 1][j] = temp;
			xc = a[i][j].x;
			a[i][j].x = a[X_LENGTH - i - 1][j].x;
			a[X_LENGTH - i - 1][j].x = xc;
		}
	}
	/*
		for(j = 0; j < X_LENGTH; j++){
		for(i = 0; i < Y_LENGTH/2; i++){
			temp = a[j][i];
			a[j][i] = a [j][Y_LENGTH - i - 1];
			a[j][Y_LENGTH - i - 1] = temp;
			xc = a[j][i].y;
			a[j][i].y = a[j][Y_LENGTH - i - 1].y;
			a[j][Y_LENGTH - i - 1].y = yc;
		}
	}*/
}

void grid_fill(node grid[X_LENGTH][Y_LENGTH], double obstacle[MAX_OBSTACLES][2], double start[2], double goal[2]){

	int i,j;
	for(i = 0; i <X_LENGTH; i++)
	{
		for(j = 0; j <Y_LENGTH; j++)
		{
			grid[i][j].x = i;
			grid[i][j].y = j;
			grid[i][j].distance = 29;
			grid[i][j].goal = false;
			grid[i][j].start = false;
		}
	}
	int x, y;
	int k;
	for(k = 0; i < MAX_OBSTACLES && obstacle[k][0] != -1; k++)
	{
		x = round(obstacle[k][1] / OBS_SIDE);
		y = round(obstacle[k][0] / OBS_SIDE);
		grid[x][y].distance = -1;
	}
	x = round(start[1] / OBS_SIDE) - 1;
	y = round(start[0] / OBS_SIDE);
	grid[x][y].distance = 0;
	grid[x][y].goal = true;

	int m;
	for(m = 0; m < Y_LENGTH; m++)
	{
		grid[X_LENGTH - 1][m].distance = -1;
	}
}

void print_grid(node grid[X_LENGTH][Y_LENGTH])
{
	int i = 0;
	int j = 0;
	for(i = 0; i < X_LENGTH; i++)
	{
		printf("\n");
		for(j = 0; j < Y_LENGTH; j++)
		{
			printf("%d", grid[i][j].distance);
		}
	}
}

struct Queue* createQueue()
{
	//Parameter used to be unsigned cap
	struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));

	q->front = q->front = NULL;
	return q;
}


void enqueue (struct Queue* q, struct node* key)
{
	if(q->back == NULL)
	{
		q->front = q->back = key;
		return;
	}

	q->back->next = key;
	q->back = key;
}


void dequeue (struct Queue* q)
{
	if(q->front == NULL)
	{
		return;
	}
	q->front = q->front->next;

	if(q->front == NULL)
	{
		q->back = NULL;
	}
}


node* Goal(node area[X_LENGTH][Y_LENGTH])
{
	int i, j;
	for(i = 0; i < X_LENGTH; i++)
	{
		for(j = 0; j < Y_LENGTH; j++)
		{
			if(area[i][j].goal == 1)
			{
				return &area[i][j];
			}
		}
	}
}

node Start(node area[X_LENGTH][Y_LENGTH])
{
	int i, j;
	for(i = 0; i < X_LENGTH; i++)
	{
		for(j = 0; j < Y_LENGTH; j++)
		{
			if(area[i][j].start == 1)
			{
				return (area[i][j]);
			}
		}
	}
}

int Manhattan(node grid[X_LENGTH][Y_LENGTH])
{
	int i, j;
	int jump;

	bool checked[X_LENGTH][Y_LENGTH];
	for(i = 0; i < X_LENGTH; i++)
	{
		for(j = 0; j < Y_LENGTH; j++)
		{
			checked[i][j] = 0;
		}
	}

	struct Queue* q;
	struct node *goal = Goal(grid);
	q = createQueue(160);

	checked[goal->x][goal->y] = 1;
	enqueue(q, goal);

	while(q->front != NULL)
	{
		node* curr = q->front;
		dequeue(q);

		checked[curr->x][curr->y]=1;

		if(curr->distance == 29)
			{
				dist = curr ->distance;
				x = curr->x;
				y = curr->y;
				jump =  curr->distance;
				return jump;
			}
		if(curr->x + 1 < X_LENGTH)
		{
			node* end = &grid[curr->x + 1][curr->y];
			if(end->distance != -1 && !checked[end->x][end->y])
			{
				end->distance = (curr->distance) + 1;
				enqueue(q,end);
			}
		}

		if(curr -> x - 1 >= 0)
		{
			node* front = &grid[curr -> x - 1][curr-> y];
			if(front -> distance != -1 && !checked[front->x][front->y])
			{
				front -> distance = (curr -> distance) + 1;
				enqueue(q, front);
			}
		}

		if(curr -> y + 1 < Y_LENGTH)
		{
			node* right = &grid[curr-> x][curr -> y + 1];
			if(right -> distance != 1 && !checked[right->x][right->y])
			{
				right->distance = (curr -> distance) + 1;
				enqueue(q, right);
			}
		}

		if(curr -> y - 1 >= 0)
		{
			node* left = &grid[curr->x][curr->y - 1];
			if(left -> distance != -1 && !checked[left -> x][left -> y])
			{
				left -> distance = (curr -> distance) + 1;
				enqueue(q, left);

			}
		}
		else if(curr->distance > 29)
		{
			TermPrintln("Goal unreachable!");
			Wait(2000);
			jump = 0;
			return jump;
		}
	}
}

struct node* newNode(int x, int y, int distance, bool goal, bool start)
{
	struct node* q = (struct node*)malloc(sizeof(struct node));
	q->distance = distance;
	q->goal = goal;
	q->start = start;
	q->x=x;
	q->y=y;
	q->next=NULL;
	return q;
}

//top = 1, bottom = 2, right = 3, left = 4
node* BreadthFirstSearch(node grid[X_LENGTH][Y_LENGTH], node* curr)
{
	node *bottom, *top, *right, *left;


	if(curr->x + 1 < X_LENGTH)
	{
		bottom = &grid[curr->x + 1][curr->y];
	}


	if(curr->x - 1 >= 0)
	{
		top = &grid[curr->x - 1][curr->y];
	}


	if(curr->y + 1 < Y_LENGTH)
	{
		right = &grid[curr->x][curr->y + 1];
	}


	if(curr->y - 1 >=0)
	{
		left = &grid[curr->x][curr->y - 1];
	}

	int i=0;
	while(true)
	{
			if(curr->distance > bottom->distance && bottom->distance != -1)
			{
				TermPrintln("returning b");
				Wait(1000);
				return bottom;
			}

			if (curr->distance > top->distance && top->distance != -1)
			{
				TermPrintln("returning t");
				Wait(1000);
				return top;
			}

			if (curr->distance > left->distance && left->distance != -1)
			{
				TermPrintln("returning left");
				Wait(1000);
				return left;
			}

			if (curr->distance > right->distance && right->distance != -1)
			{
				TermPrintln("returning right");
				Wait(1000);
				return right;
			}

	}
	if(i>100)
	{
		Off(OUT_BC);
		TermPrintf(1, "\n%s : X %d | Y: %d", "Stuck at", curr->x,curr->y);
		Wait(1000);
		ButtonWaitForPressAndRelease(BTNCENTER);
		FreeEV3();

	}
}

//top = 1, bottom = 2, right = 3, left = 4
int BreadthFirstSearch2(node grid[X_LENGTH][Y_LENGTH], node* curr)
{
	node *bottom, *top, *right, *left;
	int inter;


	if(curr->x + 1 < X_LENGTH)
	{
		bottom = &grid[curr->x + 1][curr->y];
	}


	if(curr->x - 1 >= 0)
	{
		top = &grid[curr->x - 1][curr->y];
	}


	if(curr->y + 1 < Y_LENGTH)
	{
		right = &grid[curr->x][curr->y + 1];
	}


	if(curr->y - 1 >=0)
	{
		left = &grid[curr->x][curr->y - 1];
	}

	int direct[4]={1, 2, 3, 4};
	int i=0;
	while(true)
	{
			if(curr->distance > bottom->distance && bottom->distance != -1)
			{
				inter = 2;
				return inter;
			}

			if (curr->distance > top->distance && top->distance != -1)
			{
				inter = 1;
				return inter;
			}

			if (curr->distance > left->distance && left->distance != -1)
			{
				inter = 4;
				return inter;

			}

			if (curr->distance > right->distance && right->distance != -1)
			{
				inter = 3;
				return inter;
			}

			else
			{
				inter = direct[rand()%4];
				i++;
				return inter;
			}
	}
}



void move_forward(){
	TermPrintf("Moving forward\n");
	int initialCountB = MotorRotationCount(OUT_B) + error_B;
	int initialCountC = MotorRotationCount(OUT_C) + error_C;

	while((((FWD_ANGLE - MotorRotationCount(OUT_B) - initialCountB) > 10) && (FWD_ANGLE - MotorRotationCount(OUT_C) - initialCountC) > 10)){
		OnFwdSync(OUT_BC, 0.04*(FWD_ANGLE - abs(MotorRotationCount(OUT_B) - initialCountB)) + 3);
		Wait(100);
	}
	RotateMotor(OUT_B, 3, (FWD_ANGLE - abs(MotorRotationCount(OUT_B) - initialCountB))- 5);
	RotateMotor(OUT_C, 3, (FWD_ANGLE - abs(MotorRotationCount(OUT_C) - initialCountC))- 5);
}

void left_turn_90(){

	int currentCountB = MotorRotationCount(OUT_B) + error_B;
	int currentCountC = MotorRotationCount(OUT_C) + error_C;

	OnFwdReg(OUT_B, K_V*(TURN_ANGLE - (MotorRotationCount(OUT_B) - currentCountB)) + 3);
	OnFwdReg(OUT_C, K_V*(TURN_ANGLE - (MotorRotationCount(OUT_C) - currentCountC)) + 3);
	TermPrintf("TL - 90\n");
	Wait(100);
	RotateMotor(OUT_B, 15, 15);
	RotateMotor(OUT_C, 15, 370);
	TermPrintf("Ending\n");
	Wait(100);

}

void right_turn_90(){
	int currentCountC = MotorRotationCount(OUT_C) + error_C;
	int currentCountB = MotorRotationCount(OUT_B) + error_B;

	OnFwdReg(OUT_C, K_V*(TURN_ANGLE - (MotorRotationCount(OUT_C) - currentCountC)) + 3);
	OnFwdReg(OUT_B, K_V*(TURN_ANGLE - (MotorRotationCount(OUT_B) - currentCountB)) + 3);
	TermPrintf("TR - 90\n");
	Wait(100);
	RotateMotor(OUT_C, 15, 15);
	RotateMotor(OUT_B, 15, 365);
	TermPrintf("Ending\n");
	Wait(100);

}

int main(void)
{

	InitEV3();
	//ButtonWaitForPress(BUTTON_ID_ENTER);
	ResetRotationCount(OUT_B);
	ResetRotationCount(OUT_C);

	/*int num_obstacles = 13;
	double obstacle[MAX_OBSTACLES][2] = {{0.61, 2.743},{0.915, 2.743},{1.219, 2.743},{1.829, 1.219},{1.829, 1.524},{1.829, 1.829},{1.829, 2.134},{2.743, 0.305},{2.743, 0.61},{2.743, 0.915},{2.743, 2.743},{3.048, 2.743},{3.353, 2.743},{-1, -1},{-1, -1},{-1, -1},{-1, -1},{-1, -1},{-1, -1},{-1, -1},{-1, -1},{-1, -1},{-1, -1},{-1, -1},{-1, -1}};
	double start[2] = {0.305, 1.219};
	double goal[2] = {3.658, 1.829};
	*/

	/*int num_obstacles = 20;
	double obstacle[MAX_OBSTACLES][2]  = {{0.914, 0.305},{0.914, 0.61},{0.914,0.914},{0.916,1.22},{0.916,1.524},{0.916,1.83},{1.83,1.526},{1.83,1.83},{1.83,2.13},{1.83,2.66},{1.83,2.74},{1.83,0.61},{2.13,0.61},{2.66,0.61},{2.74,0.61},{2.74,0.305},{2.76,1.83},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}};
	double start[2] = {0.305, 0.61};
	double goal[2] = {3.355, 1.524};*/
/* 12 x 10
	int num_obstacles = 20;                         // number of obstacles

	double obstacle[MAX_OBSTACLES][2] =             //obstacle locations
	{{0.915, 0.305},{0.915, 0.61},{0.915, 0.915},{0.915, 1.219},
	 {0.915, 1.524},{0.915, 1.829}, {1.829, 1.524},{1.829, 1.829},
	 {1.829, 2.13},{1.829, 2.44},{1.829, 2.743},{1.829, 0.61},
	 {2.13, 0.61}, {2.44, 0.61}, {2.743, 0.61},{2.743, 0.305},
	 {2.743, 0.915},{2.743, 1.22},{2.743, 1.524},{2.743, 1.83},
	 {-1,-1},{-1,-1},{-1,-1}, {-1,-1},{-1,-1}};

	double start[2] = {0.305, 0.61};          // start location

	double goal[2] = {3.356, 1.524};           // goal location
*/
	int num_obstacles = 22;                         /* number of obstacles         */

	double obstacle[MAX_OBSTACLES][2] =        /* obstacle locations          */
	{{0.915, 0.305},{0.915, 0.61},{0.915, 0.915},{0.915, 1.219},
	 {0.915, 1.524},{0.915, 1.829}, {1.829, 1.524},{1.829, 1.829},
	 {1.829, 2.13},{1.829, 2.44},{1.829, 2.743},{1.829, 3.05},
	 {1.829, 3.355},{1.829, 0.61},{2.13, 0.61}, {2.44, 0.61},
	 {2.743, 0.61},{2.743, 0.305},{2.743, 0.915},{2.743, 1.22},
	 {2.743, 1.524},{2.743, 1.83},{-1,-1},{-1,-1},{-1,-1}, {-1,-1},
	 {-1,-1}};

	double start[2] = {0.305, 0.61};          /* start location              */

	double goal[2] = {3.356, 1.524};           /* goal location               */

	srand(time(NULL));

	node grid[X_LENGTH][Y_LENGTH];
	grid_fill(grid, obstacle, start, goal);
	create_grid(grid);

	int checkJump;
	checkJump = Manhattan(grid);

	node tmp = Start(grid);
	node* curr = &tmp;

	int curr_direct = START_DIRECT;
	int next_direct = 0;

	if(checkJump > 0)
		{
			curr->distance=dist;
			curr->x=x;
			curr->y=y;
			goto unreached;
		}

	unreached:
	while(curr->distance != 0)
	{
		node *next = BreadthFirstSearch(grid, curr);
		next_direct= BreadthFirstSearch2(grid, curr);
		//TermPrintln("next direct: %d", next_direct);
		//Wait(1000);

		 if (curr_direct == next_direct)
			 {
				//TermPrintf("2\n");
				//Wait(500);
				TermPrintf("Curr == Next: %d = %d \n", curr_direct, next_direct);
				Wait(100);
				ResetRotationCount(OUT_B);
				ResetRotationCount(OUT_C);
				move_forward();
				Wait(1000);
				curr = next;
			 }
		 else if(curr_direct != next_direct)
				{
					//TermPrintf("1\n");
					//Wait(500);
					if(curr_direct == 3)
					{
						//change this to 2
						if(next_direct == 2)
						{
							TermPrintln("1 - Curr - Right, Next - T, turn left");
							Wait(100);
							left_turn_90();
							Wait(2000);
							ResetRotationCount(OUT_B);
							ResetRotationCount(OUT_C);
							move_forward();
							Wait(2000);
						}
						//change this to 1
						else if(next_direct == 1)
						{
							TermPrintln("2 - Curr - Right, Next - B, turn right");
							Wait(100);
							right_turn_90();
							Wait(2000);
							ResetRotationCount(OUT_B);
							ResetRotationCount(OUT_C);
							move_forward();
							Wait(2000);
						}
						else
						{
							TermPrintf("Error in case r!\n");
						}
					}
					if(curr_direct == 4)
					{
						if(next_direct == 1)
						{
							TermPrintln("3 - Curr - Left, Next - T, turn left");
							Wait(100);
							right_turn_90();
							Wait(2000);
							ResetRotationCount(OUT_B);
							ResetRotationCount(OUT_C);
							move_forward();
							Wait(2000);
						}
						else if(next_direct == 2)
						{
							TermPrintln("4 - Curr - Left, Next - B, turn right");
							Wait(100);
							left_turn_90();
							Wait(2000);
							ResetRotationCount(OUT_B);
							ResetRotationCount(OUT_C);
							move_forward();
							Wait(2000);
						}
						else
						{
							TermPrintf("Error in case l!\n");
						}
					}
					if((curr_direct == 1) || (curr_direct == 2))
					{
						if(next_direct == 4)
						{
							TermPrintln("t/b - Left");
							Wait(1000);
							left_turn_90();
							Wait(2000);
							ResetRotationCount(OUT_B);
							ResetRotationCount(OUT_C);
							move_forward();
							Wait(2000);
						}
						else if(next_direct == 3)
						{
							TermPrintln("t/b - Right");
							Wait(1000);
							right_turn_90();
							Wait(2000);
							ResetRotationCount(OUT_B);
							ResetRotationCount(OUT_C);
							move_forward();
							Wait(2000);
						}
						else
						{
							TermPrintf("Error in case t or b\n");
						}
					}

				}
		 else
		 {
			TermPrintf("Everything Broken - RIP!\n");
		 }

		curr_direct = next_direct;
		curr = next;

	}

	if(curr->distance == 0)
	{
		TermPrintf("Goal Reached!\n");
	}

	Off(OUT_BC);
	TermPrintf("Press ENTER to exit");
	ButtonWaitForPress(BUTTON_ID_ENTER);
	FreeEV3();

	return 0;
}



