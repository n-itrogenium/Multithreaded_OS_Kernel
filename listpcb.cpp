#include "listpcb.h"
#include "pcb.h"
#include "krnlsem.h"

List::List() {
	head = tail = 0;
	num_of_nodes = 0;
}

List::~List() {
	Node *oldNode = 0;
	while (head) {
		oldNode = head;
		head = head->next;
		delete oldNode;
	}
	num_of_nodes = 0;
}


void List::add(PCB * pcb) {
	Node *newNode = new Node(pcb);
	if (num_of_nodes == 0)
		head = tail = newNode;
	else
		tail = tail->next = newNode;
	num_of_nodes++;
}


void List::remove(PCB* pcb) {
	Node *temp = head, *prev = 0, *oldNode;
	while (temp) {
		if (temp->pcb != pcb) {
			prev = temp;
			temp = temp->next;
		}
		else {
			oldNode = temp;
			temp = temp->next;
			if (!prev) head = temp;
			else prev->next = temp;
			delete oldNode;
			num_of_nodes--;
		}
	}
}


int List::isEmpty() {
	if (num_of_nodes == 0) return 1;
	else return 0;
}


PCB* List::getFirst() {
	if (!head) return 0;
	Node *temp = head;
	PCB* temp_pcb = temp->pcb;
	if (!head->next) head = tail = 0;
	else head = head->next;
	num_of_nodes--;
	delete temp;
	return temp_pcb;
}


PCB* List::get(int ID) {
	Node *temp = head;
	while (temp) {
		if (temp->pcb->myThread->getId() == ID)
			return temp->pcb;
		temp = temp->next;
	}
	return 0;
}








