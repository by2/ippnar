/*
 * MyDriver.h
 *
 *  Created on: May 5, 2010
 *      Author: abhi
 */

#ifndef MYDRIVER_H_
#define MYDRIVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <usb.h>

#include "../usb/opendevice.h" // common code moved to separate module
#include "../usb/usbconfig.h"  // device's VID/PID and names

#define SUCCESS 1
#define FAILURE 0

class MyDriver {
	usb_dev_handle *handle;
public:
	MyDriver();
	int Drive(unsigned char tRequest);
	virtual ~MyDriver();
};

#endif /* MYDRIVER_H_ */
