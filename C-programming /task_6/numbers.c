#include <stdio.h>

int main() {
   unsigned int row = 1;
   unsigned int column = 1;
   unsigned int number = 1;

    while (number <= 100) {
   printf("%4d", number); // print the current number with a width of 4 characters
        if (column == 10) { // if we've printed 10 numbers in this row, move to the next row
            printf("\n");
            column = 1;
            row++;
        } else { // otherwise, move to the next column in the current row
            column++;
        }
        number++;
    }

    return 0;
}

