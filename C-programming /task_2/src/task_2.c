/*
 ============================================================================
 Name        : task_2.c
 Author      : Your name
 Version     :
 Copyright   : Your copyright notice
 Description :
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "source.h"


int main(void) {
		FILE *fp;
		int num;

		fp = fopen("eksamen_v23_oppgave2.txt", "r");
		if (fp == NULL) {
			printf("Failed to open file.\n");
			exit(EXIT_FAILURE);
		}

		printf("Index\tNumber\tFibonacci\tPrime\tSquare\tCube\tPerfect\tDeficient\tRedundant\tOdd\n");
			int i = 1;
		    while (fscanf(fp, "%d", &num) != EOF) {
		        printf("%d\t%d\t\t%d\t%d\t%d\t%d\t%d\t%d\t\t%d\t\t%d\n", i, num,
		            isFibonacci(num), isPrime(num), isSquareNumber(num), isCubeNumber(num),
		            isPerfectNumber(num), isDeficientNumber(num), isAbundantNumber(num), isOdd(num));
		        i++;
		    }

		fclose(fp);
		return 0;

	return EXIT_SUCCESS;
}

