#ifndef _list_h_
#define _list_h_

class Thread;
class PCB;

class List {

public:

	class Node {
	public:
		PCB *pcb;
		Node *next;
		Node(PCB *pcb, Node *next = 0) {
			this->pcb = pcb;
			this->next = next;
		}
	};

	Node *head, *tail;
	int num_of_nodes;

	List();
	~List();

	void add(PCB* pcb);
	void remove(int ID);
	int isEmpty();
	PCB* getFirst();
	PCB* get(int ID);

};

#endif
