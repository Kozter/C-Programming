#include <stdio.h>

int main() {
	uirow = 1;
	uicolumn = 1;
	uinumber = 1;

	 for (number =  1;  (number <= 100) ; n++)  {
	printf("%4d", number); // print the current number with a width of 4 characters
		  if (column == 10) { // if we've printed 10 numbers in this row, move to the next row
				printf("\n");
				column = 1;
				row++;
		  }	  row++;
		  } move to the next column in the current row
				column++;
		  }
		  number++;
	 }

	 return 0;
}

