/*
 * MyReader.h
 *
 *  Created on: May 9, 2010
 *      Author: abhi
 */

#ifndef MYREADER_H_
#define MYREADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <usb.h>        		// this is libusb

#include "../usb/opendevice.h" 	// common code moved to separate module
#include "../usb/usbconfig2.h"  // device's VID/PID and names
//there is a warning related to redefinition of USB_CFG_DEVICE_NAME and USB_CFG_DEVICE_NAME_LEN
//these MIGHT cause problems when working with both usb devices connected

#define OFFSET_VALUE 0

class MyReader {
	usb_dev_handle *handle;
public:
	MyReader();
	float Read();
	virtual ~MyReader();
};

#endif /* MYREADER_H_ */
