#include <iostream>
#include <string>
using namespace std;


#ifndef LINKED_QUEUE_
#define LINKED_QUEUE_

#include "Node.h"
#include "QueueADT.h"


template <typename T>
class LinkedQueue:public QueueADT<T>
{
private:
	Node<T>* backPtr;
	Node<T>* frontPtr;
public :
	LinkedQueue();	
	bool isEmpty() const ;
	bool enqueue(const T& newEntry);
	bool dequeue(T& frntEntry);  
	bool peek(T& frntEntry)  const;	
	bool top(T* &frntPointer)  const;
	

	virtual ~LinkedQueue();

	//copy constructor
	LinkedQueue(const LinkedQueue<T> & LQ);
	
	friend void print(LinkedQueue<T> & zis)
	{
		if (zis.isEmpty())
		{
			cout << endl;
			return;
		}
		LinkedQueue <T> temp;
		T x;
		zis.dequeue(x);
		temp.enqueue(x);
		cout << x;
		while (!zis.isEmpty())
		{
			T x;
			zis.dequeue(x);
			temp.enqueue(x);
			cout << ", " << x;

		}
		while (!temp.isEmpty())
		{
			T x;
			temp.dequeue(x);
			zis.enqueue(x);
		}
		cout << endl;

	}
};
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: Queue()
The constructor of the Queue class.

*/

template <typename T>
LinkedQueue<T>::LinkedQueue(){
	backPtr=nullptr;
	frontPtr=nullptr;
	QueueADT<T>::setCount(0);
}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: isEmpty
Sees whether this queue is empty.

Input: None.
Output: True if the queue is empty; otherwise false.
*/
template <typename T>
bool LinkedQueue<T>::isEmpty() const
{
	return (frontPtr==nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

/*Function:enqueue
Adds newEntry at the back of this queue.

Input: newEntry .
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool LinkedQueue<T>::enqueue( const T& newEntry)
{
	QueueADT<T>::setCount(QueueADT<T>::getCount()+1);
	Node<T>* newNodePtr = new Node<T>(newEntry);
	// Insert the new node
	if (isEmpty())	//special case if this is the first node to insert
		frontPtr = newNodePtr; // The queue is empty
	else
		backPtr->setNext(newNodePtr); // The queue was not empty

	backPtr = newNodePtr; // New node is the last node now
	return true ;
} // end enqueue


/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Function: dequeue
Removes the front of this queue. That is, removes the item that was added
earliest.

Input: NonesetCount(getCount()+1);
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool LinkedQueue<T>:: dequeue(T& frntEntry)  
{   
	if(isEmpty())
		return false;
	QueueADT<T>::setCount(QueueADT<T>::getCount()-1);
	Node<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
		backPtr = nullptr ;	
		

	return true;

}
/////////////////////////////////////////////////////////////////////////////////////////
/*
Function: peek
copies the front of this queue to the passed param. The operation does not modify the queue.

Input: None.
Output: The front of the queue.
*/
template <typename T>
bool LinkedQueue<T>:: peek(T& frntEntry) const 
{
	if(isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}


///////////////////////////////////////////////////////////////////////////////////
/*
Function: destructor
removes all nodes from the queue by dequeuing them
*/
template <typename T>
 LinkedQueue<T>:: ~LinkedQueue()
{
	T temp;
	
	//Free (Dequeue) all nodes in the queue
	while(dequeue(temp));	
}
/////////////////////////////////////////////////////////////////////////////////////////
/*
Function: Copy constructor
To avoid shallow copy, 
copy constructor is provided

Input: LinkedQueue<T>: The Queue to be copied
Output: none
*/

template <typename T>
LinkedQueue<T>::LinkedQueue(const LinkedQueue<T> & LQ)
{	
	Node<T>* NodePtr = LQ.frontPtr;
	if (!NodePtr) //LQ is empty
	{
		frontPtr = backPtr = nullptr;
		return;
	}
	setCount(LQ.getCount()) ; 
	//insert the first node
	Node<T>* ptr = new Node<T>(NodePtr->getItem());
	frontPtr = backPtr = ptr;
	NodePtr = NodePtr->getNext();

	//insert remaining nodes
	while (NodePtr)
	{
		Node<T>* ptr = new Node<T>(NodePtr->getItem());
		backPtr->setNext(ptr);
		backPtr = ptr;
		NodePtr = NodePtr->getNext();
	}	


}

#endif
