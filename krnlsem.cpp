/*
 * krnlsem.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#include "krnlsem.h"
#include "pcb.h"

KernelSem::KernelSem(int init, Semaphore* mySem) {
	value = init;
	this->mySem = mySem;
	waiting = new List();
	//limitedTime = new List();
	//unlimited = new List();
}

KernelSem::~KernelSem() {
	PCB* temp = waiting->head->pcb;
	while(waiting->head) {
		syncPrintf("Nit %d oslobodjena brisanjem semafora...\n",temp->myThread->getId());
		temp->timeExceeded = 1;
		temp->semWaitingOn = 0;
		if (temp->waitTime) System::blockedThreads->remove(temp);
		temp->waitTime = 0;
		temp->state = READY;
		Scheduler::put(temp);
		waiting->head = waiting->head->next;
		temp = waiting->head->pcb;
	}
	delete waiting;
	System::semaphores->remove(this);
}

int KernelSem::val() const {
	return value;
}

void KernelSem::incVal() {
	value++;
}
/*
int KernelSem::wait(Time maxTimeToWait) {
	lock
	if (maxTimeToWait < 0) { unlock; return 1; }
	if (--value < 0) {
		PCB::running->state = BLOCKED;
		if (maxTimeToWait == 0)
			unlimited->add(PCB::running);
		else {
			PCB::running->waitTime = maxTimeToWait;
			limitedTime->add(PCB::running);
		}
		unlock
		dispatch();
	}
	return PCB::running->timeExceeded;
}
*/
/*
int KernelSem::wait(Time maxTimeToWait) {
	lock
	if (--value < 0) {
		PCB::running->state = BLOCKED;
		PCB::running->semWaitingOn = mySem;
		waiting->add(PCB::running);
		if (maxTimeToWait > 0) {
			printf("Nit %d zaustavljena na %d tikova.\n",PCB::running->myThread->getId(),maxTimeToWait);
			PCB::running->waitTime = maxTimeToWait;
			System::blockedThreads->add(PCB::running);
		} else printf("Nit %d zaustavljena neograniceno!!!.\n",PCB::running->myThread->getId());
	}
	//unlock
	dispatch();
	//lock
	int ret = PCB::running->timeExceeded;
	//printf("Nit %d timeExceeded = %d\n",PCB::running->myThread->getId(),ret);
	//PCB::running->timeExceeded = 1;
	unlock
	printf("Nit %d timeExceeded = %d\n",PCB::running->myThread->getId(),PCB::running->timeExceeded);
	return PCB::running->timeExceeded;
//	}
//	unlock
//	return 1;
}*/

int KernelSem::wait(Time maxTimeToWait) {
	lock
	printf("Nit %d - VALUE = %d\n",PCB::running->myThread->getId(),value);
	if (--value < 0) {

		PCB::running->state = BLOCKED;
		PCB::running->semWaitingOn = mySem;
		waiting->add((PCB*)PCB::running);
		PCB::running->timeExceeded = 1;

		if (maxTimeToWait > 0) {

			printf("Nit %d zaustavljena na %d tikova.\n",PCB::running->myThread->getId(),maxTimeToWait);
			PCB::running->waitTime = maxTimeToWait;
			System::blockedThreads->add((PCB*)PCB::running);

		} else

			printf("Nit %d zaustavljena neograniceno!!!.\n",PCB::running->myThread->getId());

		dispatch();

	}
	else printf("Nit %d je samo prosla kroz semafor.\n",PCB::running->myThread->getId());

	unlock

	printf("Nit %d timeExceeded = %d\n",PCB::running->myThread->getId(),PCB::running->timeExceeded);

	return PCB::running->timeExceeded;
}

/*
int KernelSem::signal(int n) {
	lock
	if (n<0) {
		unlock
		return n;
	}

	int num_of_threads;
	if (n == 0) num_of_threads = 1;
	else num_of_threads = n;

	PCB* temp;

	int ret = 0;
	for (int i = 0; i < num_of_threads; i++) {
		//temp = waiting->getFirst();
		//if (temp->waitTime) System::blockedThreads->remove(temp);
		if (!unlimited->isEmpty()) temp = unlimited->getFirst();
		else
			if(!limitedTime->isEmpty()) temp = limitedTime->getFirst();
		if (!temp) break;
		temp->waitTime = 0;
		temp->timeExceeded = 1;
		temp->semWaitingOn = 0;
		temp->waitTime = 0;
		temp->myThread->start();
		ret++;
	}
	value += num_of_threads;
	unlock
	return (n==0) ? 0 : ret;
}
*/

int KernelSem::signal(int n) {
	lock
	if (n<0) {
		unlock
		return n;
	}

	int num_of_threads;
	if (n == 0) num_of_threads = 1;
	else num_of_threads = n;

	PCB* temp;

	int ret = 0;
	for (int i = 0; i < num_of_threads; i++) {
		temp = waiting->getFirst();
		if (temp->waitTime) System::blockedThreads->remove(temp);
		if (!temp) break;
		printf("Nit %d je odblokirana SIGNALOM.\n",temp->myThread->getId());
		temp->waitTime = 0;
		temp->timeExceeded = 1;
		temp->semWaitingOn = 0;
		temp->waitTime = 0;
		temp->state = READY;
		Scheduler::put(temp);
		ret++;
	}
	value += num_of_threads;
	unlock
	return (n==0) ? 0 : ret;
}

