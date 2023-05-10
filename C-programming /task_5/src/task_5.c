
/**
 * The program is a client/server network application.
 * The program takes command line arguments and based on the arguments passed it either starts
 *  the program as a server or a client.
 *  The program first initializes the variables for server_ip, port_number, and userid.
 *  Next, the program calls the parse_arguments function to extract the necessary information from
 *  the command line arguments.
 *  The function then validates the arguments passed using the validate_arguments function.
 *  Finally, the program starts either the server or client depending on the user ID.
 *  If the user ID is 0, it means that the program is started as a server and the start_server function is called.
 *  Otherwise, the program is started as a client and the start_client function is called.
 *  The function returns EXIT_SUCCESS constant number on successful program execution.
 */
#include "source.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Main function that starts the program either as a server or client based on the given command line arguments.
 */
int main(int argc, char *argv[]) {
	char *server_ip = NULL;
	int port_number = 0;
	int userid = 0;

	if (argc != 5 && argc != 4 ) {
		printf(
				"Usage: %s <server/client> <IP address> <port number>\nArguments: %d\n",
				argv[0], argc);
		exit(EXIT_FAILURE);
	}
	parse_arguments(argc, argv, &server_ip, &port_number, &userid);
	validate_arguments(server_ip, port_number, userid);

	if (userid == 0) {
		start_server(server_ip, port_number);
	} else {
		start_client(server_ip, port_number);
	}

	return (EXIT_SUCCESS);
}
