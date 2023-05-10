/***********************************************************************************************************
 * This program is a flight management system that allows the user to add, retrieve, and delete flights
 * as well as add, delete, and change passenger reservations. The program displays a menu of options
 ***********************************************************************************************************/

#include <string.h>
#include "source.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * This function asks for details of a flight from the user and stores them in the given parameters.
 * @param flight_id A pointer to a character array where the flight ID will be stored.
 * @param destination A pointer to a character array where the flight destination will be stored.
 * @param num_seats A pointer to an integer variable where the number of seats on the flight will be stored.
 * @param departure_time A pointer to a character array where the departure time of the flight will be stored.
 * @param num_passengers A pointer to an integer variable where the number of passengers on the flight will be stored.
 */
void getFlightDetails(char *flight_id, char *destination, int *num_seats,
		char *departure_time, int *num_passengers) {
	printf("Flight ID: ");
	fgets(flight_id, 10, stdin);
	flight_id[strcspn(flight_id, "\n")] = '\0';  // remove the newline character

	printf("Destination: ");
	fgets(destination, 50, stdin);
	destination[strcspn(destination, "\n")] = '\0'; // remove the newline character

	printf("Number of seats: ");
	char input[50];
	fgets(input, 50, stdin);
	*num_seats = atoi(input);

	printf("Departure Time (hh:mm AM/PM): ");
	fgets(departure_time, 10, stdin);
	departure_time[strcspn(departure_time, "\n")] = '\0'; // remove the newline character

	printf("Number of passengers: ");
	fgets(input, 50, stdin);
	*num_passengers = atoi(input);
}

/**
 * Prompts the user to enter details of a passenger and adds the passenger to the given passenger list.
 * @param plist A pointer to the passenger list to add the passenger to.
 */
void getPassengerDetails(Passenger *plist) {
	int id, age;
	char name[50];

	printf("Passenger Seat: ");
	char id_str[50];
	fgets(id_str, sizeof(id_str), stdin);
	sscanf(id_str, "%d", &id);

	printf("Passenger Name: ");
	fgets(name, sizeof(name), stdin);
	name[strcspn(name, "\n")] = '\0'; // remove trailing newline

	printf("Passenger Age: ");
	char age_str[50];
	fgets(age_str, sizeof(age_str), stdin);
	sscanf(age_str, "%d", &age);

	addPassenger(plist, id, name, age);
}
/**
 * Prompts the user to input the details of a new flight, creates a new passenger list
 * for the flight, and adds the flight to the flight list.
 * @param list A pointer to the flight list to which the flight will be added
 */
void addFlights(FlightList *list) {

	char flight_id[10], destination[50], departure_time[10];
	int num_seats, num_passengers;

	getFlightDetails(flight_id, destination, &num_seats, departure_time,
			&num_passengers);
	Passenger *plist = createPassengerList();
	addFlight(list, flight_id, destination, num_seats, departure_time, plist);

}

/*
 * ask the user for the flight id and search for the flight
 * using findFlightById(FlightList *list, char *flight_id),
 * from the flight returned get the passenger_list and call
 * the getPassengerDetails(Passenger *plist) and pass the passenger list
 */
void addPassengerToFlight(FlightList *list) {
	char flight_id[10];
	printf("Enter the flight ID: ");
	fgets(flight_id, 10, stdin);
	flight_id[strcspn(flight_id, "\n")] = '\0'; // remove trailing newline character

	Flight *flight = findFlightById(list, flight_id);
	if (flight == NULL) {
		printf("Flight with ID %s not found.\n", flight_id);
		return;
	}

	Passenger *plist = flight->passenger_list;
	getPassengerDetails(plist);
	printf("Passenger added to flight: %s.\n", flight_id);
}
/**
 * asks for user for the flight id and passes the flight id to the function
 * retrieveFlight(FlightList *list, char* flight_id)
 */
void getFlight(FlightList *list) {
	char flight_id[10];
	printf("Enter flight ID: ");
	fgets(flight_id, 10, stdin);
	flight_id[strcspn(flight_id, "\n")] = '\0'; // remove newline character

	retrieveFlight(list, flight_id);
}
/**
 * Prompts the user to input the time in the format of "hh:mm AM/AP" and passes it to the
 * findFlightByTime() function to retrieve the list of flights that match the given departure time
 * and prints them to the console.
 * @param list a pointer to the FlightList structure
 */
void getFlightByTime(FlightList *list) {
	char time[10];
	printf("Enter the time (hh:mm AM/AP): ");
	fgets(time, 10, stdin);
	time[strcspn(time, "\n")] = '\0'; // remove newline character

	findFlightByTime(list, time);
}

/**
 * This function prompts the user to enter a flight ID to delete and calls the deleteFlight() function
 * to remove the corresponding flight from the given FlightList.
 * @param list a pointer to the FlightList to delete from
 */
void delFlight(FlightList *list) {
	char flight_id[10];
	printf("Enter flight ID to Delete: ");
	fgets(flight_id, 10, stdin);
	flight_id[strcspn(flight_id, "\n")] = '\0'; // remove newline character

	deleteFlight(list, findFlightById(list, flight_id));
}

/**
 * This function prompts the user to enter a flight ID to delete its corresponding passenger list,
 * and frees the memory associated with each passenger in the list.
 * @param list a pointer to the FlightList to modify
 */
void deletePassengerReservation(FlightList *list) {
	char flight_id[10];
	printf("Enter flight ID to Delete it's Passenger List: ");
	fgets(flight_id, 10, stdin);
	flight_id[strcspn(flight_id, "\n")] = '\0'; // remove newline character

	Flight *flight = findFlightById(list, flight_id);
	Passenger *plist = flight->passenger_list;
	freePassengers(flight->passenger_list);
	flight->passenger_list->next = NULL;

}
/**
 * This function prompts the user to enter a flight ID and the name of a passenger, and then prompts
 * the user to enter the passenger's new seat number. It then calls the changeSeat() function to update
 * the passenger's seat number in the given FlightList.
 * @param list a pointer to the FlightList to modify
 */
void changePassengerSeat(FlightList *list) {
	char flight_id[10];
	printf("Enter Flight ID With Passenger: ");
	fgets(flight_id, 10, stdin);
	flight_id[strcspn(flight_id, "\n")] = '\0'; // remove newline character

	Flight *flight = findFlightById(list, flight_id);
	if(flight == NULL){
		printf("\tNo Flight Found with ID: %s.\n",flight_id);
		return;
	}
	Passenger *plist = flight->passenger_list;

	char name[50];
	printf("Enter The Name of the Passenger: ");
	fgets(name, 50, stdin);
	name[strcspn(name, "\n")] = '\0'; // remove newline character

	int seat;
	printf("New Passenger Seat: ");
	char id_str[50];
	fgets(id_str, sizeof(id_str), stdin);
	sscanf(id_str, "%d", &seat);

	changeSeat(plist, getPassenger(plist, name), seat);

}

/**
 * This function searches for a passenger by their name in all flights within the given FlightList.
 * @param list A pointer to the FlightList to search within.
 */
void searchPassengerByName(FlightList *list) {
	char name[10];
	printf("Enter The Name: ");
	fgets(name, 50, stdin);
	name[strcspn(name, "\n")] = '\0'; // remove newline character
	// Start at the head of the list
	Flight *curr_flight = list->head;

	int results = 0;
	// Traverse the list
	while (curr_flight != NULL) {
		if (searchPassenger(curr_flight->passenger_list, name) == 1) {
			printf("\t\tFound in Flight ID: %s\n", curr_flight->flight_id);
			results = 1;
		}
		curr_flight = curr_flight->next;
	}

	if(results == 0)
		printf("\t\tNo Passenger found with name: %s.\n",name);
}

/**
 * The main function initializes a FlightList pointer, which serves as the head of the linked list of flights.
 * It then enters a loop that displays a menu of options and prompts the user for input to carry out the chosen option.
 * The program uses switch statements to call the appropriate functions depending on the user's choice.
 * Functions, include adding a flight to the list, adding a passenger to a flight, retrieving a flight from the list,
 * finding a flight by departure time, deleting a flight, deleting a passenger reservation, changing a passenger's seat,
 * and searching for a passenger by name.
 * The addPassenger function has been modified to ensure that the seat number being added is not already in the list.
 **/

int main() {
	FlightList *list = createFlightList();

	int choice = 0;
	while (choice != 9) {
		printf("\n--- Flight Management System ---\n");
		printf("0. Print the Flight List\n");
		printf("1. Add a flight to the list\n");
		printf("2. Add a passenger to a flight\n");
		printf("3. Retrieve a flight from the list\n");
		printf("4. Find flight by departure time\n");
		printf("5. Delete a flight\n");
		printf("6. Delete a passenger reservation\n");
		printf("7. Change passenger seat\n");
		printf("8. Search passenger by name\n");
		printf("9. Exit\n");
		printf("\nEnter your choice: ");
		scanf("%d", &choice);
		getchar();

		switch (choice) {
		case 0:
			printFlightList(list);
			break;
		case 1:
			addFlights(list);
			break;
		case 2:
			addPassengerToFlight(list);
			break;
		case 3:
			getFlight(list);
			break;
		case 4:
			getFlightByTime(list);
			break;
		case 5:
			delFlight(list);
			break;
		case 6:
			deletePassengerReservation(list);
			break;
		case 7:
			changePassengerSeat(list);
			break;
		case 8:
			searchPassengerByName(list);
			break;
		case 9:
			printf("Exiting program...\n");
			break;
		default:
			printf("Invalid choice, please try again\n");
		}
	}
	return EXIT_SUCCESS;
}

