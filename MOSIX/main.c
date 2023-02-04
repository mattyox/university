// Mateusz Wolowczyk Anna Mrukwa
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>

int main(int argc, char *argv[])
{ 
	int k = atoi(argv[1]);
	int n = atoi(argv[2]);
	int i, j;
	int pid;
	int pipearray[2];
	pipe(pipearray);
	float res = 0;
	float par_res = 0;
	
	printf("k: %d\n", k);
	printf("n: %d\n", n);
 
	for (i = 0; i < k; i++)
	{
		pid = fork();
		if (pid > 0)
		{		
			for (j = 0; j < (n/k); j++)
			{ 
				par_res += pow((pow((i*(n/k)+j+1), 2) - 12),3) / (pow(n,3)*pow((i*(n/k)+j+1), 3));
			}
			printf("par res: %.2f\n", par_res);
			write(pipearray[1], &par_res, sizeof(int));
			exit(0);
		}
	}
	
	for (i = 0; i < k; i++)
	{	printf("res: %.2f\n", res);
		read(pipearray[0], &par_res, sizeof(int));
		printf("par res out: %.2f\n", par_res);
		res += par_res;
		printf("res: %.2f\n", res);
	}
	printf("res: %.2f\n", res);
	return 0;
}

