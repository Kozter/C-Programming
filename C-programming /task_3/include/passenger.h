#ifndef PASSENGER_H
#define PASSENGER_H

#include <stdio.h>
#include <stdlib.h>

typedef struct passenger {
	int seat_number;
	char name[50];
	int age;
	struct passenger *next;
} Passenger;

// Initializes the list
Passenger* createPassengerList() {
	Passenger *head = (Passenger*) malloc(sizeof(Passenger));
	head->seat_number = 0;
	head->name[0] = '\0';
	head->age = 0;
	head->next = NULL;
	return head;
}

// Add a passenger to the linked list in ascending order of seat number
void addPassenger(Passenger *head, int seat_number, char *name, int age) {
	// Check if the seat number already exists in the list
	Passenger *current = head;
	while (current->next != NULL && current->next->seat_number <= seat_number) {
		if (current->next->seat_number == seat_number) {
			printf("Error: Seat number %d is already taken.\n", seat_number);
			return;
		}
		current = current->next;
	}

	Passenger *new_passenger = (Passenger*) malloc(sizeof(Passenger));
	new_passenger->seat_number = seat_number;
	strcpy(new_passenger->name, name);
	new_passenger->age = age;
	new_passenger->next = NULL;

	current = head;
	while (current->next != NULL && current->next->seat_number < seat_number) {
		current = current->next;
	}
	new_passenger->next = current->next;
	current->next = new_passenger;
}

// Print the contents of the linked list
void printPassengers(Passenger *head) {
	Passenger *current = head->next;
	printf("~~~~~~~~~~~~~~~\tPassengers ~~~~~~~~~~~~~~~~~~\n");
	int count = 1;
	while (current != NULL) {
		printf(" Name: %s, Seat number: %d, Age: %d\n",
				current->name,current->seat_number, current->age);
		current = current->next;
		count++;
	}
	if(count == 1){ printf("\t\tList is Empty\n");}
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

// Delete a passenger's reservation on a flight
void deletePassenger(Passenger *head, Passenger *passenger) {
	Passenger *current = head;
	while (current->next != NULL && current->next != passenger) {
		current = current->next;
	}
	if (current->next == passenger) {
		current->next = passenger->next;
		free(passenger);
	}
}

// Change the seat of a passenger
void changeSeat(Passenger *head, Passenger *passenger, int new_seat) {
	Passenger *current = head;
	if(passenger == NULL)
	{
		printf("\tThe passenger not found.\n");
		return;
	}
	if(current == NULL){
		printf("\tThe passenger is empty.\n");
		return;
	}
	while (current != NULL) {
		if (current->name == passenger->name) {
			char name[50];
			int seat = new_seat;
			int age = passenger->age;
			strcpy(name,passenger->name);
			deletePassenger(head, current);
			addPassenger(head, seat, name, age);
			break;
		}
		current = current->next;
	}
}
//gets the passenger given the list and the name of the passenger
Passenger* getPassenger(Passenger *head, const char *name) {
	Passenger *current = head->next;

	while (current != NULL) {
		if (strcmp(current->name, name) == 0) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}
// Search for a passenger by name and print all flights associated with them
int searchPassenger(Passenger *head, const char *name) {
	Passenger *current = head->next;
	int found = 0;
	while (current != NULL) {
		if (strcmp(current->name, name) == 0) {
			printf(" Name: %s, Seat number: %d, Age: %d\n",
							current->name,current->seat_number, current->age);
			found = 1;
		}
		current = current->next;
	}
	return found;
}

// Free the memory used by the linked list
void freePassengers(Passenger *head) {
	Passenger *current = head;
	Passenger *next;
	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}
}

#endif

