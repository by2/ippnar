/*
 * MyListener.h
 *
 *  Created on: May 4, 2010
 *      Author: abhi
 */

#ifndef MYLISTENER_H_
#define MYLISTENER_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include "MyRobot.h"
#include <math.h>

class MyListener {
private:
    struct sockaddr_rc loc_addr, rem_addr;
    int s, client, bytes_read;
    double inLatitude, inLongitude;
    char buf[128];
public:
	MyListener();
	int Listen(double* tLatitude, double* tLongitude, unsigned char* tRequest);
	virtual ~MyListener();
};

#endif /* MYLISTENER_H_ */
