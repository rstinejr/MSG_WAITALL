/*
 * clientFunction.cpp
 *
 *  Created on: Jul 28, 2016
 *      Author: R.H. Stine, Jr.
 */
#include <arpa/inet.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

using namespace std;

void *client_routine(void *arg)
{
	void *threadRtn;

    uint16_t port = *((uint16_t *) arg);
    cout << "client_routine: request connection to port " << port << endl;
    int sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0)
    {
    	fprintf(stderr,"connectToTcp: error in socket(), %s\n", strerror(errno));
        return (void *) -1;
    }

    struct sockaddr_in sockAdr;

    memset(&sockAdr, 0, sizeof(struct sockaddr_in));
    sockAdr.sin_port        = htons(port);
    sockAdr.sin_family      = AF_INET;
    sockAdr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int connRtn = connect(sockFd, reinterpret_cast<struct sockaddr *>(&sockAdr),
    		              sizeof(sockAdr));

    if (connRtn == 0)
    {
    	struct timespec ts = {0, 500000000};  // sleep 500 msec.
    	nanosleep(&ts, NULL);

    	unsigned char dummyUDP[12] = { 0, };
    	*((uint16_t *) dummyUDP) = (uint16_t) (random() & 0x0ffff);

    	int writeCnt = write(sockFd, dummyUDP, sizeof(dummyUDP));
    	if (writeCnt == -1)
    	{
    		fprintf(stderr,"Error writing to socket: %s\n", strerror(errno));
    	}

    	close(sockFd);

    	threadRtn = (void *)((writeCnt == sizeof(dummyUDP)) ? 0 : -1);
    }
    else
    {
    	threadRtn = (void *) connRtn;
    	fprintf(stderr,"connectToTcp: error in connect(), %s\n", strerror(errno));
    }

    return threadRtn;
}



