#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include<iostream>

using namespace std;

void *server_routine(void *arg);

void *client_routine(void *arg)
{
    uint16_t port = *((uint16_t *) arg);
    cout << "client_routine: request connection to port " << port << endl;
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

	cout << "Start demo. CNTR-C to exit." << endl;

	setupHandler();

	pthread_t server_tid;
	if (pthread_create(&server_tid, NULL, server_routine, (void *) &port))
	{
		cerr << "Failure creating server thread: " << strerror(errno) << endl;
		exit(1);
	}

	struct timespec ts = {0, 5000000};
	nanosleep(&ts, NULL);

	pthread_t client_tid;
    if (pthread_create(&client_tid, NULL, client_routine, (void *) &port))
    {
        cerr << "Failure creating client thread: " << strerror(errno) << endl;
        exit(1);
    }

	pthread_join(server_tid, NULL);

	cout << "Done." << endl;

	return 0;
}
