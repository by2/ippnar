/*
 * MyLinkedList.cpp
 *
 *  Created on: May 5, 2010
 *      Author: abhi
 */

#include "MyLinkedList.h"

MyBlob::MyBlob(){
	//default constructor
	next = NULL;
}

MyBlob::MyBlob(double tX, double tY, double tArea){
	pX = tX;
	pY = tY;
	pArea = tArea;
	next = NULL;
}

MyLinkedList::MyLinkedList(){
	//default constructor to initialize values
	numberOfBlobs = 0;			//initialize to zero >> for empty list
	head = NULL;				//default head to NULL >> for empty list
	}

int MyLinkedList::GetNumberOfBlobs(){
	return numberOfBlobs;
}

int MyLinkedList::InsertNewBlob(double tX, double tY, double tArea){
	//function to insert new blob at the head of the list
	MyBlob *tNewBlob = new MyBlob(tX, tY, tArea);
	tNewBlob -> next = head;
	head = tNewBlob;
	numberOfBlobs++;			//increment number of blobs in list by 1
	return SUCCESS;				//return 1 for successful insertion
}

struct MyBlob* MyLinkedList::GetNthBlob(int indexN){
	//function to get the Nth blob
	int i;
	struct MyBlob *temp = head;
	if(indexN > numberOfBlobs || temp == NULL){
		//add suitable error handling code >> write to a global error stream, but it's ok
		cout<<"n exceeds the numberOfBlobs";
		return NULL;	//in case of error
	}
	for(i = 2;i <= indexN;i++){
		temp=temp->next;
	}
	return temp;		//return pointer to Nth blob
}

int MyLinkedList::CopyFromList(MyLinkedList* tList){
	int i = 0;
	MyBlob* tBlob;
	int tCountSourceList = tList->GetNumberOfBlobs();
	for(i = 1; i <= tCountSourceList; i++){
		tBlob = tList->GetNthBlob(i);
		InsertNewBlob(tBlob->pX, tBlob->pY, tBlob->pArea);
	}
	return SUCCESS;
}

int MyLinkedList::EmptyList(){
	struct MyBlob* temp1;
	struct MyBlob* temp2;
	if(head != NULL){
		temp1 = head;
		temp2 = temp1->next;
		while(temp2 != NULL){
			delete temp1;
			temp1 = temp2;
			temp2 = temp1 -> next;
		}
		delete temp1;
	}
	head = NULL;
	numberOfBlobs = 0;
	return SUCCESS;
}

MyLinkedList::~MyLinkedList() {
	//empty the list by using delete function
	EmptyList();
}
