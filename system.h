/*
 * system.h
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <dos.h>
#include "list.h"
#include "frstthrd.h"
#include "idle.h"


#define OLD_ENTRY 0x08
#define NEW_ENTRY 0x60

typedef void interrupt (*pInterrupt)();
void interrupt timer();

extern void tick();

List *threads = new List();

FirstThread *firstThread = 0;
Idle *idleThread = 0;

volatile unsigned lockFlag = 1; // Unlocked
#define lock lockFlag = 0;
#define unlock lockFlag = 1;\	
if (context_on_demand) {\		
		dispatch();\				
}

volatile int counter = 0;
volatile int context_on_demand = 0;


class System {

public:

	static volatile int counter;
	static volatile int context_on_demand;

	static void inic();
	static void restore();
	
};

#endif /* SYSTEM_H_ */
