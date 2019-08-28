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

private:

	int value;
	static int staticID;
	int ID;
public:

	Semaphore* mySem;
	List* waiting;

	KernelSem(int init, Semaphore* mySem);
	virtual ~KernelSem();

	virtual int wait (Time maxTimeToWait);
	virtual int signal(int n = 0);

	int val () const;
	void incVal();

};

#endif /* KRNLSEM_H_ */
