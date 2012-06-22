/*
 * MyListener.cpp
 *
 *  Created on: May 4, 2010
 *      Author: abhi
 */

#include "MyListener.h"

MyListener::MyListener() {
	socklen_t opt = sizeof(rem_addr);
	memset(buf, 0, sizeof(buf));


	// allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // bind socket to port 1 of the first available local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    //try changing it to something else to fix the sender device, but it's ok
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 3;

    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    // put socket into listening mode
    listen(s, 1);

    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);

    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
}

int MyListener::Listen(double* tLatitude, double* tLongitude, unsigned char* tRequest){
	memset(buf, 0, sizeof(buf));
    // read data from the client
    bytes_read = read(client, buf, sizeof(buf));
    //following line is only fo debugs
    //printf("received [%s]\n", buf);

    //parse received data >> process and sort it
	double cLatitude, cLongitude;
	unsigned char cRequest;
	int cMode;
	if(buf[0] == '0'){
		cMode = 0;								//AUTONOMOUS MODE
		strtok(buf, "|");						//removes 1st field >> MODE
		cLatitude = atof(strtok(NULL, "|"));	//removes 2nd field >> LATITUDE
		cLongitude = atof(strtok(NULL, "|"));	//removes 3rd field >> LONGITUDE
		*tLatitude = cLatitude;
		*tLongitude = cLongitude;
		*tRequest = NULL;
	}
	else if(buf[0] == '2'){
	//	printf("sds");
		cMode = 2;								//MANUAL MODE
		strtok(buf, "|");						//removes 1st field >> MODE
		cRequest = atoi(strtok(NULL, "|"));		//takes 2nd field >> REQUEST_OUT
		*tLatitude = 999;
		*tLongitude = 999;
		*tRequest = cRequest;
//		printf("%d",cRequest);
//		fflush(stdout);
	} else{
		cMode = -1;								//ERROR
	}

	return(cMode);
}

MyListener::~MyListener() {
    // close connection
    close(client);
    close(s);
}
