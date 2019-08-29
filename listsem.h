/*
 * listsem.h
 *
 *  Created on: Aug 27, 2019
 *      Author: OS1
 */

#ifndef LISTSEM_H_
#define LISTSEM_H_

class Semaphore;
class KernelSem;

class SemList {

public:

	class Node {
	public:
		KernelSem *sem;
		Node *next;
		Node(KernelSem *sem, Node *next = 0) {
			this->sem = sem;
			this->next = next;
		}
	};

	Node *head, *tail;
	int num_of_nodes;

	SemList();
	~SemList();

	void add(KernelSem* sem);
	void remove(KernelSem* sem);
	int isEmpty();
	KernelSem* getFirst();
	void onTick();

};

#endif /* LISTSEM_H_ */
