#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdbool.h>
#define rows 3
#define cols 3

clock_t begin, end;
int timeTaken;
int Turn_Count2 = 0;
int score = 0, s, highest;
int totaltime = 0;

int i, row, col;
bool done;

char arr[9] = { '_', '_', '_', '_', '_', '_', '_', '_', '_' };

void *No_Of_Turns(void *p)
{
	for (int i = 0; i < 9; i++)
	{
		if (arr[i] == 'O')
			Turn_Count2++;
	}

	printf("No Of Turns Taken by Player 2: %d ", Turn_Count2);
	printf("\n");

}

void *startTime(void *p)
{
	time(&begin);
}

void *getTime(void *p)
{
	time(&end);
	timeTaken = (int)(difftime(end, begin));
	totaltime = totaltime + timeTaken;
}

void *HighScore(void *p)
{
	FILE *fptr2 = fopen("score.txt", "r");

	int c;
	while ((c = fgetc(fptr2)) != EOF){
		fscanf(fptr2, "%d", &s);

		if (highest < s)
		{
			highest = s;
		}
	}

	fclose(fptr2);
	printf("\n");
	printf("\nHighest Score is: %d", highest);
	printf("\n");
}


void display()
{
	printf("\n   ");
	for (int i = 0; i < 3; i++)
	{
		printf("%d ", i);
		printf("  ");
	}
	printf("\n");
	for (int j = 0; j < 3; j++)
	{
		printf("%d", j);
		printf("  ");
		for (int i = 0; i < 3; i++)
		{
			printf("%c ", arr[j * 3 + i]);
			printf("  ");
		}
		printf("\n\n");
	}
}
void ComputerAI(char player, int no, int spaces)
{
	done = false;
	int Ncount = 0, Scount = 0;
	//Horizontal
	int i = 0, j, x;
	while (i < rows&&!done)
	{
		j = 0;
		Ncount = 0, Scount = 0;
		while (j < cols)
		{
			if (arr[i*rows + j] == '_')
			{
				Scount++;
				x = j;
			}
			else if (arr[i*rows + j] == player)
				Ncount++;
			j++;
		}
		if (Ncount == no && Scount == spaces)
		{
			arr[i*rows + x] = 'O';
			row = i;
			col = x;
			done = true;
		}
		i++;
	}
	//Vertical
	if (!done)
	{
		i = 0, x;
		while (i < cols&&!done)
		{
			j = 0;
			Ncount = 0, Scount = 0;
			while (j < rows)
			{
				if (arr[j*rows + i] == '_')
				{
					Scount++;
					x = j;
				}
				else if (arr[j*rows + i] == player)
					Ncount++;
				j++;
			}
			if (Ncount == no && Scount == spaces)
			{
				arr[x*rows + i] = 'O';
				row = x;
				col = i;
				done = true;
			}
			i++;
		}
	}
	//Leftdiagonal
	if (!done)
	{
		i = 0, x;
		Ncount = 0, Scount = 0;
		while (i < rows)
		{
			if (arr[i*rows + i] == '_')
			{
				Scount++;
				x = i;
			}
			else if (arr[i*rows + i] == player)
				Ncount++;
			i++;
		}
		if (Ncount == no && Scount == spaces)
		{
			arr[x*rows + x] = 'O';
			row = col = x;
			done = true;
		}
	}
	//Rightdiagonal
	if (!done)
	{
		i = rows - 1, x;
		Ncount = 0, Scount = 0;
		while (i >= 0)
		{
			if (arr[i*rows + cols - i - 1] == '_')
			{
				Scount++;
				x = i;
			}
			else if (arr[i*rows + cols - i - 1] == player)
				Ncount++;
			i--;
		}
		if (Ncount == no && Scount == spaces)
		{
			arr[x*rows + cols - x - 1] = 'O';
			row = x;
			col = cols - x - 1;
			done = true;
		}
	}
}
int computermove()
{
	ComputerAI('O', 2, 1);
	if (!done)
		ComputerAI('X', 2, 1);
	if (!done)
		ComputerAI('O', 1, 2);
	if (!done)
		ComputerAI('X', 1, 2);
	/*if (!done)
	{
	row = rand() % rows, col = rand() % cols;
	while (arr[row*rows + col] != '_')
	{
	row = rand() % rows; col = rand() % cols;
	}
	arr[row*rows + col] = 'O';
	printf("\nUse of random");
	}*/
	return row*rows + col;
}

char check()
{
	for (int i = 0; i < 3; i++)
	{
		if (arr[i] == arr[i + 3] && arr[i + 3] == arr[i + 6])
			return arr[i];
	}


	for (int i = 0; i < 3; i++)
	{
		if (arr[i * 3 + 0] == arr[i * 3 + 1] && arr[i * 3 + 1] == arr[i * 3 + 2])
			return arr[i * 3];
	}

	if (arr[0] == arr[4] && arr[0] == arr[8])
	{
		return arr[0];
	}
	if (arr[2] == arr[4] && arr[2] == arr[6])
	{
		return arr[2];
	}

	int count = 0;
	for (int i = 0; i < 9; i++)
	{
		if (arr[i] != '_')
		{
			count++;
		}
	}

	if (count == 9)
	{
		return 's';
	}

	else
		return '_';

}


void play()
{
	system("clear");
	int fd1;
	int input;

	char * myfifo = "/tmp/myfifo";
	mkfifo(myfifo, 0666);
	display();

	while (1)
	{
		fd1 = open(myfifo, O_RDONLY);
		read(fd1, &input, sizeof(input));
		arr[input] = 'X';
		close(fd1);

		system("clear");
		printf("Player1: %d\n", input);
		display();

		if (check() != '_')
			break;
		pthread_t tid1, tid2;

		pthread_create(&(tid1), NULL, startTime, NULL);

		pthread_join(tid1, NULL);
		input = computermove();
		sleep(5);
		pthread_create(&(tid2), NULL, getTime, NULL);
		pthread_join(tid2, NULL);

		system("clear");

		printf("Time taken for turn is: %d", timeTaken);
		printf(" seconds\n");
		printf("\n");
		fd1 = open(myfifo, O_WRONLY);
		write(fd1, &input, sizeof(input));
		close(fd1);

		arr[input] = 'O';
		sleep(2);		system("clear");

		display();

		if (check() != '_')
			break;

	}

	pthread_t tid3;

	pthread_create(&(tid3), NULL, No_Of_Turns, NULL);
	pthread_join(tid3, NULL);

	if (check() == 'O'){
		printf("You Won!\n");
	}
	else if (check() == 'X'){
		printf("You Loose!\n");
	}
	else if (check() == 's'){
		printf("Draw\n");
	}

	printf("\nTotal Time is: %d", totaltime);
	printf("\n");

	pid_t p = fork();
	if (p == 0)
	{
		score = (Turn_Count2 * totaltime) * 0.5;

		FILE *fptr = fopen("score.txt", "r");


		fscanf(fptr, "%d", &s);

		highest = s;

		fclose(fptr);
		FILE *fptr1 = fopen("score.txt", "a");


		fprintf(fptr1, "%d", score);
		fprintf(fptr1, "%c", '\n');

		fclose(fptr1);



	}
	else
	{

		wait(NULL);
		pthread_t tid4;
		pthread_create(&(tid4), NULL, HighScore, NULL);
		pthread_join(tid4, NULL);
	}


}
void main(){
	system("clear");
	//srand(time(NULL));
	play();
}
