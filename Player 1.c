#include stdio.h
#include sysstat.h
#include systypes.h
#include fcntl.h
#include unistd.h
#include stdlib.h
#include time.h
#include string.h
#include unistd.h
#include syswait.h
#include pthread.h
clock_t begin, end;
int timeTaken;
int score = 0, s, highest;
int totaltime = 0;
int Turn_Count1 = 0;
char arr[9] = { '_', '_', '_', '_', '_', '_', '_', '_', '_' };

void No_Of_Turns(void p)
{
	for (int i = 0; i9; i++)
	{
		if (arr[i] == 'X')
			Turn_Count1++;
	}

	printf(No Of Turns Taken by Player 1 %d , Turn_Count1);
	printf(n);

}
void startTime(void p)
{
	time(&begin);
}
void getTime(void p)
{
	time(&end);
	timeTaken = (int)(difftime(end, begin));
	totaltime = totaltime + timeTaken;

}
void HighScore(void p)
{
	FILE fptr2 = fopen(score.txt, r);

	int c;
	while ((c = fgetc(fptr2)) != EOF){
		fscanf(fptr2, %d, &s);

		if (highests)
		{
			highest = s;
		}
	}

	fclose(fptr2);
	printf(n);
	printf(nHighest Score is %d, highest);
	printf(n);
}

void display()
{
	printf(n   );
	for (int i = 0; i3; i++)
	{
		printf(%d , i);
		printf(  );
	}
	printf(n);
	for (int j = 0; j3; j++)
	{
		printf(%d, j);
		printf(  );
		for (int i = 0; i3; i++)
		{
			printf(%c , arr[j  3 + i]);
			printf(  );
		}
		printf(nn);
	}

}


char check()
{
	for (int i = 0; i3; i++)
	{
		if ((arr[i] == arr[i + 3]) && (arr[i + 3] == arr[i + 6]))
			return arr[i];
	}


	for (int i = 0; i3; i++)
	{
		if ((arr[i  3 + 0] == arr[i  3 + 1]) && (arr[i  3 + 1] == arr[i  3 + 2]))
			return arr[i  3];
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
	for (int i = 0; i9; i++)
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
	system(clear);
	int fd;
	int input;
	int rowpos;
	int colpos;
	int row = 3;
	int col = 3;
	int temp = 0;
	char  myfifo = tmpmyfifo;
	mkfifo(myfifo, 0666);
	display();

	while (1)
	{


		fd = open(myfifo, O_WRONLY);
		pthread_t tid1, tid2;

		pthread_create(&(tid1), NULL, startTime, NULL);

		pthread_join(tid1, NULL);
		do
		{
			printf(Enter Row );
			scanf(%d, &rowpos);
			printf(n);
			printf(Enter Column );
			scanf(%d, &colpos);
			input = (row  rowpos) + colpos;

		} while (arr[input] == 'X'  arr[input] == 'O');
		arr[input] = 'X';

		pthread_create(&(tid2), NULL, getTime, NULL);
		pthread_join(tid2, NULL);

		write(fd, &input, sizeof(input));
		close(fd);
		system(clear);
		display();

		printf(Time taken for turn is %d, timeTaken);
		printf( secondsn);
		printf(n);
		if (check() != '_')
			break;


		fd = open(myfifo, O_RDONLY);
		read(fd, &input, sizeof(input));
		arr[input] = 'O';

		system(clear);

		printf(n);
		display();

		close(fd);
		if (check() != '_')
			break;




	}

	pthread_t tid3;

	pthread_create(&(tid3), NULL, No_Of_Turns, NULL);
	pthread_join(tid3, NULL);



	if (check() == 'X'){

		printf(You Won!n);

	}
	else if (check() == 'O'){
		printf(You Loose!n);

	}
	else if (check() == 's')
	{
		printf(Drawn);
	}


	printf(nTotal Time is %d, totaltime);
	printf(n);
	int signal;
	pid_t p = fork();
	if (p == 0)
	{
		score = (Turn_Count1  totaltime)  0.5;
		int s;
		FILE fptr = fopen(score.txt, r);


		fscanf(fptr, %d, &s);

		highest = s;

		fclose(fptr);
		FILE fptr1 = fopen(score.txt, a);


		fprintf(fptr1, %d, score);
		fprintf(fptr1, %c, 'n');

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
	system(clear);
	play();



}
