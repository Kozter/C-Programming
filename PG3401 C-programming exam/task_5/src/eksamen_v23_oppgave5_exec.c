#define MAX_BUFFER_SIZE 1024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

/* The popen() function is a standard C library function that allows a program to open a pipe to 
 a process for reading or writing. The name popen stands for "process open". The function takes
 two arguments: a string specifying the command to execute, and a string specifying the mode
 in which the pipe should be opened. The mode string should be either "r" (to open the pipe for
 reading) or "w" (to open the pipe for writing). The function returns a FILE pointer that can be
 used to read from or write to the pipe.

 The below function takes a command as argument, executes that command by calling popen, and
 returns as a string the output of that command, as would have been printed to the terminal.*/

char* ExecuteCommand(const char *pcszCommand) {
	FILE *fp = NULL;
	char *pszReturnString = malloc(MAX_BUFFER_SIZE);
	if (pszReturnString == NULL)
		return NULL;
	memset(pszReturnString, 0, MAX_BUFFER_SIZE);

	fp = popen(pcszCommand, "r");
	if (fp == NULL)
		sprintf(pszReturnString, "Error: Failed to execute command");
	 else {
		if (fgets(pszReturnString, MAX_BUFFER_SIZE - 1, fp) == NULL)
			sprintf(pszReturnString, "Error: Failed to read output");
		pclose(fp);
	}
	return pszReturnString;
}

void parse_arguments(int argc, char *argv[], char **server_ip, int *port_number, int *userid) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-server") == 0){
            *server_ip = argv[i + 1];
            *userid = 1;
            i++;
        }else if (strcmp(argv[i], "-listen") == 0) {
        	*server_ip = "127.0.0.1";
            *userid = 0;
        } else if (strcmp(argv[i], "-port") == 0){
            *port_number = atoi(argv[i + 1]);
            i++;
        }else {
            printf("Invalid option: %s\n", argv[i]);
            exit(EXIT_FAILURE);
        }
    }
}

void validate_arguments(char *server_ip, int port_number, int userid) {
    if (port_number == 0) {
        printf("Port number must be specified\n");
        exit(EXIT_FAILURE);
    }
    if (server_ip == NULL) {
        printf("Server IP must be specified\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Starts the server and listens for incoming connections.
 * @param server_ip the IP address of the server
 * @param port_number the port number on which to listen for incoming connections
 *
 * Overall, the start_server() function provides a simple Bash shell interface for clients to interacting with.
 */

void start_server(char *server_ip, int port_number) {
	// create socket
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	// bind socket to address
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(server_ip);
	server_address.sin_port = htons(port_number);

	if (bind(server_socket, (struct sockaddr*) &server_address,
			sizeof(server_address)) < 0) {
		perror("Binding failed");
		exit(EXIT_FAILURE);
	}

	// listen for incoming connections
	if (listen(server_socket, 5) < 0) {
		perror("Listening failed");
		exit(EXIT_FAILURE);
	}

	printf("Server is listening on %s:%d\n", server_ip, port_number);

	// accept incoming connections
	struct sockaddr_in client_address;
	int client_socket;
	socklen_t client_address_length = sizeof(client_address);

	while ((client_socket = accept(server_socket,
			(struct sockaddr*) &client_address, &client_address_length))) {
		printf("Connection accepted from %s:%d\n",
				inet_ntoa(client_address.sin_addr),
				ntohs(client_address.sin_port));

		char command[1024];
		int command_length;

		while (1) {
			// read command from user
			printf("%s@%d>",
				inet_ntoa(client_address.sin_addr),
				ntohs(client_address.sin_port));
			fgets(command, 1024, stdin);
			command_length = strlen(command);

			// remove trailing newline character
			if (command[command_length - 1] == '\n') {
				command[command_length - 1] = '\0';
				command_length--;
			}

			// execute command
			char *result = ExecuteCommand(command);

			// send result to client
			if (send(client_socket, result, strlen(result), 0) == -1) {
				perror("Error sending result to client");
				break;
			}

			// print result to terminal
			printf("%s\n", result);

			// free memory allocated for result
			free(result);
		}

		close(client_socket);
	}

	close(server_socket);
}

/**
 * This method creates a client socket and connects it to the server specified
 * by the IP address and port number passed as arguments.
 * It then waits for commands from the server and executes them, sending the result back to the server.
 * @param server_ip The IP address of the server to connect to
 * @param port_number The port number of the server to connect to
 *
 * Notes:
 * The start_client() function implements a client-side network socket program using the
 * Transmission Control Protocol (TCP) to connect to a server specified by server_ip and port_number.
 * Once connected, the function enters an infinite loop that prompts the user to enter a Bash command,
 * sends the command to the server, and receives the result of the command execution from the server.
 * The loop continues until an error occurs or the server closes the connection.
 *
 * Overall, the start_client() function provides a simple interface for interacting with a remote Bash shell.
 */
void start_client(char *server_ip, int port_number) {
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		perror("Error creating socket");
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port_number);
	server_address.sin_addr.s_addr = inet_addr(server_ip);

	if (connect(client_socket, (struct sockaddr*) &server_address,
			sizeof(server_address)) == -1) {
		perror("Error connecting to server");
		exit(EXIT_FAILURE);
	}

	printf("Successfully connected to server %s:%d\n", server_ip, port_number);

	// execute commands sent from server
	char command[1024];
	char result[MAX_BUFFER_SIZE];
	while (1) {
		// read command from user input
		printf("Enter Bash Command: $ ");
		fgets(command, sizeof(command), stdin);
		command[strcspn(command, "\n")] = 0; // remove newline character from command

		// send command to server
		if (send(client_socket, command, strlen(command), 0) == -1) {
			perror("Error sending command to server");
			break;
		}
		// receive result from server
		int num_bytes_received = recv(client_socket, result, MAX_BUFFER_SIZE,
				0);
		if (num_bytes_received == -1) {
			perror("Error receiving result from server");
			break;
		} else if (num_bytes_received == 0) {
			printf("Server closed connection\n");
			break;
		} else {
			result[num_bytes_received] = '\0';
			printf("Result: %s\n", result);
		}
	}

	close(client_socket);
}
/* IMPORTANT NOTE: This function has alloced a string by malloc, it must be freed BY THE CALLER...*/
