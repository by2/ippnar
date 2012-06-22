/*
 * MyVision.h
 *
 *  Created on: May 4, 2010
 *      Author: abhi
 */

#ifndef MYVISION_H_
#define MYVISION_H_

#include <cv.h>
#include <highgui.h>
#include <BlobResult.h>

#include "MyLinkedList.h"

//define constant parameters
#define OFFSET_FOR_THRESHOLD	50
#define GAUSS_ODD_NUM			7
#define ERODE_ITERATOR			1
#define DILATE_ITERATOR			2

#define BLOB_THRESHOLD 100

class MyVision {
private:
	CvCapture* capture;
	CBlobResult blobs;
	MyLinkedList GiveList;
	IplImage* iGetImage();
	IplImage* iBlurImage(IplImage* src);
	CvScalar iGetThreshold(IplImage* src);
	IplImage* iDoThreshold(IplImage* src, CvScalar colorThreshold);
	IplImage* iErodeAndDilate(IplImage* src);
	IplImage* iGetFinalImage(IplImage* src, IplImage* original);
	int iDoBlobDetection(IplImage* src, int threshold);
	int iShowImage4Test(IplImage* src);
	int ConvertAndMoveToList(int numberOfBlobs);
public:
	MyVision();
	MyVision(int intId);
	MyLinkedList* View();
	virtual ~MyVision();
};

#endif /* MYVISION_H_ */
