/*
 * serverFunction.cpp
 * Thread function for listening for a connection request.
 *
 *  Created on: Jul 28, 2016
 *      Author: R.H. Stine, Jr.
 */
#include <inttypes.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

using namespace std;

void *connection_routine(void *arg)
{
	int socketFd = *((int *) arg);
	sleep(1);
	cout << "connectin_routine: read socket " << socketFd << endl;
	close(socketFd);
	return NULL;
}

void *server_routine(void *arg)
{
    uint16_t port = *((uint16_t *) arg);
    cout << "server_routine: Listen on port " << port << endl;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        cerr << "server_routine, error creating socket: " << strerror(errno)
                << endl;
        exit(1);
    }

    struct sockaddr_in serv_addr = { };

    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port        = htons(port);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        cerr << "server_routine, error binding socket: " << strerror(errno)
                << endl;
        exit(1);
    }

    if (listen(sockfd, 5) != 0)
    {
        cerr << "server_routine, error in listen: " << strerror(errno) << endl;
        exit(1);
    }

    cout << "server_routine: wait for connection requests." << endl;

    for (;;)
    {
        struct sockaddr_in saddr;
        unsigned int saddr_ln = sizeof(saddr);

        int newsock = accept(sockfd, (struct sockaddr *) &saddr, &saddr_ln);
        if (newsock == -1)
        {
            cerr << "server_routine, error in accept: " << strerror(errno)
                    << endl;
            exit(1);
        }
        cout << "server_routine, new connection, socket " << newsock << endl;
    	pthread_t child_tid;
        if (pthread_create(&child_tid, NULL, connection_routine, (void *) &newsock))
    	{
    		cerr << "Failure creating connection thread: " << strerror(errno) << endl;
    	}
        else
        {
        	pthread_detach(child_tid);
        }
    }

    return NULL;
}


