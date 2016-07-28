#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include<iostream>

using namespace std;

void *server_routine(void *arg)
{
	uint16_t port = *((uint16_t *) arg);
	cout << "Listen on port " << port << endl;

	pause();

	return NULL;
}

void sig_handler(int s)
{
	cout << endl << "Caught signal " << s << ", so exit." << endl;
	exit(0);
}

void setupHandler()
{
    struct sigaction sigAction;

	sigAction.sa_handler = sig_handler;
	sigAction.sa_flags = 0;

	sigaction(SIGHUP,  &sigAction, NULL);
	sigaction(SIGINT,  &sigAction, NULL);
	sigaction(SIGQUIT, &sigAction, NULL);
	sigaction(SIGKILL, &sigAction, NULL);
	sigaction(SIGTERM, &sigAction, NULL);
}

int main(int argc, char *argv[])
{
	const uint16_t DEFAULT_PORT = 54321;

	uint16_t port = (argc > 1) ? atoi(argv[1]) : DEFAULT_PORT;

	cout << "Start demo. Signal to exit." << endl;

	setupHandler();

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
