/*
 * MyRobot.cpp
 *
 *  Created on: May 5, 2010
 *      Author: abhi
 */

#include "MyRobot.h"

MyRobot::MyRobot() {
	//do some initialization and stuff
	rLatitude = 999;
	rLongitude = 999;
	rOrientation = 999;
	rValidPose = INVALID;
}

unsigned char MyRobot::MoveRobot(int tTheta, int tIntensity){
	//to generate the actual byte to be written to USB device
	unsigned char tRequest = 0;
	if(tTheta == AHEAD)			tRequest = MOVE_AHEAD_FAST;
	if(tTheta == SLIGHT_LEFT)	tRequest = MOVE_SLIGHT_LEFT;
	if(tTheta == LEFT ) 		tRequest = MOVE_LEFT;
	if(tTheta == HARD_LEFT)		tRequest = MOVE_HARD_LEFT;
	if(tTheta == SLIGHT_RIGHT)	tRequest = MOVE_SLIGHT_RIGHT;
	if(tTheta == RIGHT)			tRequest = MOVE_RIGHT;
	if(tTheta == HARD_RIGHT)	tRequest = MOVE_HARD_RIGHT;
	if(tTheta == STOP)			tRequest = MOVE_DO_NOT;
	int temp = tRequest;
	cout<<"\ttReq = "<<temp<<endl;
	return tRequest;
}

int MyRobot::UpdateRobot(double tLatitude, double tLongitude, float tOrientation){
	rLatitude = tLatitude;
	rLongitude = tLongitude;
	rOrientation = tOrientation;
	if(rLatitude != 999 && rLongitude != 999 && rOrientation != 999)
		rValidPose = VALID;
	return SUCCESS;
}
int MyRobot::ValidateRobot(){
	return(rValidPose);
}
MyRobot::~MyRobot() {
	//leave it empty
}
