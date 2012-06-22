/*************************************************************************************************************
 ***************************PATH PLANNING AND NAVIGATION FOR AUTONOMOUS ROBOTS********************************
 *************************************************************************************************************
 * THINGS TO DO:
 * 1. ADD THREADING TO CODE								DONE
 * 2. DO THE CONTROL MODULE								DONE
 * 3. ADD MUTEXES										DONE
 * 4. FINE TUNE IMAGE PROCESSING
 * 5. CREATE A SUITABLE ERROR HANDLING STRATEGY			DONE
 * 6. LOOK INTO CODE >> A LOT IS LEFT					DONE
 * 7. ADD MANUAL CONTROL MODULE							DONE
 ************************************************************************************************************
 ************************************************************************************************************
 */

#include "MyVision.h"
#include "MyListener.h"
#include "MyController.h"
#include "MyDriver.h"
#include "MyReader.h"
#include <cv.h>
#include <highgui.h>
#include <BlobResult.h>
#include <pthread.h>
#include <iostream>
#include <iomanip>
using namespace std;

#define AUTO		0
#define MANUAL		2

//GLOBAL DATA SPACE
MyLinkedList mBlobList;
int mMode = AUTO;
double mLatitude = 999;
double mLongitude = 999;
float mOrientation = 999;
unsigned char mRequest = 0;

//the threads
pthread_t vision_thread, listener_thread, reader_thread, controller_thread, driver_thread;

//the locks
pthread_mutex_t blob_list_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t gps_location_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t orientation_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t out_request_mutex = PTHREAD_MUTEX_INITIALIZER;

//functions to be started as threads
void *func_vision_thread(void *args);
void *func_listener_thread(void *args);
void *func_reader_thread(void *args);
void *func_controller_thread(void *args);
void *func_driver_thread(void *args);

//The MAIN function >> begins main_thread
int main(){

//	mOrientation = 170.2;
//	mLatitude = 13.329644;
//	mLongitude = 77.123285;
	pthread_create(&vision_thread, NULL, func_vision_thread, NULL);
	pthread_create(&listener_thread, NULL, func_listener_thread, NULL);
	pthread_create(&reader_thread, NULL, func_reader_thread, NULL);
	pthread_create(&controller_thread, NULL, func_controller_thread, NULL);
	pthread_create(&driver_thread, NULL, func_driver_thread, NULL);

	pthread_exit(0);
}

void *func_vision_thread(void *args){
	MyLinkedList *tBlobList;

	MyVision Vision(0);
	while(1){
	tBlobList = Vision.View();
		if(tBlobList != NULL){
			pthread_mutex_lock(&blob_list_mutex);
			mBlobList.EmptyList();
			mBlobList.CopyFromList(tBlobList);
			pthread_mutex_unlock(&blob_list_mutex);
//			sleep(1);
//			cout<<"Number of Blobs: "<<tBlobList->GetNumberOfBlobs()<<endl;
		}
	}
	pthread_exit(NULL);
}

void *func_listener_thread(void *args){
	double tLatitude, tLongitude;
	int tMode;
	unsigned char tRequest;

	MyListener Listener;
	while(1){
		tMode=Listener.Listen(&tLatitude, &tLongitude, &tRequest);
		pthread_mutex_lock(&gps_location_mutex);
		mMode = tMode;
		if(tMode == AUTO){
			mLatitude = tLatitude;
			mLongitude = tLongitude;
		} else if(tMode == MANUAL){		//update only in manual mode
			mRequest = tRequest;
		}
		pthread_mutex_unlock(&gps_location_mutex);
//		cout<<"Position >>\t"<<setprecision(10)<<"\tLat:\t"<<tLatitude<<"\tLong:\t"<<tLongitude<<"\tReq:\t"<<tRequest<<endl;
//		cout<<tMode;
	}
	pthread_exit(NULL);
}

void *func_reader_thread(void *args){
	float tOrientation;

	MyReader Reader;
	while(1){
		tOrientation = Reader.Read();
		pthread_mutex_lock(&orientation_mutex);
		mOrientation = tOrientation;
		pthread_mutex_unlock(&orientation_mutex);
//		cout<<"Orientation: "<<tOrientation<<endl;
	}
	pthread_exit(NULL);
}

void *func_controller_thread(void *args){
	MyLinkedList tBlobList;
	double tLatitude, tLongitude;
	float tOrientation;
	int tMode;
	unsigned char tRequest;

	MyController Controller;
	while(1){
		pthread_mutex_lock(&blob_list_mutex);
		tBlobList.EmptyList();
		tBlobList.CopyFromList(&mBlobList);
		pthread_mutex_unlock(&blob_list_mutex);
		pthread_mutex_lock(&gps_location_mutex);
		tMode = mMode;
		tLatitude = mLatitude;
		tLongitude = mLongitude;
		pthread_mutex_unlock(&gps_location_mutex);
		pthread_mutex_lock(&orientation_mutex);
		tOrientation = mOrientation;
		pthread_mutex_unlock(&orientation_mutex);
		if(tMode == AUTO){
			tRequest = Controller.Control(&tBlobList, tLatitude, tLongitude, tOrientation);
		}
		pthread_mutex_lock(&out_request_mutex);
		mRequest = tRequest;
		pthread_mutex_unlock(&out_request_mutex);
//		cout<<atoi((char *)&tRequest)<<endl;
//		sleep(1);
	}
	pthread_exit(NULL);
}

void *func_driver_thread(void *args){
	MyDriver Driver;
	unsigned char tRequest = 0;
	while(1){
		pthread_mutex_lock(&out_request_mutex);
		tRequest = mRequest;
		pthread_mutex_unlock(&out_request_mutex);
		Driver.Drive(tRequest);
	}
	pthread_exit(NULL);
}
