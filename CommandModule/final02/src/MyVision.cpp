/*
 * MyVision.cpp
 * LEAK PROOF CODE
 *  Created on: May 4, 2010
 *      Author: abhi
 */

#include "MyVision.h"

MyVision::MyVision() {
	//add code to choose camera from interface 1, if not working then 0

}
MyVision::MyVision(int intId){
	//create a capture stream to capture from camera
	capture = cvCreateCameraCapture(intId);
    if(capture == NULL){
    	//write error handling code
	}
}

IplImage* MyVision::iGetImage(){
	//grab images into structures

	//remove the code below by turning them into comments
	//char imgFile[] = "/home/abhi/Desktop/_TestData/DSC04276.JPG";
	//IplImage* dest = cvLoadImage(imgFile, 1);

	IplImage* temp = cvQueryFrame(capture);
	if(temp==NULL)	cout<<"Locha";
	IplImage* dest = cvCloneImage(temp);
	return(dest);
}

IplImage* MyVision::iBlurImage(IplImage* src){
	IplImage* dest = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
	cvSmooth( src, dest, CV_GAUSSIAN, GAUSS_ODD_NUM, GAUSS_ODD_NUM);	//apply Gaussian blur filter
	return dest;
}

CvScalar MyVision::iGetThreshold(IplImage* src){
	//Code to calculate appropriate thresholding values

	//code to set bounding coordinates of ROI rectangle
	CvPoint point1 = cvPoint(140, 430);
	CvPoint point2 = cvPoint(500, 480);

	IplImage* temp = cvCloneImage(src);		//better safe than sorry
	cvSetImageROI(temp,cvRect(point1.x,point1.y,point2.x - point1.x,point2.y - point1.y));
	CvScalar colAvg = cvAvg(temp);
	cvResetImageROI(temp);						//not neccessary
	cvReleaseImage(&temp);

	//debugging code to display average color of ROI rectangle
	//cout<<"\t R="<<colAvg.val[0]<<" G="<<colAvg.val[1]<<" B="<<colAvg.val[2]<<endl;

	return(colAvg);

}

IplImage* MyVision::iDoThreshold(IplImage* src, CvScalar colorThreshold){
	//takes blurred image and threshold value to return thresholded binary black and white image

	//code to calculate lower and upper thresholding values
	CvScalar lower = cvScalar(
		 colorThreshold.val[0]-OFFSET_FOR_THRESHOLD,
		 colorThreshold.val[1]-OFFSET_FOR_THRESHOLD,
		 colorThreshold.val[2]-OFFSET_FOR_THRESHOLD
		 );
	CvScalar upper = cvScalar(
		 colorThreshold.val[0]+OFFSET_FOR_THRESHOLD,
		 colorThreshold.val[1]+OFFSET_FOR_THRESHOLD,
		 colorThreshold.val[2]+OFFSET_FOR_THRESHOLD
		 );
	//code to create needed data holders
	IplImage *dest = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);	//single channel
	//code to carry out thresholding
	cvInRangeS(src,lower,upper,dest);									//apply thresholding->obtain valid bits in dest
	return dest;
}

IplImage* MyVision::iErodeAndDilate(IplImage* src){

	IplImage *dest = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);	//single channel
	IplImage *temp = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);	//single channel

	//code to erode off small noise
	cvErode(src, temp, NULL, ERODE_ITERATOR);
	//code to dilate/grow significant images
	cvDilate(temp, dest, NULL, DILATE_ITERATOR);
	cvReleaseImage(&temp);
	return(dest);

}

IplImage* MyVision::iGetFinalImage(IplImage* filteredImageMap, IplImage* src){
	IplImage  *dest = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);	//3 channel
	IplImage  *temp = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);	//3 channel
	//code to obtain final thresholded image by anding boolean map image with source image
	cvCvtColor(filteredImageMap, temp, CV_GRAY2RGB);				//convert from single to 3 channel image
	cvAnd(src, temp, dest);										//remove unwanted pixels from original
	cvReleaseImage(&temp);
	return(dest);
}

int MyVision::iDoBlobDetection(IplImage* src, int threshold){

	IplImage* temp1 = cvCreateImage(cvSize(src->width, src->height),IPL_DEPTH_8U, 1);
    cvCvtColor(src, temp1, CV_BGR2GRAY);

    blobs = CBlobResult(temp1, NULL, threshold, true );
    blobs.Filter(blobs, B_INCLUDE, CBlobGetArea(), B_INSIDE, 30 , 10000);

    CBlob blob;
    CvPoint p1, p2;
    int numberOfBlobs = blobs.GetNumBlobs();
    //the following line is only for testing
    IplImage* temp2 = cvCloneImage(src);
    for(int i = 0; i < numberOfBlobs; i++){
    	blob = blobs.GetBlob(i);
        p1.x = (int)blob.MinX();
        p1.y = (int)blob.MinY();
        p2.x = (int)blob.MaxX();
        p2.y = (int)blob.MaxY();
        //the following line is only for testing
        cvRectangle(temp2, p1, p2, CV_RGB( 0, 255, 0 ), 2, 8, 0 );
    }
    //the following line is only for testing
    //cvSaveImage("/home/sandy/Desktop/asdf.jpg",temp2);
    //iShowImage4Test(shownLabeledImage);
    cvReleaseImage(&temp1);
    cvReleaseImage(&temp2);

    return(numberOfBlobs);
}

int MyVision::iShowImage4Test(IplImage* src){
	cvShowImage("TestOutputImage", src);
	return SUCCESS;
}

MyLinkedList* MyVision::View(){
	//don't open too many windows one suffices, 2 is enough, and 3 is too much

	IplImage* originalImage = iGetImage();
	CvScalar colorThreshold=iGetThreshold(originalImage);
	IplImage* blurredImage = iBlurImage(originalImage);
	IplImage* thresholdedImage = iDoThreshold(blurredImage,colorThreshold);
	IplImage* filteredImage = iErodeAndDilate(thresholdedImage);
	IplImage* preBlobFinalImage = iGetFinalImage(filteredImage, blurredImage);
	int numberOfBlobs = iDoBlobDetection(preBlobFinalImage, BLOB_THRESHOLD);

//	cvSaveImage("/home/abhi/Desktop/asdf.jpg",preBlobFinalImage);
//	essential evil during debugging to flush out all outputs to STDOUT
//	fflush(stdout);

	cvReleaseImage(&originalImage);
	cvReleaseImage(&blurredImage);
    cvReleaseImage(&thresholdedImage);
    cvReleaseImage(&filteredImage);
    cvReleaseImage(&preBlobFinalImage );

	ConvertAndMoveToList(numberOfBlobs);

	return(&GiveList);
}

int MyVision::ConvertAndMoveToList(int numberOfBlobs){
	CBlob blob;
	GiveList.EmptyList();

	double tX, tY, tArea;

	for(int i=0;i < numberOfBlobs;i++){
		blob = blobs.GetBlob(i);

		//get the 4 corner points of the blob
		//find their distances
		//find the center of the blob
		tX = (blob.MinX() + blob.MaxX()) / 2;
		tY = (blob.MinY() + blob.MaxY()) / 2;
		tArea = blob.Area();

		GiveList.InsertNewBlob(tX, tY, tArea);
	}
	return SUCCESS;
 }

MyVision::~MyVision() {
	//release interfaces for camera
	cvReleaseCapture(&capture);
}
