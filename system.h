/*
 * system.h
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "list.h"
#include "Idletred.h"
#include "trdfrst.h"


#define OLD_ENTRY 0x08
#define NEW_ENTRY 0x60

typedef void interrupt (*pInterrupt)(...);
void interrupt timer(...);
//void syncPrintf(char *text, ID id = 0);

extern void tick();

List *threads = new List();

volatile unsigned lockFlag = 1; // Unlocked
#define lock lockFlag = 0;
#define unlock { lockFlag = 1; if (System::context_on_demand) { dispatch(); }


class System {

public:

	static FirstThread *firstThread;
	static IdleThread *idleThread;
	static pInterrupt oldRoutine;

	static volatile int counter;
	static volatile int context_on_demand;

	static void inic();
	static void restore();
	
};

#endif /* SYSTEM_H_ */
