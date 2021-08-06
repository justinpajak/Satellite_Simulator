#ifndef STACK_H
#define STACK_H

#include "SLList.h"

template<class T>
class Stack{

private:

	SLList<T> list;			// Singly Linked List
	unsigned int max_len;	// Maximum size of the SLList
	unsigned int stackSize;	// Current number of stack elements

public:

	// Default Constructor
	Stack() : list(), max_len(2147483647), stackSize( 0 ) {}

	// Overloaded Constructor - Limit Stack Size
	Stack(unsigned int max) : list(), max_len(max), stackSize( 0 ) {}

	unsigned int max_size() const{

		return max_len;
	}

	unsigned int size() const{

		return stackSize;
	}

	bool empty() const{

		return stackSize == 0;

	}

	void push(T elem){

		if(stackSize < max_len){

			list.push_front( elem );
			stackSize++;

		}
	}

	void pop(){

		if(stackSize > 0){

			list.pop_front();
			stackSize--;

		}
	}

	T top() const{
		return list.front();
	}

};

#endif
