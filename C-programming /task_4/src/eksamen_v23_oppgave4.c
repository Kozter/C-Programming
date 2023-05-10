#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE 4096
#define NUM_THREADS 3
#define BYTE_RANGE 256

//struct used to pass arguments to the threads
typedef struct {
	int count[BYTE_RANGE];
	unsigned char buffer[BUFFER_SIZE];
	//Three semaphores: full, empty, and mutex.
	sem_t full;
	sem_t empty;
	sem_t mutex;
	int bytes_in_buffer; //An integer bytes_in_buffer.
	char filename[50]; //A character array filename, with size equal to 50
} ThreadData;

void* threadA(void *arg) {
	// cast the argument to ThreadData
	ThreadData *thread_data = (ThreadData*) arg;

	// open the file for reading
	FILE *fp = fopen(thread_data->filename, "rb");
	if (!fp) {
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}
	while (1) {
		// wait for the buffer to be full
		sem_wait(&thread_data->empty);
		// wait for access to the buffer
		sem_wait(&thread_data->mutex);

		// read from the file into the buffer
		int read_bytes = fread(
				thread_data->buffer + thread_data->bytes_in_buffer, 1,
				BUFFER_SIZE - thread_data->bytes_in_buffer, fp);
		thread_data->bytes_in_buffer += read_bytes;

		// if there was an error reading from the file or if the end of the file was reached
		if (read_bytes < BUFFER_SIZE - thread_data->bytes_in_buffer) {
			// release the mutex
			sem_post(&thread_data->mutex);
			// signal that the buffer is full
			sem_post(&thread_data->full);
			break;
		}
		// release the mutex
		sem_post(&thread_data->mutex);
		// signal that the buffer is full
		sem_post(&thread_data->full);
	}
	// close the file
	fclose(fp);
	// exit the thread
	pthread_exit(NULL);
}

void* threadB(void *arg) {
	// cast the argument to ThreadData
	ThreadData *thread_data = (ThreadData*) arg;
	memset(thread_data->count, 0, sizeof(thread_data->count));

	while (1) {
		// wait for the buffer to be full
		sem_wait(&thread_data->full);
		// wait for access to the buffer
		sem_wait(&thread_data->mutex);

		for (int i = 0; i < thread_data->bytes_in_buffer; i++) {
			// Ignore characters outside the ASCII printable range
			if (thread_data->buffer[i] >= 32 && thread_data->buffer[i] <= 126) {
				thread_data->count[thread_data->buffer[i]]++;
			}
		}

		// reset the buffer and release access to it
		thread_data->bytes_in_buffer = 0;
		sem_post(&thread_data->mutex);
		// signal that the buffer is empty
		sem_post(&thread_data->empty);
		// check if there are no more bytes in the buffer
		if (thread_data->bytes_in_buffer == 0)
			break;
	}
	// Print the count of each printable character
	for (int i = 32; i <= 126; i++) {
		if (thread_data->count[i] > 0) {
			printf("%c: %d\n", i, thread_data->count[i]);
		}
	}
	pthread_exit(NULL);
}

void* threadC(void *arg) {
	// cast the argument to ThreadData
	ThreadData *thread_data = (ThreadData*) arg;

	// initialize counts for each word
	int and_count = 0;
	int at_count = 0;
	int it_count = 0;
	int my_count = 0;
	int hamlet_count = 0;

	while (1) {
		// wait for the buffer to be full
		sem_wait(&thread_data->full);
		// wait for access to the buffer
		sem_wait(&thread_data->mutex);

		// split the buffer into words and count each occurrence of the target words
		char *word;
		word = strtok((char*) thread_data->buffer, " \n\r\t");
		while (word != NULL) {
			if (strcmp(word, "and") == 0) {
				and_count++;
			} else if (strcmp(word, "at") == 0) {
				at_count++;
			} else if (strcmp(word, "it") == 0) {
				it_count++;
			} else if (strcmp(word, "my")) {
				my_count++;
			} else if (strcmp(word, "Hamlet") == 0) {
				hamlet_count++;
			}
			word = strtok(NULL, " \n\r\t");
		}

		// reset the buffer and release access to it
		thread_data->bytes_in_buffer = 0;
		sem_post(&thread_data->mutex);
		// signal that the buffer is empty
		sem_post(&thread_data->empty);

		// check if there are no more bytes in the buffer
		if (thread_data->bytes_in_buffer == 0)
			break;
	}
	// print the counts for each word
	printf("and: %d\n", and_count);
	printf("at: %d\n", at_count);
	printf("it: %d\n", it_count);
	printf("my: %d\n", my_count);
	printf("Hamlet: %d\n", hamlet_count);

	pthread_exit(NULL);
}

/*
 * This is the main function of the program that creates three threads and passes them a shared buffer
 * and several semaphores to synchronize access to the buffer. Each thread has a specific task: threadA
 * reads from a file and writes to the buffer, threadB counts the number of vowels in the buffer, and
 * threadC counts the number of digits.
 * The main function initializes the semaphores and the shared buffer, creates the threads,
 * waits for their completion, and finally destroys the semaphores.
 *
 * Notes:
 *
 * This function is the entry point to the program that creates three threads, each of which has a specific task.
 * The threads share a buffer and several semaphores, which are used to synchronize access to the buffer.
 * The first step of the function is to check that the program was executed with the correct number of arguments,
 * which should be the name of a file to read from.
 *
 * The function then creates a ThreadData struct, which holds information about the buffer, the filename, and the semaphores.
 * The filename is copied from the command line argument into the struct. Three semaphores are initialized.
 * The full semaphore is used to count the number of full slots in the buffer, the empty semaphore is used to count
 * the number of empty slots in the buffer, and the mutex semaphore is used to ensure mutual exclusion when accessing the buffer.
 *
 * Three threads are then created using the pthread_create() function.
 * The first argument to this function is a pointer to a pthread_t variable that was be used to identify the thread.
 * The second argument is a pointer to a function that the thread will execute, and the third argument is a pointer to the thread data struct.
 *
 * After all threads are created, the main function waits for them to complete using the pthread_join() function.
 * The program then destroys the semaphores using the sem_destroy() function and returns with a success status.
 */
int main(int argc, char *argv[]) {
	// Check that the program was executed with a filename argument
	if (argc != 2) {
		// Prints error message to stderr
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		// Exits program with failure status
		exit(EXIT_FAILURE);
	}

	// Create a struct to hold thread data
	ThreadData thread_data;
	thread_data.bytes_in_buffer = 0;
	// Copy the filename provided in the command line argument to the struct
	strncpy(thread_data.filename, argv[1], sizeof(thread_data.filename));

	// Initialize semaphores
	sem_init(&thread_data.full, 0, 0);
	sem_init(&thread_data.empty, 0, 1);
	sem_init(&thread_data.mutex, 0, 1);

	// Create three threads and pass the thread_data struct as an argument to each
	pthread_t threads[NUM_THREADS];

	if (pthread_create(&threads[0], NULL, threadA, &thread_data)) {
		// Prints an error message and exits with failure status if pthread_create fails
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	if (pthread_create(&threads[1], NULL, threadB, &thread_data)) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	if (pthread_create(&threads[2], NULL, threadC, &thread_data)) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	// Wait for all threads to complete
	for (int i = 0; i < NUM_THREADS; i++) {
		if (pthread_join(threads[i], NULL)) {
			perror("pthread_join");
			exit(EXIT_FAILURE);
		}
	}

	// Destroy semaphores
	sem_destroy(&thread_data.full);
	sem_destroy(&thread_data.empty);
	sem_destroy(&thread_data.mutex);

	// Exit program with success status
	return EXIT_SUCCESS;
}
