/*
 * Main.cpp
 *
 *  Created on: Aug 27, 2019
 *      Author: OS1
 */

#include "system.h"
#include <stdio.h>

extern int userMain(int argc, char** argv);

int main(int argc, char** argv) {
	System::inic();
	userMain(argc, argv);
	System::restore();
	printf("Happy ending!\n");
	return 0;
}
