/*
 * MyController.cpp
 *
 *  Created on: May 4, 2010
 *      Author: abhi
 */

#include "MyController.h"
#include <iomanip>

MyController::MyController() {
	cWayPointListFileStream.open("/home/sandy/Desktop/WayList.ppl");
	iGetNextWayPoint();
}

int MyController::iGetNextWayPoint(){
	char tBuffer[50];
	if(!cWayPointListFileStream.eof()){
		cWayPointListFileStream.getline(tBuffer, 50, '|');
		cWayPointId = atoi(tBuffer);
		cWayPointListFileStream.getline(tBuffer, 50, '|');
		cWayPointLatitude = (double )atof(tBuffer);
		cWayPointListFileStream.getline(tBuffer, 50, '\n');
		cWayPointLongitude = (double )atof(tBuffer);
		cout<<"Current WayPoint:\t"<<setprecision(10)<<cWayPointId<<"\t"<<cWayPointLatitude<<"\t"<<cWayPointLongitude<<"\n";
		return SUCCESS;
	} else{
		//target reached
		return FAILURE;
	}
}

float MyController::iGetDistanceToNextWayPoint(){
	//convert all latitudes and longitudes from radians to degrees first
	float tLatitude1 = (Robot.rLatitude * PI_VALUE) / 180;
	float tLongitude1 = (Robot.rLongitude * PI_VALUE) / 180;
	float tLatitude2 = (cWayPointLatitude * PI_VALUE) / 180;
	float tLongitude2 = (cWayPointLongitude * PI_VALUE) / 180;
	//apply haversine formula
	double tDeltaLatitude = tLatitude2 - tLatitude1;
	double tDeltaLongitude = tLongitude2 - tLongitude1;
	double tA = pow(sin(tDeltaLatitude/2),2) + cos(tLatitude1) * cos(tLatitude2) * pow(sin(tDeltaLongitude/2),2);
	double tC = 2 * atan2(sqrt(tA), sqrt(1-tA));
	double tDistance = RADIUS_OF_EARTH * tC;
	return(tDistance);
}

float MyController::iGetAngleToNextWayPoint(){
	//convert all latitudes and longitudes from radians to degrees first
	float tLatitude1 = (Robot.rLatitude * PI_VALUE) / 180;
	float tLongitude1 = (Robot.rLongitude * PI_VALUE) / 180;
	float tLatitude2 = (cWayPointLatitude * PI_VALUE) / 180;
	float tLongitude2 = (cWayPointLongitude * PI_VALUE) / 180;
	//determine the angle in radians
	float tBearingR = atan2(sin(tLongitude2-tLongitude1)*cos(tLatitude2),(cos(tLatitude1)*sin(tLatitude2))-(sin(tLatitude1)*cos(tLatitude2)*cos(tLongitude2-tLongitude1)));
	//convert radians obtained to degrees
	float tBearingD = (tBearingR * 180) / PI_VALUE;
	//cout<<"Bearing:\t"<<tBearingD<<endl;
	//convert angle found (-180,+180] to positive angle [0,+360)
	tBearingD = (tBearingD < 0)?(360 + tBearingD):tBearingD;
	float tAngleOfRelativeBearing = tBearingD - Robot.rOrientation;
	tAngleOfRelativeBearing = (tAngleOfRelativeBearing < 0)?(360 + tAngleOfRelativeBearing):tAngleOfRelativeBearing;
	//returned angle is in degrees from [0,+360)
	return(tAngleOfRelativeBearing);
}

unsigned char MyController::Control(MyLinkedList *tBlobList, double tLatitude, double tLongitude, float tOrientation){
	int tTargetReachedFlag = 1;
	unsigned char tResultRequest = 0;
	float tDistance;
	Robot.UpdateRobot(tLatitude, tLongitude, tOrientation);
	if(Robot.ValidateRobot() == VALID){
		cout<<"Robot Updated:\tValid Pose\n";
		//move only when the pose is valid
		tDistance = iGetDistanceToNextWayPoint();
		cout<<"Distance from WayPoint:\t"<<tDistance<<endl;
		if(tDistance < 2){
			cout<<"Clearing WayPoint "<<cWayPointId<<endl;
			tTargetReachedFlag = iGetNextWayPoint();
		}
		if(tTargetReachedFlag == 0){
			//target reached << no more way points
			//show congratulation message
			cout<<"Destination Reached";
			exit(0);
		}
		iGetPull(tDistance);
		cout<<"Pull >>\tAngle:\t"<<cPullTheta<<"\tIntensity:\t"<<cPullIntensity<<endl;
		iGetPush(tBlobList);
		cout<<"Push >>\tAngle:\t"<<cPushTheta<<"\tIntensity:\t"<<cPushIntensity<<endl;
		iDoPlan();
		cout<<"Result >>\tAngle:\t"<<cResultTheta<<"\tIntensity:\t"<<cResultIntensity<<endl;
		tResultRequest = Robot.MoveRobot(cResultTheta, cResultIntensity);
		cout<<"Current WayPoint:\t"<<setprecision(10)<<cWayPointId<<"\t"<<cWayPointLatitude<<"\t"<<cWayPointLongitude<<"\n";
	}
	return(tResultRequest);
}

int MyController::iGetPull(float tDistance){
	//uses way point information and finds pull
	//produces cPullIntensity and cPullTheta
	if(tDistance < 5)
		cPullIntensity = LOW;
	else
		cPullIntensity = HIGH;
	float tAngle = iGetAngleToNextWayPoint();
	cout<<"Angle of Relative Bearing:\t"<<tAngle<<endl;
	if(tAngle < 30)
		cPullTheta = AHEAD;
	else if(tAngle < 60)
		cPullTheta = SLIGHT_RIGHT;
	else if(tAngle < 120)
		cPullTheta = RIGHT;
	else if(tAngle < 180)
		cPullTheta = HARD_RIGHT;
	else if(tAngle < 240)
		cPullTheta = HARD_LEFT;
	else if(tAngle < 300)
		cPullTheta = LEFT;
	else if(tAngle < 330)
		cPullTheta = SLIGHT_LEFT;
	else
		cPullTheta = AHEAD;
	return SUCCESS;
}

int MyController::iGetPush(MyLinkedList *tBlobList){
	//finds the relative push from the points closest to the preferred path
	//works only on objects within a certain range
	//produces cPushIntensity and cPushTheta
	iFillMap(tBlobList);
	cPushIntensity = HIGH;
	cPushTheta = AHEAD;
	if(cMap[0][0] == 1){
		cPushTheta = LEFT;
		cPushIntensity = LOW;
	}
	else if(cMap[0][1]){
		cPushTheta = RIGHT;
		cPushIntensity = LOW;
	}
	else if(cMap[1][0]){
		cPushTheta = SLIGHT_LEFT;
		cPushIntensity = HIGH;
	}
	else if(cMap[0][1]){
		cPushTheta = SLIGHT_RIGHT;
		cPushIntensity = HIGH;
	}
	return SUCCESS;
}

int MyController::iFindRegionId(int tX, int tY){
	float tDistanceOfOriginFromLeftLine, tDistanceOfOriginFromRightLine, tDistanceOfOriginFromPointP;
	if(tY < TRAPEZIUM_P12_Y || tY > TRAPEZIUM_P34_Y)
		return(ZONE_OUT);
	tDistanceOfOriginFromLeftLine = pow((TRAPEZIUM_P12_Y*(TRAPEZIUM_P04_X-TRAPEZIUM_P01_X)-TRAPEZIUM_P01_X*(TRAPEZIUM_P34_Y-TRAPEZIUM_P12_Y)),2)/(pow(TRAPEZIUM_P34_Y-TRAPEZIUM_P12_Y, 2)+pow(TRAPEZIUM_P04_X-TRAPEZIUM_P01_X,2));
	tDistanceOfOriginFromRightLine = pow((TRAPEZIUM_P12_Y*(TRAPEZIUM_P03_X-TRAPEZIUM_P02_X)-TRAPEZIUM_P02_X*(TRAPEZIUM_P34_Y-TRAPEZIUM_P12_Y)),2)/(pow(TRAPEZIUM_P34_Y-TRAPEZIUM_P12_Y, 2)+pow(TRAPEZIUM_P04_X-TRAPEZIUM_P01_X,2));
	tDistanceOfOriginFromPointP = tX*tX + tY*tY;
	if(tDistanceOfOriginFromPointP < tDistanceOfOriginFromLeftLine || tDistanceOfOriginFromPointP > tDistanceOfOriginFromRightLine)
		return(ZONE_OUT);
	//if the point is still inside the function, then it is inside the trapezium; now look for which zone
	if(tX < TRAPEZIUM_CEN_Y){
		if(tY < TRAPEZIUM_CEN_X)
			return(ZONE_IN1);
		else
			return(ZONE_IN2);
	}else {
		if(tY < TRAPEZIUM_CEN_X)
			return(ZONE_IN3);
		else
			return(ZONE_IN4);
	}
}

int MyController::iFillMap(MyLinkedList *tBlobList){
	MyBlob* tBlob;
	int tZone;
	int tNumberOfBlobs = tBlobList->GetNumberOfBlobs();
	cMap[0][0] = cMap[0][1] = cMap[1][0] = cMap[1][1] = 0;
	cout<<"HELLO:"<<tNumberOfBlobs<<endl;
	fflush(stdout);
	for(int i=1;i<=tNumberOfBlobs;i++){
		tBlob = tBlobList->GetNthBlob(i);
		tZone = iFindRegionId(tBlob->pX, tBlob->pY);
		if(tZone == ZONE_IN1)
			cMap[0][0] = 1;
		if(tZone == ZONE_IN2)
			cMap[0][1] = 1;
		if(tZone == ZONE_IN3)
			cMap[0][0] = 1;
		if(tZone == ZONE_IN4)
			cMap[0][0] = 1;
	}
	return SUCCESS;
}

int MyController::iDoPlan(){
	//give the structure a bit more thought, but it's ok
	//operates on data from push and pull to generate a rough sense of direction of an obstacle free path
	//default case: push is absent
	if(cPushTheta != AHEAD){
		//for obstacles
		cResultTheta = cPushTheta;
		cResultIntensity = cPushIntensity;
	} else {
		//for no obstacles
		cResultTheta = cPullTheta;
		cResultIntensity = cPullIntensity;
	}
	return SUCCESS;
}

MyController::~MyController() {
	cWayPointListFileStream.close();
}
