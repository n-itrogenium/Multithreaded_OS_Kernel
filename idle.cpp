#include "idle.h"
#include <stdio.h>

Idle::~Idle() { delete getMyPCB(); }

void Idle::run() {
	while (1);
}

void Idle::start() {
	getMyPCB()->state = READY;
}
