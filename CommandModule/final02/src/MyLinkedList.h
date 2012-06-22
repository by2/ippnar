/*
 * MyLinkedList.h
 *
 *  Created on: May 5, 2010
 *      Author: abhi
 */

#ifndef MYLINKEDLIST_H_
#define MYLINKEDLIST_H_

#include<iostream>
using namespace std;

#define SUCCESS 1
#define	FAILURE	0

struct MyBlob{
	double  pX, pY, pArea;
	struct MyBlob *next;
	MyBlob();
	MyBlob(double tX, double tY, double tArea);
};

class MyLinkedList {
	int numberOfBlobs;
	MyBlob *head;
public:
	MyLinkedList();
	int GetNumberOfBlobs();
	int InsertNewBlob(double tX, double tY, double tArea);
	MyBlob* GetNthBlob(int indexN);										//from 1 to NumMax
	int CopyFromList(MyLinkedList* tList);
	int EmptyList();
	virtual ~MyLinkedList();
};

#endif /* MYLINKEDLIST_H_ */
