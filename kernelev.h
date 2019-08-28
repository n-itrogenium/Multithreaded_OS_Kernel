/*
 * kernelev.h
 *
 *  Created on: Aug 28, 2019
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "Event.h"
#include "pcb.h"

class KernelEv {

private:

	int value;

public:

	PCB *myCreator, *blocked;
	Event* myEvent;
	IVTNo ivtNo;

	KernelEv(IVTNo ivtNo, Event* myEvent);
	virtual ~KernelEv();

	void wait();
	void signal();

};

#endif /* KERNELEV_H_ */
