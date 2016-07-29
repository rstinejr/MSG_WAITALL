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

    if (connRtn != 0)
    {
    	cerr << "clientRoutine: error in connect: "
    			<< strerror(errno) << endl;
    	return NULL;
    }

    cout << "clientRoutine: connection established." << endl;

    char msg1[] = "This is message 1";
    char msg2[] = "...and this is message 2";
    char msg3[] = "and this is the last message.";

    uint16_t ln;
    ln = ntohs(sizeof(msg1));
    write(sockFd, &ln, sizeof(ln));
    write(sockFd, msg1, sizeof(msg1));

    return NULL;
}



