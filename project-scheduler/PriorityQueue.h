#pragma once
#include "LinkedQueue.h"
#include "QueueADT.h"
template <class T>
class PriorityQueue : public QueueADT <T>
{
private:

	Node<T>* backPtr;
	Node<T>* frontPtr;
public:
	PriorityQueue()
	{
		QueueADT<T>::setCount(0);
		backPtr = nullptr;
		frontPtr = nullptr;
	}
	bool isEmpty() const
	{
		return (frontPtr == nullptr);
	}
	bool dequeue(T& frntEntry)
	{

		if (isEmpty())
			return false;

		QueueADT<T>::setCount(QueueADT<T>::getCount()-1);
		Node<T>* nodeToDeletePtr = frontPtr;
		frntEntry = frontPtr->getItem();
		frontPtr = frontPtr->getNext();
		// Queue is not empty; remove front
		if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
			backPtr = nullptr;

		// Free memory reserved for the dequeued node
		delete nodeToDeletePtr;

		return true;

	}
	bool peek(T& frntEntry)  const
	{
		if (isEmpty())
			return false;

		frntEntry = frontPtr->getItem();
		return true;

	}
	virtual ~PriorityQueue()
	{
		T temp;

		//Free (Dequeue) all nodes in the queue
		while (dequeue(temp));
	}
	PriorityQueue(const LinkedQueue<T>& LQ)
	{
		Node<T>* NodePtr = LQ.frontPtr;
		if (!NodePtr) //LQ is empty
		{
			QueueADT<T>::Count=0;
			frontPtr = backPtr = nullptr;
			return;
		}
		QueueADT<T>::Count=LQ->getCount();
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
	bool enqueue(const T& newEntry)
	{   
		QueueADT<T>::setCount(QueueADT<T>::getCount()+1);
		Node<T>* newNodePtr = new Node<T>(newEntry);
		// Insert the new node
		if (isEmpty())	//special case if this is the first node to insert
		{
			frontPtr = newNodePtr; // The queue is empty
			backPtr  = newNodePtr;
		}
		else
		{
			if (frontPtr->getItem() > newNodePtr->getItem())
			{
				newNodePtr->setNext(frontPtr);
				frontPtr = newNodePtr;
				return true;
			}
			else
			{
				Node <T>* ptr = frontPtr;
				while (ptr->getNext())
				{
					if (ptr->getNext()->getItem() > newNodePtr->getItem())
					{
						newNodePtr->setNext(ptr->getNext());
						ptr->setNext(newNodePtr);
						return true;
					}
					ptr = ptr->getNext();
				}
				if (ptr->getNext() == nullptr)
				{
					ptr->setNext(newNodePtr);
					backPtr = newNodePtr;
					return true;
				}
			}

		}
		// The queue was not empty
		return true;
	}
};

