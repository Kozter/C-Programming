#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// This function reads the contents of a file and returns a pointer to a buffer containing the data.
// It also sets the value pointed to by "size" to the size of the file in bytes.
char* read_file(const char *filename, long *size) {
	FILE *file = fopen(filename, "r");
	if (!file) {
		// Print an error message if the file could not be opened
		perror("Error opening input file");
		// Exit the program with a failure status
		exit(EXIT_FAILURE);
	}

	// Move the file pointer to the end of the file
	fseek(file, 0, SEEK_END);
	// Get the size of the file in bytes and store it in the "size" variable
	*size = ftell(file);
	// Move the file pointer back to the beginning of the file
	rewind(file);

	// Allocate memory for the buffer to hold the file contents
	char *buffer = malloc(*size + 1);
	if (!buffer) {
		// Print an error message if memory allocation failed
		perror("Error allocating memory for input buffer");
		exit(EXIT_FAILURE);
	}

	// Read the contents of the file into the buffer
	if (fread(buffer, *size, 1, file) != 1) {
		// Print an error message if the file could not be read
		perror("Error reading input file");
		exit(EXIT_FAILURE);
	}

	// Close the file
	fclose(file);

	// Add a null terminator to the end of the buffer to create a C-style string
	buffer[*size] = '\0';

	// Return a pointer to the buffer containing the file contents
	return buffer;
}

// This function writes the contents of the given buffer to a file with the given filename.
void write_file(const char *filename, const char *buffer) {
	// Open the file in write mode
	FILE *file = fopen(filename, "w");
	if (!file) {
		// Print an error message if the file could not be opened
		perror("Error opening output file");
		exit(EXIT_FAILURE);
	}

	if (fputs(buffer, file) == EOF) {
		// Print an error message if the file could not be written
		perror("Error writing output file");
		exit(EXIT_FAILURE);
	}

	// Close the file
	fclose(file);
}

// This function replaces all while loops in the input buffer with equivalent for loops.
void replace_while_loops_with_for_loops(char *input_buffer) {
	// Find the first occurrence of "while" in the buffer
	char *while_pos = strstr(input_buffer, "while");

	// Continue while there are still while loops in the buffer
	while (while_pos != NULL) {
		// Find the start of the while loop
		char *loop_start = strchr(while_pos, '{');
		if (loop_start == NULL) {
			// Print an error message if the loop is missing an opening brace
			fprintf(stderr, "Error: Missing opening brace for while loop\n");
			exit(EXIT_FAILURE);
		}

		// Find the end of the while loop
		char *loop_end = strchr(loop_start, '}');
		if (loop_end == NULL) {
			fprintf(stderr, "Error: Missing closing brace for while loop\n");
			exit(EXIT_FAILURE);
		}

		// Get the loop condition
		char *cond_start = while_pos + strlen("while");
		char *cond_end = loop_start - 1;
		char loop_cond[cond_end - cond_start + 2];
		//copy the characters in between
		strncpy(loop_cond, cond_start, cond_end - cond_start + 1);
		// adds a null terminator.
		loop_cond[cond_end - cond_start + 1] = '\0';

		// Get the loop body
		char *body_start = loop_start + 1;
		char loop_body[loop_end - body_start + 1];
		//copies the characters in between
		strncpy(loop_body, body_start, loop_end - body_start);
		// adds a null terminator.
		loop_body[loop_end - body_start] = '\0';

		// Get the loop increment
		char *inc_start = strstr(loop_end, "++");
		if (inc_start == NULL) {
			// Print an error message if the loop is missing an increment statement
			fprintf(stderr, "Error: Missing loop increment\n");
			exit(EXIT_FAILURE);
		}

		// Find the loop increment, by going back from "++" to the previous non-whitespace character
		// Note that inc_end points to the second '+' in "++", so we start by decrementing inc_start by one.
		char *inc_end = inc_start + 2;
		while (isspace(*--inc_start))
			;
		//declares the variable
		char loop_inc[inc_end - inc_start + 1];
		//copies the characters in between to loop_inc.
		strncpy(loop_inc, inc_start, inc_end - inc_start);
		// adds a null terminator.
		loop_inc[inc_end - inc_start] = '\0';

		// Find the initialization variable
		char *init_end = while_pos;
		while (*init_end != ';') {
			init_end--;
		}
		char *init_start = init_end;
		while (*init_start != '=') {
			init_start--;
		}
		init_start++;
		char loop_init[init_end - init_start + 2];
		//copies the characters in between
		strncpy(loop_init, init_start, init_end - init_start + 1);
		// adds a null terminator.
		loop_init[init_end - init_start + 1] = '\0';

		// Build the for loop
		char for_loop[strlen(loop_cond) + strlen(loop_inc) + strlen(loop_body)
				+ 14];
		sprintf(for_loop, "for (number = %s %s; %s)  {%s}", loop_init,
				loop_cond, loop_inc, loop_body);

		// Replace the while loop with the for loop
		int loop_len = loop_end - while_pos + 1;
		//insert the for loop
		memmove(while_pos, for_loop, strlen(for_loop));
		//insert the remaining part of the buffer
		memmove(while_pos + strlen(for_loop), loop_end + 1,
				strlen(loop_end + 1));
		while_pos = strstr(while_pos + strlen(for_loop), "while");
	}
}
/*
 * This function renames variables in the input buffer that have the type "unsigned int".
 * It first finds the position of "unsigned int" in the buffer, then finds the end of
 * the variable name and renames it by prepending "ui" to the original name. Finally,
 * it removes the "unsigned int" string from the buffer and advances to the next
 * occurrence of "unsigned int" to repeat the process.
 * @param input_buffer the buffer that contains the code
 */
void rename_unsigned_int_vars(char *input_buffer) {
	const char *unsigned_int_str = "unsigned int ";
	size_t unsigned_int_str_len = strlen(unsigned_int_str);

	char *pos = input_buffer;
	while ((pos = strstr(pos, unsigned_int_str)) != NULL) {
		// Find the end of the variable name
		char *var_name_start = pos + unsigned_int_str_len;
		char *var_name_end = var_name_start;
		while (isalnum(*var_name_end) || *var_name_end == '_') {
			++var_name_end;
		}

		// Rename the variable
		size_t var_name_len = var_name_end - var_name_start;
		char new_var_name[var_name_len + 3];
		new_var_name[0] = 'u';
		new_var_name[1] = 'i';
		strncpy(new_var_name + 2, var_name_start, var_name_len);
		new_var_name[var_name_len + 2] = '\0';
		memmove(pos + 2, var_name_start, var_name_len);
		memcpy(pos, new_var_name, 2);

		// Remove the "unsigned int" string
		memmove(pos + 2 + var_name_len, var_name_end, strlen(var_name_end) + 1);

		// Advance to the next occurrence of "unsigned int"
		pos = pos + 2;
	}
}
/*
 * Replaces sequences of three spaces in the input buffer with a single tab character.
 * @param input_buffer a pointer to a character array containing the input buffer
 *
 * Note: The function iterates over the input buffer and replaces any occurrences of three
 * consecutive space characters with a single tab character. The function modifies the
 * input buffer in-place.
 */
void replace_spaces_with_tab(char *input_buffer) {
	size_t buffer_len = strlen(input_buffer);
	char *pos = input_buffer;
	while ((pos = strstr(pos, "   ")) != NULL) {
		// Replace 3 spaces with a tab character
		*pos++ = '\t';
		memmove(pos, pos + 2, buffer_len - (pos - input_buffer) + 1);
		buffer_len -= 2;
	}
}

/**
 * This function reads a C source code file from the disk, applies several
 * transformations to beautify the code, and writes the result to a new file.
 * The transformations include:
 *   - Replacing while loops with for loops
 *   - Renaming unsigned int variables
 *   - Replacing 3 consecutive spaces with a tab character
 *
 * The beautified code is then written to a new file in the same directory as the
 * input file, with "_beautified.c" appended to the filename. The original input
 * file contents are output to the console, and the name of the new file is also
 * output to the console.
 *
 * @param input_filename The path to the C source code file to be beautified.
 */
void beautify(const char *input_filename) {
	long input_size;
	char *input_buffer = read_file(input_filename, &input_size);

	replace_while_loops_with_for_loops(input_buffer);
	rename_unsigned_int_vars(input_buffer);
	replace_spaces_with_tab(input_buffer);

	// Write the beautified output to a new file
	char output_filename[256];
	char fname[30];

	size_t len = strlen(input_filename) - 2;
	strncpy(fname, input_filename, len);
	fname[len] = '\0';
	snprintf(output_filename, sizeof(output_filename), "%s_beautified.c",
			fname);
	write_file(output_filename, input_buffer);
	// Output the contents of the input file to the console
	printf("Input file contents:\n%s\n", input_buffer);
	free(input_buffer);

	printf("file saved as %s\n", output_filename);
}
/*
 * This function ensures that it is the filename has a valid C source code file with the correct extension.
 *
 * This function takes a filename as input and validates it to ensure it has the .c extension.
 * The function first checks if the filename has a length of at least 3 characters because a
 * valid filename with extension ".c" should have at least 3 characters (ex. a.c).
 * If the filename is too short, the function prints an error message and exits with a failure code.
 *
 * If the filename is long enough, the function checks if the last two characters in the string are equal to ".c"
 * using the strcmp function. If the filename does not have a valid extension, the function prints
 * an error message and exits with a failure code.
 */
void validate_filename(const char *filename) {
	size_t len = strlen(filename);
	if (len < 3) {
		// Prints an error
		printf("Filename is too short to have an extension\n");
		exit(EXIT_FAILURE);
	}
	if (!strcmp(&filename[len - 2], ".c") == 0) {
		printf("Filename has an invalid extension\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * The main function that calls validate_filename function, the beautify function to beautify the code
 * in a c file by replacing while loops with for loops, renaming unsigned int variables,
 * and replacing three spaces with a tab character.
 */
int main(int argc, char *argv[]) {
	// Check that the filename was provided
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	validate_filename(argv[1]);
	beautify(argv[1]);

	return EXIT_SUCCESS;
}
