#ifndef FLIGHT_H
#define FLIGHT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "passenger.h"

typedef struct flight {
	char flight_id[10];
	char destination[50];
	int num_seats;
	char departure_time[10];
	Passenger *passenger_list;
	struct flight *next;
	struct flight *prev;
} Flight;

typedef struct flightList {
	Flight *head;
	Flight *tail;
} FlightList;

#include "flight.h"

// Creates and initializes a new FlightList
FlightList* createFlightList() {
	FlightList *list = (FlightList*) malloc(sizeof(FlightList));
	list->head = NULL;
	list->tail = NULL;
	return list;
}

// Adds a new Flight to the FlightList
void addFlight(FlightList *flight_list, char flight_id[], char destination[],
		int num_seats, char departure_time[], Passenger *passenger_list) {
	// Create a new Flight
	Flight *new_flight = (Flight*) malloc(sizeof(Flight));
	strcpy(new_flight->flight_id, flight_id);
	strcpy(new_flight->destination, destination);
	new_flight->num_seats = num_seats;
	strcpy(new_flight->departure_time, departure_time);
	new_flight->passenger_list = passenger_list;
	new_flight->next = NULL;
	new_flight->prev = NULL;

	// Add the new Flight to the FlightList
	if (flight_list->head == NULL) {
		flight_list->head = new_flight;
		flight_list->tail = new_flight;
	} else {
		new_flight->prev = flight_list->tail;
		flight_list->tail->next = new_flight;
		flight_list->tail = new_flight;
	}
}
void printFlight(Flight *flight) {
	printf("-----------------------------------------------------------------------------\n");
	printf("Flight ID: %s\n", flight->flight_id);
	printf("Destination: %s\n", flight->destination);
	printf("Num Seats: %d\n", flight->num_seats);
	printf("Departure Time: %s\n", flight->departure_time);
	printPassengers(flight->passenger_list);
	printf("-----------------------------------------------------------------------------\n");
}
// Retrieves the nth Flight from the FlightList and prints its details
void retrieveFlight(FlightList *list, char *flight_id) {
	Flight *curr_flight = list->head;
	int count = 1;

	// Traverse the list to the nth Flight
	while (curr_flight != NULL) {
		if (strcmp(curr_flight->flight_id, flight_id) == 0)
			break;
		curr_flight = curr_flight->next;
	}

	// Print the Flight details and its passenger list
	if (curr_flight != NULL) {
		printFlight(curr_flight);

	} else {
		printf(" Error: ==> Flight ID: %s Not Found.\n", flight_id);
	}
}

// Finds the index of the Flight with the given departure_time
int findFlightByTime(FlightList *list, char *departure_time) {
	Flight *curr_flight = list->head;
	int count = 1;

	// Traverse the list to find the Flight with the given departure_time
	while (curr_flight != NULL) {
		if (strcmp(curr_flight->departure_time, departure_time) == 0) {
			printFlight(curr_flight);
		}
		curr_flight = curr_flight->next;
		count++;
	}

	// Flight not found
	return -1;
}
// Finds the index of the Flight with the given departure_time
Flight* findFlightById(FlightList *list, char *flight_id) {
	Flight *curr_flight = list->head;
	int count = 1;

	// Traverse the list to find the Flight with the given departure_time
	while (curr_flight != NULL) {
		if (strcmp(curr_flight->flight_id, flight_id) == 0) {
			return curr_flight;
		}
		curr_flight = curr_flight->next;
		count++;
	}

	// Flight not found
	return NULL;
}

// Deletes a Flight from the FlightList
void deleteFlight(FlightList *list, Flight *flight) {
	// Remove the Flight from the list
	if (flight == list->head) {
		list->head = flight->next;
	}
	if (flight == list->tail) {
		list->tail = flight->prev;
	}
	if (flight->prev != NULL) {
		flight->prev->next = flight->next;
	}
	if (flight->next != NULL) {
		flight->next->prev = flight->prev;
	}

	// Free the memory associated with the Flight and its passenger list
	freePassengers(flight->passenger_list);
	free(flight);
}

// Prints the FlightList and its Flight details
void printFlightList(FlightList *list) {
	// Start at the head of the list
	Flight *curr_flight = list->head;

	printf(
			"******************************\tFlight List *********************************\n");
	int count = 1;
	// Traverse the list and print each Flight's details
	while (curr_flight != NULL) {

		printFlight(curr_flight);

		curr_flight = curr_flight->next;
		count++;
	}
	if (count == 1) {
		printf("\t\t\t\tList is Empty\n");
	}
	printf(
			"****************************************************************************\n");
}

// Frees the memory associated with a FlightList and its Flights and passenger lists
void freeFlightList(FlightList *flight_list) {
	Flight *curr_flight = flight_list->head;
	// Traverse the list and free each Flight and its passenger list
	while (curr_flight != NULL) {

		freePassengers(curr_flight->passenger_list);

		Flight *temp = curr_flight;
		curr_flight = curr_flight->next;
		free(temp);
	}

	// Free the FlightList itself
	free(flight_list);
}
#endif

