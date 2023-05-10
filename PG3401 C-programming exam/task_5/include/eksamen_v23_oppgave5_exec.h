#ifndef EXEC_H
#define EXEC_H

/* The popen() function is a standard C library function that allows a program to open a pipe to
   a process for reading or writing. The name popen stands for "process open". The function takes
   two arguments: a string specifying the command to execute, and a string specifying the mode
   in which the pipe should be opened. The mode string should be either "r" (to open the pipe for
   reading) or "w" (to open the pipe for writing). The function returns a FILE pointer that can be
   used to read from or write to the pipe.

   The below function takes a command as argument, executes that command by calling popen, and
   returns as a string the output of that command, as would have been printed to the terminal.*/

char* ExecuteCommand(const char *pcszCommand);


/**
 * Parses command line arguments for the server IP, port number, and user ID.
 * @param argc The number of command line arguments.
 * @param argv An array of command line argument strings.
 * @param server_ip A pointer to a character array that will store the server IP.
 * @param port_number A pointer to an integer that will store the port number.
 * @param userid A pointer to an integer that will store the user ID.
 */
void parse_arguments(int argc, char *argv[], char **server_ip, int *port_number, int *userid);

/**
 * Validates the server IP, port number, and user ID passed as arguments.
 * @param server_ip The server IP to validate.
 * @param port_number The port number to validate.
 * @param userid The user ID to validate.
 **/
void validate_arguments(char *server_ip, int port_number, int userid);

/**
 * Starts the server on the specified IP address and port number.
 * @param server_ip The IP address to bind the server to.
 * @param port_number The port number to bind the server to.
 * */
void start_server(char *server_ip, int port_number);

/**
 * Connects to the server running on the specified IP address and port number.
 * @param server_ip The IP address of the server to connect to.
 * @param port_number The port number of the server to connect to.
 */
void start_client(char *server_ip, int port_number);

#endif
