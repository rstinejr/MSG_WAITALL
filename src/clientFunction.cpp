/*
 * clientFunction.cpp
 *
 *  Created on: Jul 28, 2016
 *      Author: R.H. Stine, Jr.
 */
#include <inttypes.h>

#include <iostream>

using namespace std;

void *client_routine(void *arg)
{
    uint16_t port = *((uint16_t *) arg);
    cout << "client_routine: request connection to port " << port << endl;
    return NULL;
}



