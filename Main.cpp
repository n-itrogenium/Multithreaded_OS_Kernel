/*
 * Main.cpp
 *
 *  Created on: Aug 27, 2019
 *      Author: OS1
 */

#include "system.h"


extern int userMain(int argc, char** argv);

int main(int argc, char** argv) {
	System::inic();
	userMain(argc, argv);
	System::restore();
	return 0;
}
