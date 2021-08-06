#ifndef SLLIST_H
#define SLLIST_H

#include <cstdlib>
#include <iostream>

template<class T>
class SLList{

private:

	/* NODE contains the data and a pointer to the next node */
	struct node
	{
		T     data;
		node* next;

		/********************************************
			* Function Name  : node
			* Pre-conditions : none
			* Post-conditions: none
			*
			* Node Struct Default Constructor
			********************************************/
		node() : data(), next(NULL) { }


		/********************************************
			* Function Name  : node
			* Pre-conditions : none
			* Post-conditions: none
			*
			* Node Struct Overloaded Constructor with data input
			********************************************/
		node(T dataIn) : data(dataIn), next(NULL) { }


		/********************************************
			* Function Name  : ~node
			* Pre-conditions : none
			* Post-conditions: none
			*
			* Delete operator required for PQC since it contains a
			* private member which is a pointer
			********************************************/
		~node(){

			delete next;

		}


		/********************************************
			* Function Name  : node
			* Pre-conditions : const node& copy
			* Post-conditions: none
			*
			* Rule of 3: Copy Constructor
			* Required for -weffc++ flag
			********************************************/
		node(const node& copy){
			data = copy.data;
			next = copy.next;
		}


		/********************************************
			* Function Name  : operator=
			* Pre-conditions : const node& assign
			* Post-conditions: node&
			*
			* Rule of 3: Assignment Operator
			* Required for -weffc++ flag
			********************************************/
		node& operator=(const node& assign){

			if(this != &assign){
				this->data = assign.data;
				this->next = assign.next;
			}
			return *this;
		}


		/********************************************
			* Function Name  : operator=
			* Pre-conditions : const node* assign
			* Post-conditions: node*
			*
			* Assignment Operator for Pointer
			* Required for pointer assignment in SLList
			********************************************/
		node* operator=(const node* assign){

			if(this != (void *)&assign){
				this->data = assign->data;
				this->next = assign->next;
			}
			return *this;
		}


		/********************************************
			* Function Name  : operator!=
			* Pre-conditions : const node* rhs
			* Post-conditions: bool
			*
			* != Operator for Pointer
			* Required for pointer comparison in SLList
			********************************************/
		bool operator!=(const node* rhs){

			return this != (void *)&rhs;

		}

	};

	node* head;	// Head pointer for Singly-Linked List

public:

	/********************************************
		* Function Name  : SLList
		* Pre-conditions : none
		* Post-conditions: none
		*
		* SLList Default Constructor
		********************************************/
	SLList() : head(NULL) {}


	/********************************************
		* Function Name  : ~SLList
		* Pre-conditions : none
		* Post-conditions: none
		*
		* Singly Linked List Destructor
		********************************************/
	~SLList(){

		/*node* prev = NULL;
			node* curr = head;

			while(curr != NULL){

				prev = curr;
				curr = curr->next;

				delete prev;
			}*/
		delete head;

	}


	/********************************************
		* Function Name  : SLList
		* Pre-conditions : const SLList<T>& copy
		* Post-conditions: none
		*
		* Copy Constructor for Singly Linked List
		********************************************/
	SLList(const SLList<T>& copy) : head(NULL) {

		node* curr = copy.head;

		while(curr != NULL){

			Insert(curr->data);

			curr = curr->next;

		}

	}

	/********************************************
		* Function Name  : operator=
		* Pre-conditions : const SLList<T>& assign
		* Post-conditions: SLList<T>&
		*
		* Assignment Operator for Singly Linked List
		********************************************/
	SLList<T>& operator=(const SLList<T>& assign){

		if(this != &assign){

			node* prev = NULL;
			node* curr = assign.head;

			while(curr != NULL){

				this->Insert(curr->data);

				prev = curr;
				curr = curr->next;

			}
		}
		return *this;

	}

	/********************************************
		* Function Name  : Insert
		* Pre-conditions : int value
		* Post-conditions: void
		*
		* Creates a Node with value, and inserts at
		* the end of the Singly Linked List
		********************************************/
	void Insert (T value)
	{
		node* temp = new node(value);

		if ( IsEmpty() )
		{
			head = temp;
		}
		else
		{
			node* prev =  NULL;
			node* curr = head;

			/* traverse the list until the end */
			while (curr != NULL)
			{
				prev = curr;
				curr = curr->next;
			}

			/* insert the node, temp, at the end */
			prev -> next = temp;
		}
	}


	/********************************************
		* Function Name  : push_front
		* Pre-conditions : T value
		* Post-conditions: none
		*
		* Puts the element at the front
		* For O(1) insert time
		********************************************/
	void push_front(T value){

		// Create new node
		node* temp = new node(value);

		// Set temp->next = head
		temp -> next = head;

		// Set head equal to temp
		head = temp;

	}


	/********************************************
		* Function Name  : Delete
		* Pre-conditions : int data
		* Post-conditions: int
		*
		* Delets the first instance of data. Returns -1
		* if not found
		********************************************/
	bool Delete (T target)
	{
		node* temp = new node(); node* prev = new node(); node* curr = new node();

		if (IsEmpty ())
		{
			std::cout << "Can't delete from an empty list" << std::endl;
			return (-1);
		}

		/* if the target value is the first in the list, move head */
		else if (target == head -> data)
		{
			temp = head;
			head = head -> next;
			free (temp);
			return true;
		}

		/* traverse the list until the target value is found */
		else
		{
			prev = head;
			curr = head -> next;

			while (curr != NULL && curr -> data != target)
			{
				prev = curr;
				curr = curr -> next;
			}

			if(curr != NULL)
			{
				/* delete the node the contains the target value */
				temp = curr;
				prev -> next = curr -> next;
				free(temp);
				return true;
			}
			else
			{
				std::cout << target << " was not in the list" << std::endl;
				return false;
			}
		}
	}

	bool pop_front(){

		if (IsEmpty ())
		{
			std::cout << "Can't delete from an empty list" << std::endl;
			return (-1);
		}

		/* if the target value is the first in the list, move head */
		else
		{
			node* temp = head;
			head = head -> next;
			free (temp);
			return true;
		}

	}

	T front() const{

		return head->data;

	}


	/********************************************
		* Function Name  : IsEmpty
		* Pre-conditions : node* head
		* Post-conditions: int
		*
		* Bool returns if the list contains values
		********************************************/
	bool IsEmpty () const{

		return head == NULL;

	}


	/********************************************
		* Function Name  : contains
		* Pre-conditions : const T& searchVal
		* Post-conditions: bool
		*
		* Returns whether the list contains the element
		********************************************/
	bool contains(const T& searchVal) const{

		if (IsEmpty()) {

			return false;

		}
		else{

			node* prev =  NULL;
			node* curr = head;

			while (curr != NULL && curr -> data != searchVal){
				prev = curr;
				curr = curr -> next;
			}

			return curr != NULL;
		}
	}


	/********************************************
		* Function Name  : operator<<
		* Pre-conditions : std::ostream& output, const SLList<T>& theList
		* Post-conditions: std::ostream&
		*
		* Overloaded friend ostream operator for SLList
		********************************************/
	friend std::ostream& operator<<( std::ostream& output, const SLList<T>& theList ){

		node* curr;

		if (theList.IsEmpty())
		{
			output << "The list is empty" << std::endl;;
		}
		else
		{
			/* set the current pointer to the first
			** node of the list */
			curr = theList.head;

			/* Until the end of the list */
			while (curr != NULL)
			{
				/* print the current data item */
				output << curr->data << " ";

				/* move to the next node */
				curr = curr -> next;
			}
		}

		return output;
	}

};

#endif
