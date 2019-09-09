/*
 * krnlsem.h
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#ifndef KRNLSEM_H_
#define KRNLSEM_H_

#include "listpcb.h"
#include "semaphor.h"

const int VAL_BLOCKED = -1;

class KernelSem {

public:
	static int staticID;
	int ID;
	int value;
	Semaphore* mySem;
	List limitedTime, total;

	KernelSem(Semaphore* mySem, int init = 1);
	virtual ~KernelSem();

	virtual int wait (Time maxTimeToWait = 0);
	virtual int signal(int n = 0);

	int val () const;
	void incVal();

};

#endif /* KRNLSEM_H_ */
