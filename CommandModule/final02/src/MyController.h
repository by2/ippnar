/*
 * MyController.h
 *
 *  Created on: May 4, 2010
 *      Author: abhi
 */

#ifndef MYCONTROLLER_H_
#define MYCONTROLLER_H_

#include "MyLinkedList.h"
#include "MyRobot.h"

#include <fstream>
#include <cmath>

#define PI_VALUE 		3.14159265

//find a suitable radius of earth for this
#define RADIUS_OF_EARTH 6371004

//choose and insert values here
#define TRAPEZIUM_P01_X 1
#define TRAPEZIUM_P02_X 2
#define TRAPEZIUM_P12_Y 3

#define TRAPEZIUM_P03_X 4
#define TRAPEZIUM_P04_X 6
#define TRAPEZIUM_P34_Y 5

#define TRAPEZIUM_CEN_X 7
#define TRAPEZIUM_CEN_Y 8

#define ZONE_OUT 0
#define ZONE_IN1 1
#define ZONE_IN2 2
#define ZONE_IN3 3
#define ZONE_IN4 4

class MyController {
private:
	int cPushTheta, cPushIntensity;
	int cPullTheta, cPullIntensity;
	int cResultTheta, cResultIntensity;
	int cMap[2][2];
	MyRobot Robot;
	ifstream cWayPointListFileStream;
	int cWayPointId;
	double cWayPointLatitude, cWayPointLongitude;
	int iGetPull(float tDistance);
	int iGetPush(MyLinkedList *tBlobList);
	int iFindRegionId(int tX, int tY);
	int iFillMap(MyLinkedList *tBlobList);
	int iDoPlan();
	int iGetNextWayPoint();
	float iGetDistanceToNextWayPoint();
	float iGetAngleToNextWayPoint();
public:
	MyController();
	unsigned char Control(MyLinkedList *tBlobList, double tLatitude, double tLongitude, float tOrientation);
	virtual ~MyController();
};

#endif /* MYCONTROLLER_H_ */
