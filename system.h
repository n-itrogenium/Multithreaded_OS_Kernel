/*
 * system.h
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "listpcb.h"
#include "listsem.h"
#include <stdio.h>
#include <stdarg.h>
#include <dos.h>

#define OLD_ENTRY 0x08
#define NEW_ENTRY 0x60

typedef void interrupt (*pInterrupt)(...);
void interrupt timer(...);

extern void tick();

#define lock System::lockFlag = 0;
#define unlock { System::lockFlag = 1; if (System::context_on_demand) dispatch(); }

extern int syncPrintf(const char *format, ...);


class FirstThread;
class Idle;

class System {

public:

	static volatile unsigned lockFlag; // Unlocked

	static List *threads, *blockedThreads;
	static SemList *semaphores;

	static FirstThread *firstThread;
	static Idle *idleThread;

	static pInterrupt oldRoutine;

	static volatile int counter;
	static volatile int context_on_demand;

	static void inic();
	static void restore();
	
};

#endif /* SYSTEM_H_ */
