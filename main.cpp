#include <errno.h>
#include <inttypes.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include<iostream>

using namespace std;

void *server_routine(void *arg)
{
	uint16_t port = *((uint16_t *) arg);
	cout << "Listen on port " << port << endl;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
    	cerr << "server_routine, error creating socket: " << strerror(errno) << endl;
    	exit(1);
    }

    struct sockaddr_in serv_addr= { };
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
    	cerr << "server_routine, error binding socket: " << strerror(errno) << endl;
    	exit(1);
    }

    if (listen(sockfd,5) != 0)
    {
    	cerr << "server_routine, error in listen: " << strerror(errno) << endl;
    	exit(1);
    }

    cout << "server_routine: wait for connection requests." << endl;

    for ( ; ; )
    {
    	struct sockaddr_in saddr;
    	unsigned int       saddr_ln = sizeof(saddr);

    	int newsock = accept(sockfd, (struct sockaddr *) &saddr, &saddr_ln);
    	if (newsock == -1)
    	{
    		cerr << "server_routine, error in accept: " << strerror(errno) << endl;
    		exit(1);
    	}
    }

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

	pthread_join(server_tid, NULL);

	cout << "Done." << endl;

	return 0;
}
