#pragma once

using namespace std;

template<typename T>
struct node
{
	T* data;
	node* next;

	node(T* data) {
		this->data = data;
		next = nullptr;
	}

	~node() {
		delete data;
		next = nullptr;
	}
};

template<typename T>
class LinkedList {
public:
	node<T>* head;
	node<T>* tail;

	LinkedList() {
		head = nullptr;
		tail = nullptr;
	}

	void push(T* n) {
		node<T>* tmp = new node<T>(n);

		tmp->next = nullptr;

		if (head == nullptr) {
			head = tail = tmp;
		}

		else {
			tail->next = tmp;
			tail = tail->next;
		}
	}


	void remove(T* n) {
		node<T>* tmp = head;
		node<T>* prev = nullptr;

		while (tmp->next != nullptr && tmp->data != n) {
			prev = tmp;
			tmp = tmp->next;
		}

		if (tmp->data == n) {
			if (prev == nullptr) {
				head = tmp->next;
			}
			else {
				prev->next = tmp->next;
			}
			delete tmp;
		}

		else if (tmp->next == nullptr) {
			cout << "Error: value not found" << endl;
		}
	}

	~LinkedList() {
		while (head!=nullptr)
		{
			node<T>* tmp = head;
			head = head->next;
			delete tmp;
		}
	}

};


