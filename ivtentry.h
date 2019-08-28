/*
 * ivtentry.h
 *
 *  Created on: Aug 28, 2019
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "kernelev.h"

typedef void interrupt (*pInterrupt)(...);
#define IVT_SIZE 256

#define PREPAREENTRY(ivtNo,old)\
void interrupt intr##ivtNo(...);\
IVTEntry entry##ivtNo(ivtNo,intr##ivtNo);\
void interrupt intr##ivtNo(...) {\
	if (old) entry##ivtNo.callOld();\
	entry##ivtNo.signal();\
}

class IVTEntry {

private:

	pInterrupt oldRoutine;
	IVTNo ivtNo;
	KernelEv* myEvent;

public:

	static IVTEntry* IVT[IVT_SIZE];

	IVTEntry(IVTNo ivtNo, pInterrupt newRoutine);
	~IVTEntry();
	void callOld();
	void signal();
	void setMyEvent(KernelEv *myEvent);
	KernelEv* getMyEvent();

};

#endif /* IVTENTRY_H_ */
