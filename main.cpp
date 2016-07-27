extern "C"
{
#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
}

#include<iostream>

using namespace std;

void *server_routine(void *arg)
{
	uint16_t port = *((uint16_t *) arg);
	cout << "Listen on port " << port << endl;

	return NULL;
}

int main(int argc, char *argv[])
{
	const uint16_t DEFAULT_PORT = 54321;

	uint16_t port = (argc > 1) ? atoi(argv[1]) : DEFAULT_PORT;

	cout << "Start demo. Signal to exit." << endl;

	pthread_t server_tid;

	if (pthread_create(&server_tid, NULL, server_routine, (void *) &port))
	{
		cerr << "Failure creating server thread: " << strerror(errno) << endl;
		exit(1);
	}

	pthread_join(server_tid, NULL);

	cout << "Done." << endl;

	return 0;
}
