/*
 * MyRobot.h
 *
 *  Created on: May 5, 2010
 *      Author: abhi
 */

#ifndef MYROBOT_H_
#define MYROBOT_H_

#include <cstring>
#include <cstdlib>
#include <iostream>
using namespace std;

#define SUCCESS 1
#define FAILURE 0

#define VALID	1
#define INVALID	0

#define HIGH 		1
#define LOW 		0

#define AHEAD 			0
#define SLIGHT_LEFT		1
#define LEFT 			2
#define HARD_LEFT 		3
#define SLIGHT_RIGHT	4
#define RIGHT			5
#define HARD_RIGHT 		6
#define STOP			9

#define MOVE_AHEAD_FAST		0x0a
#define MOVE_AHEAD_SLOW		0x0a	//change to 0x0f
#define MOVE_SLIGHT_LEFT	0x08	//change to 0x0b
#define MOVE_LEFT			0x08
#define MOVE_HARD_LEFT		0x09
#define MOVE_SLIGHT_RIGHT	0x02	//change to 0x0e
#define MOVE_RIGHT			0x02
#define MOVE_HARD_RIGHT		0x06
#define MOVE_DO_NOT			0x00

class MyRobot {
public:
	double rLatitude,rLongitude;
	float rOrientation;
	int rValidPose;
	MyRobot();
	unsigned char MoveRobot(int tTheta, int tIntensity);
	int UpdateRobot(double tLatitude, double tLongitude, float tOrientation);
	int ValidateRobot();
	virtual ~MyRobot();
};

#endif /* MYROBOT_H_ */
